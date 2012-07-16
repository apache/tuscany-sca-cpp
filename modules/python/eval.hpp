/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/* $Rev$ $Date$ */

#ifndef tuscany_python_eval_hpp
#define tuscany_python_eval_hpp

/**
 * Python script evaluation logic.
 */
#if PYTHON_VERSION == 27
#undef _POSIX_C_SOURCE
#undef _XOPEN_SOURCE
#include <python2.7/Python.h>
#include <python2.7/frameobject.h>
#include <python2.7/traceback.h>
#else
#undef _POSIX_C_SOURCE
#undef _XOPEN_SOURCE
#include <python2.6/Python.h>
#include <python2.6/frameobject.h>
#include <python2.6/traceback.h>
#endif

#include "list.hpp"
#include "value.hpp"

namespace tuscany {
namespace python {

class PythonThreadIn;
class PythonThreadOut;
class PythonRuntimeLock;
class PythonRuntime;

/**
 * Maintain a garbage collected reference to a Python object.
 */
const bool pyIncRef(PyObject* o);
const bool pyDecRef(PyObject* o, PythonRuntime* py);

/**
 * Write to debug log from Python.
 */
const value pyDebug(const list<value>& args) {
    debug(args, "python::debug");
    return true;
}

/**
 * Represent a Python runtime.
 */

class PythonRuntime {
public:
    PythonRuntime() {
        debug("python::pythonruntime");

        // Save current process id
#ifdef WANT_THREADS
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_init(&pidmutex, NULL);
#endif
        pid = processId();

        // Initialize the Python interpreter
#ifdef IS_DARWIN
        debug("python::pythonruntime::initialize");
        Py_SetPythonHome(const_cast<char*>(PYTHON_PREFIX));
        Py_InitializeEx(0);
#else
        if (!Py_IsInitialized()) {
            debug("python::pythonruntime::initialize");
            Py_SetPythonHome(const_cast<char*>(PYTHON_PREFIX));
            Py_InitializeEx(0);
        }
#endif

#ifdef WANT_THREADS
        // Initialize the Python thread support
#ifdef IS_DARWIN
        PyEval_InitThreads();
        PyEval_ReleaseLock();
#else
        if (!PyEval_ThreadsInitialized()) {
            PyEval_InitThreads();
            PyEval_ReleaseLock();
        }
#endif
#endif

#ifdef WANT_THREADS
        PyGILState_STATE gstate = PyGILState_Ensure();
#endif

        // Set default interpreter args
        const char* arg0 = "";
        PySys_SetArgv(0, const_cast<char**>(&arg0));

        // Install debug log function
        PyObject* mkPyLambda(const lambda<value(const list<value>&)>& l, PythonRuntime* py);
        PyObject* pyd= mkPyLambda(pyDebug, this);
        PyObject* sys = PyImport_ImportModule("sys");
        PyObject_SetAttrString(sys, "debug", pyd);
        pyDecRef(pyd, this);
        pyDecRef(sys, this);

#ifdef WANT_THREADS
        PyGILState_Release(gstate);
#endif
    }

    ~PythonRuntime() {
    }

private:
#ifdef WANT_THREADS
    pthread_mutex_t mutex;
    pthread_mutex_t pidmutex;
#endif
    unsigned long pid;

    friend class PythonThreadIn;
    friend class PythonThreadOut;
    friend class PythonRuntimeLock;
};

/**
 * Return the last python error.
 */
const string lastErrorTrace(PyObject *trace) {
    if (trace == NULL)
        return "";
    PyTracebackObject* tb = (PyTracebackObject*)trace;
    const int limit = 16;
    int depth = 0;
    PyTracebackObject *tb1 = tb;
    while (tb1 != NULL) {
        depth++;
        tb1 = tb1->tb_next;
    }
    ostringstream os;
    while (tb != NULL) {
        if (depth <= limit)
            os << ", File " << PyString_AsString(tb->tb_frame->f_code->co_filename) <<
                " line " << tb->tb_lineno << " in " << PyString_AsString(tb->tb_frame->f_code->co_name);
        depth--;
        tb = tb->tb_next;
    }
    return str(os);
}

const string lastError(PythonRuntime* py) {
    if(PyErr_Occurred()) {
        PyObject* type = NULL;
        PyObject* val = NULL;
        PyObject* trace = NULL;
        PyErr_Fetch(&type, &val, &trace);
        if (type != NULL && val != NULL) {
            PyObject* stype = PyObject_Repr(type);    
            PyObject* sval = PyObject_Repr(val);    
            string msg = string() + PyString_AsString(stype) + " : " + PyString_AsString(sval) + lastErrorTrace(trace);
            pyDecRef(stype, py);
            pyDecRef(sval, py);
            PyErr_Restore(type, val, trace);
            PyErr_Clear();
            return msg;
        }
        PyErr_Restore(type, val, trace);
        PyErr_Clear();
        return "Unknown Python error";
    }
    return "";
}

/**
 * Represent a lock on the Python runtime.
 */
class PythonRuntimeLock {
public:
    PythonRuntimeLock(PythonRuntime* py) : py(py) {
#ifdef WANT_THREADS
        pthread_mutex_lock(&py->mutex);
#endif
    }

