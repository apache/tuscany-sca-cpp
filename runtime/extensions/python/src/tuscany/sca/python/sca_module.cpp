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

#include <string>
#include <iostream>
#include <sstream>

// undefine _DEBUG so Python does not need it's debug dll
#ifdef _DEBUG
#undef _DEBUG
#define _SCA_PYTHON_DEBUG
#endif
#include <Python.h>
#ifdef _SCA_PYTHON_DEBUG
#define _DEBUG
#endif

#include "commonj/sdo/SDO.h"

#include "PythonServiceProxy.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/Exceptions.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/model/ServiceBinding.h"
#include "tuscany/sca/core/Operation.h"

using namespace std;
using namespace commonj::sdo;
using namespace tuscany::sca;
using namespace tuscany::sca::model;

static PyObject* scaError;

/**
* Prints out PyObject and dir(PyObject)
* for debugging purposes
*/
static void printPyObject(char* prefix, char* name, PyObject* pObj)
{
    PyObject* pObjRepr = PyObject_Repr(pObj);    
    loginfo("%s printPyObject %s: %s", prefix, name, PyString_AsString(pObjRepr));
    Py_XDECREF(pObjRepr);

    if(pObj != NULL)
    {
        PyObject* pObjDir = PyObject_Dir(pObj);    
        PyObject* pObjDirRepr = PyObject_Repr(pObjDir);
        loginfo("%s printPyObject dir(%s): %s", prefix, name, PyString_AsString(pObjDirRepr));
        Py_DECREF(pObjDirRepr);
        Py_DECREF(pObjDir);
    }
}

static PyObject* sca_locateservice(PyObject *self, PyObject *args)
{
    logentry();
    
    // Get the service name
    PyObject* pServiceName = PyTuple_GetItem(args, 0);

    // Import the SCA python-extension module
    PyObject* pModuleName = PyString_FromString("sca_proxy");
    PyObject* sca_proxy_module = PyImport_Import(pModuleName);
    Py_DECREF(pModuleName);

    if(!sca_proxy_module)
    {
        if(PyErr_Occurred())
        {
            PyErr_Print();
        }
        string msg = "Failed to load the sca_proxy Python module - has it been successfully installed?\nReferences from Python components will not be supported";
        logerror(msg.c_str());
    }
    else
    {
        // Get the sca_proxy class
        PyObject* sca_proxy_class = PyObject_GetAttrString(sca_proxy_module, "sca_proxy_class");

        PyObject* scaArgs = PyTuple_New(2);
        PyTuple_SetItem(scaArgs, 0, pServiceName);
        Py_INCREF(Py_True);
        PyTuple_SetItem(scaArgs, 1, Py_False);

        // Create the instance of the scaReference class
        PyObject* sca_proxy_classInstance = PyInstance_New(sca_proxy_class, scaArgs, NULL);
        Py_DECREF(scaArgs);

        return sca_proxy_classInstance;
    }

    Py_INCREF(Py_None);
    return Py_None;
}


static tuscany::sca::python::PythonServiceProxy* getServiceProxy(PyObject *args)
{
    logentry();

    tuscany::sca::python::PythonServiceProxy* serviceProxy = NULL;
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
        loginfo("Service/Reference name is %s", name.c_str());
    }
    else
    {
        string msg = "Service/Reference name has not been set";
        logwarning(msg.c_str());
        PyErr_SetString(scaError, msg.c_str());
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
            string msg = "Could not find the reference: "+name;
            logwarning(msg.c_str());
            PyErr_SetString(scaError, msg.c_str());

            return NULL;
        }

        ReferenceBinding* refBinding = ref->getBinding();
        serviceProxy = (tuscany::sca::python::PythonServiceProxy*) refBinding->getServiceProxy();
    }
    else
    {
        Component* component = runtime->getDefaultComponent();    
        Composite* composite = (Composite*)component->getType();
        Service* service = composite->findComponentService(name);

        if(!service)
        {
            string msg = "Could not find service: "+name;
            logwarning(msg.c_str());
            PyErr_SetString(scaError, msg.c_str());
            return NULL;
        }

        serviceProxy = new tuscany::sca::python::PythonServiceProxy(service);
    }

    return serviceProxy;
}


