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

#include <Python.h>

#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/model/ServiceBinding.h"
#include "tuscany/sca/core/Operation.h"
using namespace tuscany::sca;
using namespace tuscany::sca::model;

#include "PythonServiceProxy.h"
using namespace tuscany::sca::python;

#include "tuscany/sca/util/Logging.h"

#include <string>
#include <iostream>
using namespace std;

static PyObject* tuscanySCAError;

/**
* Prints out PyObject and dir(PyObject)
* for debugging purposes
*/
static void printPyObject(char* prefix, char* name, PyObject* pObj)
{
    LOGINFO(4, "Entering printPyObject");
    PyObject* pObjRepr = PyObject_Repr(pObj);    
    LOGINFO_3(4, "%s printPyObject %s: %s", prefix, name, PyString_AsString(pObjRepr));
    Py_XDECREF(pObjRepr);

    if(pObj != NULL)
    {
        PyObject* pObjDir = PyObject_Dir(pObj);    
        PyObject* pObjDirRepr = PyObject_Repr(pObjDir);
        LOGINFO_3(4, "%s printPyObject dir(%s): %s", prefix, name, PyString_AsString(pObjDirRepr));
        Py_DECREF(pObjDirRepr);
        Py_DECREF(pObjDir);
    }
    LOGINFO(4, "Returning from printPyObject");
}

static PyObject* TuscanySCA_locateService(PyObject *self, PyObject *args)
{
    // Get the service name
    PyObject* pServiceName = PyTuple_GetItem(args, 0);

    // Import the TuscanySCA python-extension module
    PyObject* pModuleName = PyString_FromString("TuscanySCAProxy");
    PyObject* tuscanySCAProxyModule = PyImport_Import(pModuleName);
    Py_DECREF(pModuleName);

    if(!tuscanySCAProxyModule)
    {
        if(PyErr_Occurred())
        {
            PyErr_Print();
        }
        string msg = "Failed to load the TuscanySCAProxy Python module - has it been successfully installed?\nReferences from Python components will not be supported";
        LOGERROR(0, msg.c_str());
    }
    else
    {
        // Get the TuscanySCAProxy class
        PyObject* tuscanySCAProxyClass = PyObject_GetAttrString(tuscanySCAProxyModule, "TuscanySCAProxyClass");

        PyObject* tuscanySCAArgs = PyTuple_New(2);
        PyTuple_SetItem(tuscanySCAArgs, 0, pServiceName);
        Py_INCREF(Py_True);
        PyTuple_SetItem(tuscanySCAArgs, 1, Py_False);

        // Create the instance of the TuscanySCAReference class
        PyObject* tuscanySCAProxyClassInstance = PyInstance_New(tuscanySCAProxyClass, tuscanySCAArgs, NULL);
        Py_DECREF(tuscanySCAArgs);

        return tuscanySCAProxyClassInstance;
    }

    Py_INCREF(Py_None);
    return Py_None;
}


static PythonServiceProxy* getServiceProxy(PyObject *args)
{
    LOGENTRY(1, "TuscanySCAModule getPythonServiceProxy");

    PythonServiceProxy* serviceProxy = NULL;
    SCARuntime* runtime = SCARuntime::getInstance();

    // The first argument holds the name
    string name;
    PyObject* pName = PyTuple_GetItem(args, 0);
    if(pName && PyString_Check(pName))
    {
        name = PyString_AsString(pName);
    }
    if(name.size() > 0)
    {
        LOGINFO_1(3, "TuscanySCA_invoke Service/Reference name is %s", name.c_str());
    }
    else
    {
        string msg = "TuscanySCA_invoke Service/Reference name has not been set";
        LOGERROR(1, msg.c_str());
        PyErr_SetString(tuscanySCAError, msg.c_str());
        return NULL;
    }

    // The second argument is a boolean
    PyObject* isReference = PyTuple_GetItem(args, 1);

    // Get the serviceProxy from the reference or service name provided
    if(PyObject_IsTrue(isReference))
    {
        Component* component = runtime->getCurrentComponent();    
        Reference* ref = component->findReference(name);
        if(!ref)
        {
            string msg = "TuscanySCA_invoke Could not find the reference named "+name;
            LOGERROR(1, msg.c_str());
            PyErr_SetString(tuscanySCAError, msg.c_str());

            return NULL;
        }

        ReferenceBinding* refBinding = ref->getBinding();
        serviceProxy = (PythonServiceProxy*) refBinding->getServiceProxy();
    }
    else
    {
        Component* component = runtime->getDefaultComponent();    
        Composite* composite = (Composite*)component->getType();
        Service* service = composite->findComponentService(name);

        if(!service)
        {
            string msg = "TuscanySCA_invoke Could not find the service named "+name;
            LOGERROR(1, msg.c_str());
            PyErr_SetString(tuscanySCAError, msg.c_str());
            return NULL;
        }

        serviceProxy = new PythonServiceProxy(service);
    }

    return serviceProxy;
}