    ~PythonRuntimeLock() {
#ifdef WANT_THREADS
        pthread_mutex_unlock(&py->mutex);
#endif
    }

private:
    PythonRuntime* py;
};

/**
 * Represent a thread calling into the Python interpreter.
 */
class PythonThreadIn {
public:
    PythonThreadIn(PythonRuntime* py) : py(py) {

        // Reinitialize Python thread support after a fork
        const unsigned long pid = processId();
#ifdef WANT_THREADS
        if (pid != py->pid) {
            pthread_mutex_lock(&py->pidmutex);
            if (pid != py->pid) {
                debug("python::gil::afterfork");
                PyOS_AfterFork();
                PyEval_ReleaseLock();
                debug("python::gil::afterforked");
                py->pid = pid;
            }
            pthread_mutex_unlock(&py->pidmutex);
        }

        // Acquire the Python GIL
        //debug("python::gil::ensure");
        gstate = PyGILState_Ensure();
        //debug("python::gil::ensured");
#else
        if (pid != py->pid) {
            debug("python::afterfork");
            PyOS_AfterFork();
            debug("python::afterforked");
            py->pid = pid;
        }
#endif
    }

    ~PythonThreadIn() {
        // Run Python cyclic reference garbage collector
        //const size_t c = PyGC_Collect();
        //debug(c, "python::gc::collect::c");

#ifdef WANT_THREADS
        // Release the Python GIL
        //debug("python::gil::release");
        PyGILState_Release(gstate);
        //debug("python::gil::released");
#endif
    }

private:
    PythonRuntime* py;
#ifdef WANT_THREADS
    PyGILState_STATE gstate;
#endif
};

/**
 * Represent a thread calling out of the Python interpreter.
 */
class PythonThreadOut {
public:
    PythonThreadOut(PythonRuntime* py) : py(py) {
#ifdef WANT_THREADS
        //debug("python::gil::save");
        tstate = PyEval_SaveThread();
        //debug("python::gil::saved");
#endif
    }

    ~PythonThreadOut() {
#ifdef WANT_THREADS
        //debug("python::gil::restore");
        PyEval_RestoreThread(tstate);
        //debug("python::gil::restored");
#endif
    }

private:
    PythonRuntime* py;
#ifdef WANT_THREADS
    PyThreadState* tstate;
#endif
};

/**
 * A gargabe collected Python object reference.
 */
class PyGCRef {
public:
    PyGCRef(PyObject* o, PythonRuntime* py) : o(o), py(py) {
    }