static PyObject* sca_invoke(PyObject *self, PyObject *args)
{
    logentry();

    tuscany::sca::python::PythonServiceProxy* pythonServiceProxy = getServiceProxy(args);
    if(!pythonServiceProxy)
    {
        return NULL;
    } 

    // Get the component from the reference or service provided
    Component* component = NULL; 
    SCARuntime* runtime = SCARuntime::getInstance();

    PyObject* isReference = PyTuple_GetItem(args, 1);   
    if(PyObject_IsTrue(isReference))
    {
        component = runtime->getCurrentComponent(); 
    }
    else
    {
        component = runtime->getDefaultComponent();
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
        loginfo("Operation: %s", operationName.c_str());
    }
    else
    {
        string msg = "Operation name has not been set";
        logwarning(msg.c_str());
        PyErr_SetString(scaError, msg.c_str());
        return NULL;
    }

    // Create the Operation object
    Operation operation(operationName.c_str());

    // Load up the xml.etree.ElementTree module for dealing with SDO params and return values
    PyObject* elementTreeModuleName = PyString_FromString("xml.etree.ElementTree"); 
    PyObject* elementTreeModule = PyImport_Import(elementTreeModuleName);                        

    // Parameters are the fourth argument
    PyObject* paramTuple = PyTuple_GetItem(args, 3);

    // Go through the supplied parameters
    for(int i=0; i < PyTuple_Size(paramTuple); i++)
    {
        PyObject* param = PyTuple_GetItem(paramTuple, i);

        if(PyInt_Check(param))
        {
            loginfo("Int param %d: %d", i, PyInt_AsLong(param));
            long* intData = new long;
            *intData = PyInt_AsLong(param);
            operation.addParameter(intData);
        }
        else if(PyBool_Check(param))
        {
            loginfo("Bool param %d: %d", i, (param == Py_True));
            bool* boolData = new bool;
            *boolData = (param == Py_True);
            operation.addParameter(boolData);
        }
        else if(PyLong_Check(param))
        {
            loginfo("Long param %d: %l", i, PyLong_AsLong(param));
            long* longData = new long;
            *longData = PyLong_AsLong(param);
            operation.addParameter(longData);
        }
        else if(PyFloat_Check(param))
        {
            loginfo("Float param %d: %f", i, PyFloat_AsDouble(param));
            double* doubleData = new double;
            *doubleData = PyFloat_AsDouble(param);
            operation.addParameter(doubleData);
        }
        else if(PyString_Check(param))
        {
            loginfo("String param %d: %s", i, PyString_AsString(param));
            const char** stringData = new const char*; 
            *stringData = PyString_AsString(param);
            operation.addParameter(stringData);
        }
        else
        {
            // Get the xml.etree.ElementTree.iselement function
            PyObject* elementTreeIsElementFunc = PyObject_GetAttrString(elementTreeModule, "iselement");

            // Call the iselement() function with pValue to check it
            PyObject* pIsElement = PyObject_CallFunction(elementTreeIsElementFunc, "O", param);

            if(PyObject_IsTrue(pIsElement) == 1)
            {
                // pValue is an xml.etree.ElementTree.Element - convert to SDO
                PyObject* elementTreeToStringFunc = PyObject_GetAttrString(elementTreeModule, "tostring");
                PyObject* pElemString = PyObject_CallFunction(elementTreeToStringFunc, "O", param);
                char* data = PyString_AsString(pElemString);
                
                Py_DECREF(elementTreeToStringFunc);
                Py_DECREF(pElemString);

                loginfo("Converting Python ElementTree to SDO DataObject: %s", data);

                Composite* composite = component->getComposite();                                   
                XMLHelper* xmlHelper = composite->getXMLHelper();
                XMLDocumentPtr xmlDoc = xmlHelper->load(data);

                DataObjectPtr* dataObjectData = new DataObjectPtr;
                if (xmlDoc != NULL)
                {
                    *dataObjectData = xmlDoc->getRootDataObject();
                }
                else
                {
                    *dataObjectData = NULL;
                }
                if (*dataObjectData != NULL)
                {
                    operation.addParameter(dataObjectData);
                }
                else
                {
                    string msg = "xml.etree.ElementTree.Element could not be converted to a DataObject";
                    logerror(msg.c_str());
                    PyErr_SetString(scaError, msg.c_str());
                    return NULL;
                }                                    
            }
            else
            {

                PyObject* paramRepr = PyObject_Repr(param); 
                PyObject* paramType = PyObject_Type(param);             
                PyObject* paramTypeRepr = PyObject_Repr(paramType);    
                
                string msg = "sca_invoke Param ";
                msg += i;
                msg += "is of unknown type (";
                msg += PyString_AsString(paramTypeRepr);
                msg += ") and has repr: ";
                msg += PyString_AsString(paramRepr);
    
                logerror(msg.c_str());
                PyErr_SetString(scaError, msg.c_str());
                
                Py_DECREF(paramTypeRepr);
                Py_DECREF(paramType);
                Py_DECREF(paramRepr);

                return NULL;
            }
        }
    }
   
    PyObject* returnValue = NULL;

    try
    {
        // Invoke the wired service
        pythonServiceProxy->invokeService(operation);
    }
    catch(TuscanyRuntimeException& ex)
    {
        string msg = "Exception whilst invoking the ";
        msg += operationName.c_str();
        msg += " operation on an SCA service/reference: ";
        msg += ex.getEClassName();
        msg += ": ";
        msg += ex.getMessageText();
        logwarning(msg.c_str());
        PyErr_SetString(scaError, msg.c_str());
        return NULL;
    }
    catch(...)
    {
        string msg = "Exception whilst invoking the ";
        msg += operationName.c_str();
        msg += " operation on an SCA service/reference";

        logwarning(msg.c_str());
        PyErr_SetString(scaError, msg.c_str());
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
        case Operation::DATAOBJECT: 
            {
                DataObjectPtr dob = *(DataObjectPtr*)operation.getReturnValue();

                // Convert a DataObject to a xml.etree.ElementTree Element object
                Composite* composite = component->getComposite();                                    
                XMLHelper* xmlHelper = composite->getXMLHelper();
                char* str = xmlHelper->save(
                    dob,
                    dob->getType().getURI(),
                    dob->getType().getName());                                    

                loginfo("Converting SDO DataObject to Python ElementTree: %s", str);

                // Get the xml.etree.ElementTree.XML function
                PyObject* elementTreeXMLFunc = PyObject_GetAttrString(elementTreeModule, "XML");

                // Call the XML() function with the XML string 
                returnValue = PyObject_CallFunction(elementTreeXMLFunc, "s", str);

                Py_DECREF(elementTreeXMLFunc);
                break;
            }
        default:
            {
                Py_INCREF(Py_None);
                returnValue = Py_None;
            }

    }

    Py_XDECREF(elementTreeModuleName); 
    Py_XDECREF(elementTreeModule);                        

    return returnValue;
}
static PyMethodDef ModuleMethods[] = 
{
    {"locateservice", sca_locateservice, METH_VARARGS, "Locates an SCA service & returns an sca_proxy_class instance"},
    {"invoke", sca_invoke, METH_VARARGS, "Invoke an operation on an SCA service or reference"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initsca(void)
{
    logentry();

    // Create a new module 
    PyObject* module = Py_InitModule("sca", ModuleMethods);
    
    scaError = PyErr_NewException("sca.error", NULL, NULL);
    Py_INCREF(scaError);
    PyModule_AddObject(module, "error", scaError);
}
