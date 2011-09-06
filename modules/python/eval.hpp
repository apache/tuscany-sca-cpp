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
#include <python2.7/Python.h>
#else
#include <python2.6/Python.h>
#endif

#include "list.hpp"
#include "value.hpp"

namespace tuscany {
namespace python {

class PythonThreadIn;
class PythonThreadOut;
class PythonRuntimeLock;

/**
 * Represent a Python runtime.
 */

class PythonRuntime {
public:
    PythonRuntime() {
        debug("python::pythonruntime");

#ifdef WANT_THREADS
        pthread_mutex_init(&mutex, NULL);

#ifdef IS_DARWIN
        // Save current process id
        pthread_mutex_init(&pidmutex, NULL);
        pid = processId();
#endif
#endif

        // Initialize the Python interpreter
#ifdef IS_DARWIN
        debug("python::pythonruntime::initialize");
        Py_InitializeEx(0);
#else
        if (!Py_IsInitialized()) {
            debug("python::pythonruntime::initialize");
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

        // Set default interpreter args
#ifdef WANT_THREADS
        PyGILState_STATE gstate = PyGILState_Ensure();
#endif
        const char* arg0 = "";
        PySys_SetArgv(0, const_cast<char**>(&arg0));
#ifdef WANT_THREADS
        PyGILState_Release(gstate);
#endif
    }

    ~PythonRuntime() {
        debug("python::~pythonruntime");
    }

private:
#ifdef WANT_THREADS
    pthread_mutex_t mutex;
#ifdef IS_DARWIN
    pthread_mutex_t pidmutex;
    unsigned long pid;
#endif
#endif

    friend class PythonThreadIn;
    friend class PythonThreadOut;
    friend class PythonRuntimeLock;
};

/**
 * Return the last python error.
 */
const string lastError() {
    if(PyErr_Occurred()) {
        PyObject* type;
        PyObject* val;
        PyObject* trace;
        PyErr_Fetch(&type, &val, &trace);
        if (type != NULL && val != NULL) {
            PyObject* stype = PyObject_Str(type);    
            PyObject* sval = PyObject_Str(val);    
            string msg = string() + PyString_AsString(stype) + " : " + PyString_AsString(sval);
            Py_DECREF(stype);
            Py_DECREF(sval);
            PyErr_Restore(type, val, trace);
            PyErr_Print();
            return msg;
        }
        PyErr_Restore(type, val, trace);
        PyErr_Print();
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
#ifdef WANT_THREADS

#ifdef IS_DARWIN
        // Reinitialize Python thread support after a fork
        const unsigned long pid = processId();
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
#endif

        // Acquire the Python GIL
        //debug("python::gil::ensure");
        gstate = PyGILState_Ensure();
        //debug("python::gil::ensured");
#endif
    }

    ~PythonThreadIn() {
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
    debug(self, "python::pylambda_dealloc");
    PyObject_Del(self);
}

const string pyRepr(PyObject* o) {
    PyObject* r = PyObject_Repr(o);
    const string s = PyString_AsString(r);
    Py_DECREF(r);
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
        Py_INCREF(self);
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
    debug(pyl, "python::mkpylambda");
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
    Py_DECREF(pyv);
    return valuesToPyListHelper(l, cdr(v), py);
}

PyObject* valuesToPyTuple(const list<value>& v, PythonRuntime* py) {
    PyObject* pyl = valuesToPyListHelper(PyList_New(0), v, py);
    PyObject* pyt = PyList_AsTuple(pyl);
    Py_DECREF(pyl);
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
    case value::Bool:
        return (bool)v? Py_True : Py_False;
    default:
        return Py_None;
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

    pyCallable(PyObject* func, PythonRuntime* py) : func(func), py(py) {
        Py_INCREF(func);
    }

    ~pyCallable() {
        Py_DECREF(func);
    }

    const value operator()(const list<value>& args) const {
        PythonThreadIn pyin(py);
        PyObject* pyargs = valuesToPyTuple(args, py);
        PyObject* result = PyObject_CallObject(func, pyargs);
        const value v = pyObjectToValue(result, py);
        Py_DECREF(pyargs);
        Py_DECREF(result);
        return v;
    }
};

/**
 * Convert a python object to a value.
 */
const value pyObjectToValue(PyObject *o, PythonRuntime* py) {
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

        return mkfailure<value>(string("Couldn't find function: ") + car<value>(expr) + " : " + lastError());
    }
    if (!PyCallable_Check(func)) {
        Py_DECREF(func);
        return mkfailure<value>(string("Couldn't find callable function: ") + car<value>(expr));
    }

    // Convert args to python objects
    PyObject* args = valuesToPyTuple(cdr<value>(expr), &py);

    // Call the function
    PyObject* result = PyObject_CallObject(func, args);
    Py_DECREF(func);
    Py_DECREF(args);
    if (result == NULL)
        return mkfailure<value>(string("Function call failed: ") + car<value>(expr) + " : " + lastError());

    // Convert python result to a value
    const value v = pyObjectToValue(result, &py);
    Py_DECREF(result);
    return v;
}

/**
 * Read a python script from an input stream.
 */
const failable<PyObject*> readScript(const string& name, const string& path, istream& is, PythonRuntime& py) {
    PythonThreadIn pyin(&py);

    const list<string> ls = streamList(is);
    ostringstream os;
    write(ls, os);
    PyObject* code = Py_CompileStringFlags(c_str(str(os)), c_str(path), Py_file_input, NULL);
    if (code == NULL)
        return mkfailure<PyObject*>(string("Couldn't compile script: ") + path + " : " + lastError());
    PyObject* mod = PyImport_ExecCodeModuleEx(const_cast<char*>(c_str(name)), code, const_cast<char*>(c_str(path)));
    if (mod == NULL) {
        Py_DECREF(code);
        return mkfailure<PyObject*>(string("Couldn't import module: ") + path + " : " + lastError());
    }
    Py_DECREF(code);
    return mod;
}

/**
 * Release a python script.
 */
const failable<bool> releaseScript(PyObject* script, PythonRuntime& py) {
    PythonThreadIn pyin(&py);
    Py_DECREF(script);
    return true;
}

/**
 * Evaluate an expression against a script provided as an input stream.
 */
const failable<value> evalScript(const value& expr, istream& is, PythonRuntime& py) {
    failable<PyObject*> script = readScript("script", "script.py", is, py);
    if (!hasContent(script))
        return mkfailure<value>(reason(script));
    return evalScript(expr, content(script), py);
}

}
}
#endif /* tuscany_python_eval_hpp */