    ~PyGCRef() {
        if (o == NULL)
            return;
        PythonThreadIn pyin(py);
        Py_DECREF(o);
    }

private:
    PyObject* o;
    PythonRuntime* py;
};

/**
 * Maintain a garbage collected reference to a Python object.
 */
const bool pyIncRef(PyObject* o) {
    if (o == NULL)
        return true;
    Py_INCREF(o);
    return true;
}

const bool pyDecRef(unused PyObject* o, unused PythonRuntime* py) {
    if (o == NULL)
        return true;
    //new (gc_new<PyGCRef>()) PyGCRef(o, py);
    Py_DECREF(o);
    return true;
}

/**
 * Declare conversion functions.
 */
PyObject* valueToPyObject(const value& v, PythonRuntime* py);
const value pyObjectToValue(PyObject *o, PythonRuntime* py);
PyObject* valuesToPyTuple(const list<value>& v, PythonRuntime* py);
const list<value> pyTupleToValues(PyObject* o, PythonRuntime* py);

/**
 * Callable python type used to represent a lambda expression.
 */
typedef struct {
    PyObject_HEAD
    lambda<value(const list<value>&)>* func;
    PythonRuntime* py;
} pyLambda;

PyObject *mkPyLambda(const lambda<value(const list<value>&)>& l, PythonRuntime* py);

void pyLambda_dealloc(PyObject* self) {
    //debug(self, "python::pylambda_dealloc");
    PyObject_Del(self);
}

const string pyRepr(PyObject* o, PythonRuntime* py) {
    PyObject* r = PyObject_Repr(o);
    const string s = PyString_AsString(r);
    pyDecRef(r, py);
    return s;
}

const value pyLambda_callout(const pyLambda* pyl, const list<value>& args, PythonRuntime* py) {
    PythonThreadOut pyout(py);
    return (*(pyl->func))(args);
}

PyObject* pyLambda_call(PyObject* self, PyObject* args, unused PyObject* kwds) {
    debug("python::call");
    const pyLambda* pyl = (const pyLambda*)self;
    const value result =  pyLambda_callout(pyl, pyTupleToValues(args, pyl->py), pyl->py);
    debug(result, "python::call::result");
    PyObject *pyr = valueToPyObject(result, pyl->py);
    return pyr;
}

struct pyProxy {
    const value name;
    const lambda<value(const list<value>&)> func;

    pyProxy(const value& name, const lambda<value(const list<value>&)>& func) : name(name), func(func) {
    }

    const value operator()(const list<value>& args) const {
        debug(name, "python::proxy::name");
        const value result = func(cons<value>(name, args));
        debug(result, "python::proxy::result");
        return result;
    }
};

PyObject* pyLambda_getattr(PyObject *self, PyObject *attrname) {
    const string name = PyString_AsString(attrname);
    if (substr(name, 0, 1) == "_")
        return PyObject_GenericGetAttr(self, attrname);

    if (name == "eval") {
        pyIncRef(self);
        return self;
    }

    const pyLambda* pyl = (pyLambda*)self;
    debug(name, "python::getattr::name");
    PyObject* pyr = mkPyLambda(pyProxy(name, *(pyl->func)), pyl->py);
    return pyr;
}

 PyTypeObject pyLambda_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "lambda",                     /*tp_name*/
    sizeof(pyLambda),             /*tp_basicsize*/
    0,                            /*tp_itemsize*/
    /* methods */
    (destructor)pyLambda_dealloc, /*tp_dealloc*/
    0,                            /*tp_print*/
    0,                            /*tp_getattr*/
    0,                            /*tp_setattr*/
    0,                            /*tp_compare*/
    0,                            /*tp_repr*/
    0,                            /*tp_as_number*/
    0,                            /*tp_as_sequence*/
    0,                            /*tp_as_mapping*/
    0,                            /*tp_hash*/
    (ternaryfunc)pyLambda_call,   /*tp_call*/
    0,                            /*tp_str*/
    (binaryfunc)pyLambda_getattr, /*tp_getattro*/
    0,                            /*tp_setattro*/
    0,                            /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,           /*tp_flags*/
    0,                            /*tp_doc*/
    0,                            /*tp_traverse*/
    0,                            /*tp_clear*/
    0,                            /*tp_richcompare*/
    0,                            /*tp_weaklistoffset*/
    0,                            /*tp_iter*/
    0,                            /*tp_iternext*/
    0,                            /*tp_methods*/
    0,                            /*tp_members*/
    0,                            /*tp_getset*/
    0,                            /*tp_base*/
    0,                            /*tp_dict*/
    0,                            /*tp_descr_get*/
    0,                            /*tp_descr_set*/
    0,                            /*tp_dictoffset*/
    0,                            /*tp_init*/
    0,                            /*tp_alloc*/
    0,                            /*tp_new*/
    0,                            /*tp_free*/
    0,                            /*tp_is_gc*/
    0,                            /*tp_bases*/
    0,                            /*tp_mro*/
    0,                            /*tp_cache*/
    0,                            /*tp_subclasses*/
    0,                            /*tp_weaklist*/
    0,                            /*tp_del*/
    0                             /*tp_version_tag*/
};


/**
 * Create a new python object representing a lambda expression.
 */
PyObject *mkPyLambda(const lambda<value(const list<value>&)>& l, PythonRuntime* py) {
    pyLambda* pyl = PyObject_New(pyLambda, &pyLambda_type);
    if (pyl != NULL) {
        pyl->func = new (gc_new<lambda<value(const list<value>&)> >()) lambda<value(const list<value>&)>(l);
        pyl->py = py;
    }
    //debug(pyl, "python::mkpylambda");
    return (PyObject*)pyl;
}

/**
 * Convert a list of values to a python list.
 */
PyObject* valuesToPyListHelper(PyObject* l, const list<value>& v, PythonRuntime* py) {
    if (isNil(v))
        return l;
    PyObject* pyv = valueToPyObject(car(v), py);
    PyList_Append(l, pyv);
    pyDecRef(pyv, py);
    return valuesToPyListHelper(l, cdr(v), py);
}

PyObject* valuesToPyTuple(const list<value>& v, PythonRuntime* py) {
    PyObject* pyl = valuesToPyListHelper(PyList_New(0), v, py);
    PyObject* pyt = PyList_AsTuple(pyl);
    pyDecRef(pyl, py);
    return pyt;
}

/**
 * Convert a value to a python object.
 */
PyObject* valueToPyObject(const value& v, PythonRuntime* py) {
    switch (type(v)) {
    case value::List:
        return valuesToPyTuple(v, py);
    case value::Lambda:
        return mkPyLambda(v, py);
    case value::Symbol:
        return PyString_FromString(c_str(string("'") + v));
    case value::String: {
        const string s = (string)v;
        return PyString_FromStringAndSize(c_str(s), length(s));
    }
    case value::Number:
        return PyFloat_FromDouble((double)v);
    case value::Bool: {
        PyObject* b = (bool)v? Py_True : Py_False;
        pyIncRef(b);
        return b;
    }
    default: {
        PyObject* n = Py_None;
        pyIncRef(n);
        return n;
    }
    }
}

/**
 * Convert a python tuple to a list of values.
 */
const list<value> pyTupleToValuesHelper(PyObject* o, const size_t i, const size_t size, PythonRuntime* py) {
    if (i == size)
        return list<value>();
    return cons(pyObjectToValue(PyTuple_GetItem(o, i), py), pyTupleToValuesHelper(o, i + 1, size, py));
}

const list<value> pyTupleToValues(PyObject* o, PythonRuntime* py) {
    return pyTupleToValuesHelper(o, 0, PyTuple_Size(o), py);
}

/**
 * Lambda function used to represent a python callable object.
 */
struct pyCallable {
    PyObject* func;
    PythonRuntime* py;
    bool owner;

