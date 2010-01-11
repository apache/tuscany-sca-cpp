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

#ifndef tuscany_python_io_hpp
#define tuscany_python_io_hpp

/**
 * Hooks used to capture python stdout and stderr.
 */
#include <python2.6/Python.h>

#include "stream.hpp"
#include "../scheme/io.hpp"

namespace tuscany {
namespace python {

/**
 * Hook method used to redirect python output to a stream.
 */
PyObject* display(unused PyObject* self, PyObject* args) {
    char* s = NULL;
    if (!PyArg_ParseTuple(args, "s", &s))
        return NULL;

    scheme::displayStream() << s;

    Py_INCREF(Py_None);
    return Py_None;
}

PyMethodDef displayHookMethods[] = {
        {"display", display, METH_VARARGS, "Redirects stdout"},
        {NULL, NULL, 0, NULL}};

/**
 * Hook method used to redirect python errors to a stream.
 */
PyObject* log(unused PyObject* self, PyObject* args) {
    char* s = NULL;
    if (!PyArg_ParseTuple(args, "s", &s))
        return NULL;

    scheme::logStream() << s;

    Py_INCREF(Py_None);
    return Py_None;
}

PyMethodDef logHookMethods[] = {
        {"log", log, METH_VARARGS, "Redirects stdout"},
        {NULL, NULL, 0, NULL}};

/**
 * Setup the display and log hooks.
 */
bool setupIO() {
    Py_InitModule("displayhook", displayHookMethods);
    PyRun_SimpleString(
            "import sys\n"
            "import displayhook\n"
            "\n"
            "class DisplayHook:\n"
            "    def write(self, text):\n"
            "        displayhook.display(text)\n"
            "\n"
            "sys.stdout = DisplayHook()\n");

    Py_InitModule("loghook", logHookMethods);
    PyRun_SimpleString(
            "import sys\n"
            "import loghook\n"
            "\n"
            "class LogHook:\n"
            "    def write(self, text):\n"
            "        loghook.log(text)\n"
            "\n"
            "sys.stderr = LogHook()\n");
    return true;
}

/**
 * Return the last python error.
 */
const string lastError() {
    ostream* pos = scheme::logOutStream;
    ostringstream eos;
    scheme::logOutStream = &eos;
    if (PyErr_Occurred())
        PyErr_Print();
    scheme::logOutStream = pos;
    return str(eos);
}

/**
 * Read and write values (using the scheme representation as it's the canonical
 * representation used throughout the runtime.)
 */
const value readValue(istream& in) {
    return scheme::readValue(in);
}

const bool writeValue(const value& val, ostream& out) {
    return scheme::writeValue(val, out);
}

}
}
#endif /* tuscany_python_io_hpp */