static PyObject* TuscanySCA_invoke(PyObject *self, PyObject *args)
{
    LOGENTRY(1, "TuscanySCA_invoke");

    PythonServiceProxy* pythonServiceProxy = getServiceProxy(args);
    if(!pythonServiceProxy)
    {
        return NULL;
    }

    // Get the name of the operation to invoke
    string operationName;
    PyObject* opName = PyTuple_GetItem(args, 2);
    if(opName && PyString_Check(opName))
    {
        operationName = PyString_AsString(opName);
    }

    if(operationName.size() > 0)
    {
        LOGINFO_1(3, "TuscanySCA_invoke Operation name is %s", operationName.c_str());
    }
    else
    {
        string msg = "TuscanySCA_invoke Operation name has not been set";
        LOGERROR(1, msg.c_str());
        PyErr_SetString(tuscanySCAError, msg.c_str());
        return NULL;
    }

    // Create the Operation object
    Operation operation(operationName.c_str());

    // Parameters are the fourth argument
    PyObject* paramTuple = PyTuple_GetItem(args, 3);

    // Go through the supplied parameters
    for(int i=0; i < PyTuple_Size(paramTuple); i++)
    {
        PyObject* param = PyTuple_GetItem(paramTuple, i);

        if(PyInt_Check(param))
        {
            LOGINFO_2(3, "TuscanySCA_invoke Param %d is int type: %d", i, PyInt_AsLong(param));
            long* intData = new long;
            *intData = PyInt_AsLong(param);
            operation.addParameter(intData);
        }
        else if(PyBool_Check(param))
        {
            LOGINFO_2(3, "TuscanySCA_invoke Param %d is bool type: %d", i, (param == Py_True));
            bool* boolData = new bool;
            *boolData = (param == Py_True);
            operation.addParameter(boolData);
        }
        else if(PyLong_Check(param))
        {
            LOGINFO_2(3, "TuscanySCA_invoke Param %d is long type: %l", i, PyLong_AsLong(param));
            long* longData = new long;
            *longData = PyLong_AsLong(param);
            operation.addParameter(longData);
        }
        else if(PyFloat_Check(param))
        {
            LOGINFO_2(3, "TuscanySCA_invoke Param %d is float type: %f", i, PyFloat_AsDouble(param));
            double* doubleData = new double;
            *doubleData = PyFloat_AsDouble(param);
            operation.addParameter(doubleData);
        }
        else if(PyString_Check(param))
        {
            LOGINFO_2(3, "TuscanySCA_invoke %d is string type: %s", i, PyString_AsString(param));
            const char** stringData = new const char*; 
            *stringData = PyString_AsString(param);
            operation.addParameter(stringData);
        }
        else
        {
            PyObject* paramRepr = PyObject_Repr(param); 
            PyObject* paramType = PyObject_Type(param);             
            PyObject* paramTypeRepr = PyObject_Repr(paramType);    
            
            string msg = "TuscanySCA_invoke Param ";
            msg += i;
            msg += "is of unknown type (";
            msg += PyString_AsString(paramTypeRepr);
            msg += ") and has repr: ";
            msg += PyString_AsString(paramRepr);

            LOGERROR(1, msg.c_str());
            PyErr_SetString(tuscanySCAError, msg.c_str());
            
            Py_DECREF(paramTypeRepr);
            Py_DECREF(paramType);
            Py_DECREF(paramRepr);

            return NULL;
        }
    }
   
    PyObject* returnValue = NULL;

    try
    {
        // Invoke the wired service
        pythonServiceProxy->invokeService(operation);
    }
    catch(...)
    {
        string msg = "TuscanySCA_invoke Exception thrown whilst invoking the service";
        LOGERROR(1, msg.c_str());
        PyErr_SetString(tuscanySCAError, msg.c_str());
        return NULL;
    }


    switch(operation.getReturnType())
    {
        case Operation::BOOL: 
            {
                if(*(bool*)operation.getReturnValue())
                {
                    returnValue = Py_True;
                }
                else
                {
                    returnValue = Py_False;
                }
                break;
            }
        case Operation::SHORT: 
            {
                returnValue = PyInt_FromLong(*(short*)operation.getReturnValue());
                break;
            }
        case Operation::LONG: 
            {
                returnValue = PyLong_FromLong(*(long*)operation.getReturnValue());
                break;
            }
        case Operation::USHORT: 
            {
                returnValue = PyInt_FromLong(*(unsigned short*)operation.getReturnValue());
                break;
            }
        case Operation::ULONG: 
            {
                returnValue = PyLong_FromLong(*(unsigned long*)operation.getReturnValue());
                break;
            }
        case Operation::FLOAT: 
            {
                returnValue = PyFloat_FromDouble(*(float*)operation.getReturnValue());
                break;
            }
        case Operation::DOUBLE: 
            {
                returnValue = PyFloat_FromDouble(*(double*)operation.getReturnValue());
                break;
            }
        case Operation::LONGDOUBLE: 
            {
                returnValue = PyFloat_FromDouble(*(long double*)operation.getReturnValue());
                break;
            }
        case Operation::CHARS: 
            {
                returnValue = PyString_FromString(*(char**)operation.getReturnValue());
                break;
            }
        case Operation::STRING: 
            {
                returnValue = PyString_FromString((*(string*)operation.getReturnValue()).c_str());
                break;
            }
        default:
            {
                Py_INCREF(Py_None);
                returnValue = Py_None;
            }

    }

    LOGEXIT(1, "TuscanySCA_invoke");
    return returnValue;
}
static PyMethodDef ModuleMethods[] = 
{
    {"locateService", TuscanySCA_locateService, METH_VARARGS, "Locates a TuscanySCA service & returns a TuscanySCAReference instance"},
    {"invoke", TuscanySCA_invoke, METH_VARARGS, "Invoke an operation on a Tuscany service or reference"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initTuscanySCA(void)
{
    LOGENTRY(1, "initTuscanySCA");

    // Create a new module 
    PyObject* module = Py_InitModule("TuscanySCA", ModuleMethods);
    
    tuscanySCAError = PyErr_NewException("TuscanySCA.error", NULL, NULL);
    Py_INCREF(tuscanySCAError);
    PyModule_AddObject(module, "error", tuscanySCAError);

    LOGEXIT(1, "initTuscanySCA");
}