    pyCallable(PyObject* func, PythonRuntime* py) : func(func), py(py), owner(true) {
        pyIncRef(func);
    }

    pyCallable(const pyCallable& c) : func(c.func), py(c.py), owner(false) {
    }

    const pyCallable& operator=(const pyCallable& c) {
        if(this == &c)
            return *this;
        func = c.func;
        py = c.py;
        owner = false;
        return *this;
    }

    ~pyCallable() {
        if (!owner)
            return;
        pyDecRef(func, py);
    }

    const value operator()(const list<value>& args) const {
        PythonThreadIn pyin(py);
        if (debug_islogging()) {
            PyObject* rfunc = PyObject_Repr(func);
            char* s = NULL;
            Py_ssize_t l = 0;
            PyString_AsStringAndSize(rfunc, &s, &l);
            debug(string(s, l), "python::operator()::func");
            pyDecRef(rfunc, py);
        }
        PyObject* pyargs = valuesToPyTuple(args, py);
        if (debug_islogging()) {
            PyObject* rargs = PyObject_Repr(pyargs);
            char* s = NULL;
            Py_ssize_t l = 0;
            PyString_AsStringAndSize(rargs, &s, &l);
            debug(string(s, l), "python::operator()::args");
            pyDecRef(rargs, py);
        }

        PyObject* result = PyObject_CallObject(func, pyargs);
        pyDecRef(pyargs, py);

        const value v = pyObjectToValue(result, py);
        pyDecRef(result, py);
        return v;
    }
};

/**
 * Convert a python object to a value.
 */
const value pyObjectToValue(PyObject *o, PythonRuntime* py) {
    if (o == NULL)
        return value();
    if (PyString_Check(o)) {
        char* s = NULL;
        Py_ssize_t l = 0;
        PyString_AsStringAndSize(o, &s, &l);
        if (l != 0 && *s == '\'')
            return value(s + 1);
        return value(string(s, l));
    }
    if (PyBool_Check(o))
        return value(o == Py_True);
    if (PyInt_Check(o))
        return value((double)PyInt_AsLong(o));
    if (PyLong_Check(o))
        return value((double)PyLong_AsLong(o));
    if (PyFloat_Check(o))
        return value((double)PyFloat_AsDouble(o));
    if (PyTuple_Check(o))
        return pyTupleToValues(o, py);
    if (PyObject_TypeCheck(o, &pyLambda_type))
        return *(((pyLambda*)o)->func);
    if (PyCallable_Check(o))
        return lambda<value(const list<value>&)>(pyCallable(o, py));
    return value();
}

/**
 * Convert a python script path to a module name.
 */
const string moduleName(const string& path) {
    return join(".", tokenize("/", substr(path, 0, length(path) -3)));
}

/**
 * Evaluate an expression against a script provided as a python object.
 */
const failable<value> evalScript(const value& expr, PyObject* script, PythonRuntime& py) {
    PythonThreadIn pyin(&py);

    // Get the requested function
    PyObject* func = PyObject_GetAttrString(script, c_str(car<value>(expr)));
    if (func == NULL) {

        // The start, stop, and restart functions are optional
        const value fn = car<value>(expr);
        if (fn == "start" || fn == "stop") {
            PyErr_Clear();
            return value(lambda<value(const list<value>&)>());
        }

        return mkfailure<value>(string("Couldn't find function: ") + car<value>(expr) + " : " + lastError(&py));
    }
    if (!PyCallable_Check(func)) {
        pyDecRef(func, &py);
        return mkfailure<value>(string("Couldn't find callable function: ") + car<value>(expr));
    }

    // Convert args to python objects
    PyObject* args = valuesToPyTuple(cdr<value>(expr), &py);

    // Call the function
    PyObject* result = PyObject_CallObject(func, args);
    if (result == NULL) {
        const string msg = lastError(&py);
        pyDecRef(func, &py);
        pyDecRef(args, &py);
        return mkfailure<value>(string("Function call failed: ") + car<value>(expr) + " : " + msg);
    }
    pyDecRef(func, &py);
    pyDecRef(args, &py);

    // Convert python result to a value
    const value v = pyObjectToValue(result, &py);
    pyDecRef(result, &py);
    return v;
}

/**
 * Read a python script from an input stream.
 */
const failable<PyObject*> readScript(const string& name, const string& path, istream& is, PythonRuntime& py) {
    PythonThreadIn pyin(&py);

    // Lookup already loaded module
    PyObject *mods = PyImport_GetModuleDict();
    PyObject *emod = PyDict_GetItemString(mods, const_cast<char*>(c_str(name)));
    if (emod != NULL)
        return emod;

    // Compile and import new module
    debug(name, "python::readScript::compile::name");
    debug(path, "python::readScript::compile::path");
    const list<string> ls = streamList(is);
    ostringstream os;
    write(ls, os);
    PyObject* code = Py_CompileStringFlags(c_str(str(os)), c_str(path), Py_file_input, NULL);
    if (code == NULL)
        return mkfailure<PyObject*>(string("Couldn't compile script: ") + path + " : " + lastError(&py));
    PyObject* mod = PyImport_ExecCodeModuleEx(const_cast<char*>(c_str(name)), code, const_cast<char*>(c_str(path)));
    if (mod == NULL) {
        const string msg = lastError(&py);
        pyDecRef(code, &py);
        return mkfailure<PyObject*>(string("Couldn't import module: ") + path + " : " + msg);
    }
    pyDecRef(code, &py);
    pyDecRef(mod, &py);

    // Lookup the loaded module
    PyObject *lmod = PyDict_GetItemString(mods, const_cast<char*>(c_str(name)));
    if (lmod != NULL)
        return lmod;

    return mkfailure<PyObject*>(string("Couldn't lookup module: ") + path);
}

/**
 * Release a python script.
 */
const failable<bool> releaseScript(unused PyObject* script, PythonRuntime& py) {
    PythonThreadIn pyin(&py);
    // No need to decref the script here, as it's referenced only once from sys.modules
    return true;
}

/**
 * Evaluate an expression against a script provided as an input stream.
 */
const failable<value> evalScript(const value& expr, istream& is, PythonRuntime& py) {
    const value uuid = mkuuid();
    failable<PyObject*> script = readScript(string("script") + string(uuid), string("script") + string(uuid) + string(".py"), is, py);
    if (!hasContent(script))
        return mkfailure<value>(script);
    return evalScript(expr, content(script), py);
}

}
}
#endif /* tuscany_python_eval_hpp */
