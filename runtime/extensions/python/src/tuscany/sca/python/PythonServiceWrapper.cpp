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

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/python/PythonServiceWrapper.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/Exceptions.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/Interface.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/python/model/PythonImplementation.h"

using namespace std;
using namespace commonj::sdo;
using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        namespace python
        {
             
            /**
            * Prints out PyObject and dir(PyObject)
            * for debugging purposes
            */
            void printPyObject(char * name, PyObject *pObj)
            {
                PyObject* pObjRepr = PyObject_Repr(pObj);    
                loginfo("PyObject %s: %s", name, PyString_AsString(pObjRepr));
                Py_DECREF(pObjRepr);

                PyObject* pObjDir = PyObject_Dir(pObj);    
                PyObject* pObjDirRepr = PyObject_Repr(pObjDir);
                loginfo("PyObject dir(%s): %s", name, PyString_AsString(pObjDirRepr));
                Py_DECREF(pObjDirRepr);
                Py_DECREF(pObjDir);
            }

            // ===========
            // Constructor
            // ===========
            PythonServiceWrapper::PythonServiceWrapper(Service* service)
                : ServiceWrapper(service)
            {
                logentry();
    
                component = service->getComponent();
                implementation = (PythonImplementation*)component->getType();              

                pythonModule = NULL;
                pythonClassInstance = NULL;
                
                // -----------------------------------------------
                // Get the implementation for the target component
                // -----------------------------------------------
                PythonImplementation* impl = (PythonImplementation*)component->getType();
                if (!impl)
                {
                    string msg = "Component " + component->getName() + " has no implementation defined";
                    throwException(SystemConfigurationException, msg.c_str());
                }

                // Initialize the Python environment
                Py_Initialize();

                // Add the path to the composite (+ any further path specified) to the Python sys.path
                string path = component->getComposite()->getRoot();
                if(impl->getModulePath().size() > 0)
                {
                    path += "/" + impl->getModulePath();
                }

                loginfo("Module: %s", impl->getModule().c_str());
                loginfo("Path: %s", path.c_str());
                loginfo("Class: %s", impl->getClass().c_str());

                PyObject* pSysName = PyString_FromString("sys");
                PyObject* pSys = PyImport_Import(pSysName);
                Py_DECREF(pSysName);

                if(pSys != NULL)
                {
                    PyObject* pSysPath = PyObject_GetAttrString(pSys, "path");

                    if(pSysPath != NULL && PyList_Check(pSysPath))
                    {
                        PyObject* pPath = PyString_FromString(path.c_str());
                        PyList_Append(pSysPath, pPath);

                        Py_DECREF(pPath);
                        Py_DECREF(pSysPath);
                    }
                    Py_DECREF(pSys);
                }

                if(&(impl->getModule()) != NULL && impl->getModule().size() > 0)
                {
                    // Now import the module
                    PyObject* pModuleName = PyString_FromString(impl->getModule().c_str());

                    pythonModule = PyImport_Import(pModuleName);
                    Py_DECREF(pModuleName);
                }

                if (!pythonModule)
                {
                    if(PyErr_Occurred())
                    {
                        PyErr_Print();
                    }
                    string msg = "Failed to load module named " + impl->getModule() + " on path " + path;
                    throwException(SystemConfigurationException, msg.c_str());
                }
                printPyObject("pythonModule",pythonModule);
            }
            
            // ==========
            // Destructor
            // ==========
            PythonServiceWrapper::~PythonServiceWrapper()
            {
                logentry();

                Py_XDECREF(pythonClassInstance);
                Py_XDECREF(pythonModule);
                Py_Finalize();
            }
            
            // ======================================================================
            // newInstance: create a new class instance
            // ======================================================================
            PyObject* PythonServiceWrapper::newInstance()
            {
                logentry();

                PythonImplementation* impl = (PythonImplementation*)component->getType();
                string className = impl->getClass();

                PyObject* pClassInstance = NULL;
                if (pythonModule != NULL) 
                {
                    if(&className != NULL && className.size() > 0)
                    {
                        // We have a class name, so create an instance and use this to invoke the correct function
                        PyObject* pClass = PyObject_GetAttrString(pythonModule, (char*) className.c_str());

                        if(pClass == NULL)
                        {
                            if(PyErr_Occurred())
                            {
                                PyErr_Print();
                            }
                            string msg = "Cannot find class named " + className + " in Python module";
                            throwException(SystemConfigurationException, msg.c_str());
                        }

                        pClassInstance = PyInstance_New(pClass, NULL, NULL);
                        
                        if(pClassInstance == NULL || !PyInstance_Check(pClassInstance))
                        {
                            if(PyErr_Occurred())
                            {
                                PyErr_Print();
                            }
                            string msg = "Could not create new instance of class named " + className + " in Python module";
                            throwException(ServiceInvocationException, msg.c_str());
                        }
                        Py_XDECREF(pClass);
                    }
                }
                return pClassInstance;
            }

            // ======================================================================
            // getInstance: get a class instance for this scope
            // ======================================================================
            PyObject* PythonServiceWrapper::getInstance()
            {
                logentry();

                PythonImplementation::Scope scope = implementation->getScope();
                if (scope == PythonImplementation::COMPOSITE)
                {
                    if (!pythonClassInstance)
                    {
                        pythonClassInstance = newInstance();
                    }
                    return pythonClassInstance;
                }
                else // (scope == PythonImplementation::STATELESS)
                {
                    return newInstance();
                }        
            }
            
            // ======================================================================
            // releaseImplementation: release the implementation for this scope
            // ======================================================================
            void PythonServiceWrapper::releaseInstance()
            {
                logentry();
                
                PythonImplementation::Scope scope = implementation->getScope();
                if(scope == PythonImplementation::STATELESS)
                {
                    // Delete the class instance if there is one
                    if(pythonClassInstance != NULL && PyInstance_Check(pythonClassInstance))
                    {
                        Py_DECREF(pythonClassInstance);
                        pythonClassInstance = NULL;
                    }

                    // Need to reload the module
                    PyObject* reloadedPythonModule = PyImport_ReloadModule(pythonModule);

                    if(reloadedPythonModule != NULL)
                    {
                        // Get rid of old pythonModule and replace with the reloaded one
                        Py_DECREF(pythonModule);
                        pythonModule = reloadedPythonModule;
                    }
                }
            }
            
            // ======================================================================
            // invoke: wrapper call to service with setting the component context
            // ======================================================================
            void PythonServiceWrapper::invoke(Operation& operation)
            {
                logentry();
    
                SCARuntime* runtime = SCARuntime::getCurrentRuntime();
                runtime->setCurrentComponent(component);
                

                // Load the references & properties into the module
                addReferences(pythonModule);
                addProperties(pythonModule);

                try
                {
                    loginfo("Operation: %s", operation.getName().c_str());

                    PyObject* pFunc = NULL; 
                    pythonClassInstance = getInstance();

                    if(pythonClassInstance != NULL && PyInstance_Check(pythonClassInstance))
                    {
                        // Get the function from the instance
                        pFunc = PyObject_GetAttrString(pythonClassInstance, (char*) operation.getName().c_str());
                    }
                    if(pFunc == NULL && pythonModule != NULL)
                    {
                        // Get the function directly from the module if it could not be got from an instance
                        pFunc = PyObject_GetAttrString(pythonModule, (char*) operation.getName().c_str());
                    }
                    if(pFunc == NULL)
                    {
                        // Can't get the function from the class or module
                        string msg = "Python module or class instance has not been created";
                        throwException(ServiceInvocationException, msg.c_str());
                    }

                    if (pFunc && PyCallable_Check(pFunc)) 
                    {
                        PyObject* pArgs = PyTuple_New(operation.getNParms());
                        PyObject* pKeywordsDict = PyDict_New();
                        PyObject* pValue = NULL;

                        // Load up the xml.etree.ElementTree module for dealing with SDO params and return values
                        PyObject* elementTreeModuleName = PyString_FromString("xml.etree.ElementTree"); 
                        PyObject* elementTreeModule = PyImport_Import(elementTreeModuleName); 

                        if(elementTreeModule == NULL)
                        {
                            // pre-Python2.5? - try to get an installed elementtree package
                            elementTreeModuleName = PyString_FromString("elementtree.ElementTree"); 
                            elementTreeModule = PyImport_Import(elementTreeModuleName); 
                        }
                        if(elementTreeModule == NULL)
                        {
                            // Still null - throw a warning but carry on - user may not need XML
                            logwarning("Could not load Python ElementTree module - is it installed? SDO and XML will not be supported");
                        }

                        for(unsigned int i = 0; i < operation.getNParms(); i++) 
                        {
                            const Operation::Parameter& parm = operation.getParameter(i);
    		                switch(parm.getType())
    		                {
    			                case Operation::BOOL: 
    			                {
    				                if( *(bool*)parm.getValue())
                                    {
                                        //boolean true
                                        pValue = Py_True;
                                    }
                                    else
                                    {
                                        pValue = Py_False;
                                    }
    				                break;
    			                }
    			                case Operation::SHORT: 
    			                {
                                    pValue = PyInt_FromLong(*(short*)parm.getValue());
    				                break;
    			                }
    			                case Operation::USHORT: 
    			                {
                                    pValue = PyInt_FromLong(*(unsigned short*)parm.getValue());
    				                break;
    			                }
    			                case Operation::INT: 
    			                {
                                    pValue = PyInt_FromLong(*(int*)parm.getValue());
    				                break;
    			                }
    			                case Operation::UINT: 
    			                {
                                    pValue = PyInt_FromLong(*(unsigned int*)parm.getValue());
    				                break;
    			                }
    			                case Operation::LONG: 
    			                {
                                    pValue = PyLong_FromLong(*(long*)parm.getValue());
    				                break;
    			                }
    			                case Operation::ULONG: 
    			                {
                                    pValue = PyLong_FromUnsignedLong(*(unsigned long*)parm.getValue());
    				                break;
    			                }
    			                case Operation::FLOAT: 
    			                {
                                    pValue = PyFloat_FromDouble(*(float*)parm.getValue());
    				                break;
    			                }
    			                case Operation::DOUBLE: 
    			                {
                                    pValue = PyFloat_FromDouble(*(double*)parm.getValue());
    				                break;
    			                }
    			                case Operation::LONGDOUBLE: 
    			                {
                                    pValue = PyFloat_FromDouble(*(long double*)parm.getValue());
    				                break;
    			                }
    			                case Operation::CHARS: 
    			                {
                                    pValue = PyString_FromString(*(char**)parm.getValue());
    				                break;
    			                }
    			                case Operation::STRING: 
    			                {
                                    pValue = PyString_FromString((*(string*)parm.getValue()).c_str());
    				                break;
    			                }
                                case Operation::DATAOBJECT: 
                                {
                                    if(elementTreeModule != NULL)
                                    {
                                        DataObjectPtr dob = *(DataObjectPtr*)parm.getValue();

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
                                        pValue = PyObject_CallFunction(elementTreeXMLFunc, "s", str);

                                        Py_DECREF(elementTreeXMLFunc);
                                    }
                                    else
                                    {
                                        throwException(ServiceDataException, "Could not convert SDO DataObject to Python ElementTree as ElementTree module could not be loaded");
                                    }
                                    break;
                                }
                                default:
                                    throwException(ServiceDataException, "Operation parameter type not supported");
    		                }

                            if (!pValue) 
                            {
                                Py_DECREF(pArgs);

                                if(PyErr_Occurred())
                                {
                                    PyErr_Print();
                                }

                                string msg = "Error converting parameter into Python type";
                                throwException(ServiceDataException, msg.c_str());
                                
                            }
                            //printPyObject("Param value", pValue);                                                        

                            // If we have a param name, put it in the keyword args
                            if(parm.hasName())
                            {
                                PyDict_SetItemString(pKeywordsDict, parm.getName().c_str(), pValue);
                                Py_DECREF(pValue);
                            }
                            else
                            {   
                                /* pValue reference stolen here: */
                                PyTuple_SetItem(pArgs, i, pValue);
                            }
                        }

                        // Resize the args to the correct length
                        _PyTuple_Resize(&pArgs, operation.getNParms() - PyDict_Size(pKeywordsDict));

                        loginfo("Calling python func with %d args and %d keyword args", PyTuple_Size(pArgs), PyDict_Size(pKeywordsDict));

                        pValue = PyObject_Call(pFunc, pArgs, pKeywordsDict);
                        //printPyObject("Return value", pValue);

                        Py_DECREF(pArgs);
                        if (pValue != NULL) 
                        {
                            char buf[20];
                            if(PyInt_Check(pValue) || PyLong_Check(pValue))
                            {
                                long* data = new long;
                                if(PyInt_Check(pValue))
                                {
                                    loginfo("Int return value: %d", PyInt_AsLong(pValue));           
                                    *data = PyInt_AsLong(pValue);
                                }
                                else
                                {
                                    loginfo("Long return value: %l", PyLong_AsLong(pValue));
                                    *data = PyLong_AsLong(pValue);                                
                                }

                                // Check if the return type has already been set (for typed languages)
    		                    switch(operation.getReturnType())
    		                    {
    			                    case Operation::BOOL: 
    			                    {
    				                    *(bool*)operation.getReturnValue() = (*data != 0);
    				                    break;
    			                    }
    			                    case Operation::SHORT: 
    			                    {
                                        *(short*)operation.getReturnValue() = (short)*data;
    				                    break;
    			                    }
    			                    case Operation::USHORT: 
    			                    {
                                        *(unsigned short*)operation.getReturnValue() = (unsigned short)*data;
    				                    break;
    			                    }
    			                    case Operation::INT: 
    			                    {
                                        *(int*)operation.getReturnValue() = (int)*data;
    				                    break;
    			                    }
    			                    case Operation::UINT: 
    			                    {
                                        *(unsigned int*)operation.getReturnValue() = (unsigned int)*data;
    				                    break;
    			                    }
    			                    case Operation::LONG: 
    			                    {
                                        *(long*)operation.getReturnValue() = (long)*data;
    				                    break;
    			                    }
    			                    case Operation::ULONG: 
    			                    {
                                        *(unsigned long*)operation.getReturnValue() = (unsigned long)*data;
    				                    break;
    			                    }
    			                    case Operation::FLOAT: 
    			                    {
                                        *(float*)operation.getReturnValue() = (float)*data;
    				                    break;
    			                    }
    			                    case Operation::DOUBLE: 
    			                    {
                                        *(double*)operation.getReturnValue() = (double)*data;
    				                    break;
    			                    }
    			                    case Operation::LONGDOUBLE: 
    			                    {
                                        *(long double*)operation.getReturnValue() = (long double)*data;
    				                    break;
    			                    }
    			                    case Operation::CHARS: 
    			                    {
                                        sprintf(buf, "%d", *data);
                                        *(char**)operation.getReturnValue() = buf;
    				                    break;
    			                    }
    			                    case Operation::STRING: 
    			                    {
                                        sprintf(buf, "%d", *data);
                                        *(string*)operation.getReturnValue() = buf;
    				                    break;
    			                    }
                                    default:
                                    {
                                        // The type is set as something else or has not been set
                                        operation.setReturnValue(data);
                                    }
    		                    }
                            }
                            else if(PyBool_Check(pValue))
                            {
                                loginfo("Bool return value: %d", (pValue == Py_True));
                                bool* data = new bool;
                                *data = (pValue == Py_True);

                                // Check if the return type has already been set (for typed languages)                                
    		                    switch(operation.getReturnType())
    		                    {
    			                    case Operation::BOOL: 
    			                    {
    				                    *(bool*)operation.getReturnValue() = *data;
    				                    break;
    			                    }
    			                    case Operation::SHORT: 
    			                    {
                                        *(short*)operation.getReturnValue() = (short)*data;
    				                    break;
    			                    }
    			                    case Operation::USHORT: 
    			                    {
                                        *(unsigned short*)operation.getReturnValue() = (unsigned short)*data;
    				                    break;
    			                    }
    			                    case Operation::INT: 
    			                    {
                                        *(int*)operation.getReturnValue() = (int)*data;
    				                    break;
    			                    }
    			                    case Operation::UINT: 
    			                    {
                                        *(unsigned int*)operation.getReturnValue() = (unsigned int)*data;
    				                    break;
    			                    }
    			                    case Operation::LONG: 
    			                    {
                                        *(long*)operation.getReturnValue() = (long)*data;
    				                    break;
    			                    }
    			                    case Operation::ULONG: 
    			                    {
                                        *(unsigned long*)operation.getReturnValue() = (unsigned long)*data;
    				                    break;
    			                    }
    			                    case Operation::FLOAT: 
    			                    {
                                        *(float*)operation.getReturnValue() = (float)*data;
    				                    break;
    			                    }
    			                    case Operation::DOUBLE: 
    			                    {
                                        *(double*)operation.getReturnValue() = (double)*data;
    				                    break;
    			                    }
    			                    case Operation::LONGDOUBLE: 
    			                    {
                                        *(long double*)operation.getReturnValue() = (long double)*data;
    				                    break;
    			                    }
    			                    case Operation::CHARS: 
    			                    {
                                        if(*data)
                                        {
                                            *(char**)operation.getReturnValue() = "true";
                                        }
                                        else
                                        {
                                            *(char**)operation.getReturnValue() = "false";
                                        }
    				                    break;
    			                    }
    			                    case Operation::STRING: 
    			                    {
                                        if(*data)
                                        {
                                            *(string*)operation.getReturnValue() = "true";
                                        }
                                        else
                                        {
                                            *(string*)operation.getReturnValue() = "false";
                                        }
    				                    break;
    			                    }
                                    default:
                                    {
                                        // The type is set as something else or has not been set
                                        operation.setReturnValue(data);
                                    }
    		                    }
                            }
                            else if(PyFloat_Check(pValue))
                            {
                                loginfo("Float return value: %f", PyFloat_AsDouble(pValue));

                                double* data = new double;
                                *data = PyFloat_AsDouble(pValue);

                                // Check if the return type has already been set (for typed languages)                                
    		                    switch(operation.getReturnType())
    		                    {
    			                    case Operation::BOOL: 
    			                    {
    				                    *(bool*)operation.getReturnValue() = (*data != 0.0);
    				                    break;
    			                    }
    			                    case Operation::SHORT: 
    			                    {
                                        *(short*)operation.getReturnValue() = (short)*data;
    				                    break;
    			                    }
    			                    case Operation::USHORT: 
    			                    {
                                        *(unsigned short*)operation.getReturnValue() = (unsigned short)*data;
    				                    break;
    			                    }
    			                    case Operation::INT: 
    			                    {
                                        *(int*)operation.getReturnValue() = (int)*data;
    				                    break;
    			                    }
    			                    case Operation::UINT: 
    			                    {
                                        *(unsigned int*)operation.getReturnValue() = (unsigned int)*data;
    				                    break;
    			                    }
    			                    case Operation::LONG: 
    			                    {
                                        *(long*)operation.getReturnValue() = (long)*data;
    				                    break;
    			                    }
    			                    case Operation::ULONG: 
    			                    {
                                        *(unsigned long*)operation.getReturnValue() = (unsigned long)*data;
    				                    break;
    			                    }
    			                    case Operation::FLOAT: 
    			                    {
                                        *(float*)operation.getReturnValue() = (float)*data;
    				                    break;
    			                    }
    			                    case Operation::DOUBLE: 
    			                    {
                                        *(double*)operation.getReturnValue() = (double)*data;
    				                    break;
    			                    }
    			                    case Operation::LONGDOUBLE: 
    			                    {
                                        *(long double*)operation.getReturnValue() = (long double)*data;
    				                    break;
    			                    }
    			                    case Operation::CHARS: 
    			                    {
                                        sprintf(buf, "%f", *data);
                                        *(char**)operation.getReturnValue() = buf;
    				                    break;
    			                    }
    			                    case Operation::STRING: 
    			                    {
                                        sprintf(buf, "%f", *data);
                                        *(string*)operation.getReturnValue() = buf;
    				                    break;
    			                    }
                                    default:
                                    {
                                        // The type is set as something else or has not been set
                                        operation.setReturnValue(data);
                                    }
                                }
                            }
                            else if(PyString_Check(pValue))
                            {
                                loginfo("String return value: %s", PyString_AsString(pValue));
                                const char** data = new const char*; 
                                *data = PyString_AsString(pValue);

                                // Check if the return type has already been set (for typed languages)                                
    		                    switch(operation.getReturnType())
    		                    {
    			                    case Operation::BOOL: 
    			                    {
                                        // If the string is empty or "0" or "false" set to false, otherwise true
                                        if(strlen(*data) == 0 || strcmp(*data, "0") == 0 || strcmp(*data, "false") == 0)
                                        {
    				                        *(bool*)operation.getReturnValue() = false;
                                        }
                                        else
                                        {
                                            *(bool*)operation.getReturnValue() = true;
                                        }
    				                    break;
    			                    }
    			                    case Operation::SHORT: 
    			                    {
                                        *(short*)operation.getReturnValue() = (short)atoi(*data);
    				                    break;
    			                    }
    			                    case Operation::USHORT: 
    			                    {
                                        *(unsigned short*)operation.getReturnValue() = (unsigned short)atoi(*data);
    				                    break;
    			                    }
    			                    case Operation::INT: 
    			                    {
                                        *(int*)operation.getReturnValue() = (int)atoi(*data);
    				                    break;
    			                    }
    			                    case Operation::UINT: 
    			                    {
                                        *(unsigned int*)operation.getReturnValue() = (unsigned int)atoi(*data);
    				                    break;
    			                    }
    			                    case Operation::LONG: 
    			                    {
                                        *(long*)operation.getReturnValue() = (long)atol(*data);
    				                    break;
    			                    }
    			                    case Operation::ULONG: 
    			                    {
                                        *(unsigned long*)operation.getReturnValue() = (unsigned long)atol(*data);
    				                    break;
    			                    }
    			                    case Operation::FLOAT: 
    			                    {
                                        *(float*)operation.getReturnValue() = (float)atof(*data);
    				                    break;
    			                    }
    			                    case Operation::DOUBLE: 
    			                    {
                                        *(double*)operation.getReturnValue() = (double)atof(*data);
    				                    break;
    			                    }
    			                    case Operation::LONGDOUBLE: 
    			                    {
                                        *(long double*)operation.getReturnValue() = (long double)atof(*data);
    				                    break;
    			                    }
    			                    case Operation::CHARS: 
    			                    {
                                        *(const char**)operation.getReturnValue() = *data;
    				                    break;
    			                    }
    			                    case Operation::STRING: 
    			                    {
                                        *(string*)operation.getReturnValue() = *data;
    				                    break;
    			                    }
                                    default:
                                    {
                                        // The type is set as something else or has not been set
                                        string* stringData = new string;
                                        *stringData = *data;
                                        operation.setReturnValue(stringData);
                                    }
                                }
                            }
                            else
                            {
                                PyObject* pIsElement = Py_False;

                                if(elementTreeModule != NULL)
                                {
                                    // Get the xml.etree.ElementTree.iselement function
                                    PyObject* elementTreeIsElementFunc = PyObject_GetAttrString(elementTreeModule, "iselement");

                                    // Call the iselement() function with pValue to check it
                                    pIsElement = PyObject_CallFunction(elementTreeIsElementFunc, "O", pValue);
                                    Py_DECREF(elementTreeIsElementFunc);
                                }

                                if(PyObject_IsTrue(pIsElement) == 1)
                                {
                                    // pValue is an xml.etree.ElementTree.Element - convert to SDO
                                    PyObject* elementTreeToStringFunc = PyObject_GetAttrString(elementTreeModule, "tostring");
                                    PyObject* pElemString = PyObject_CallFunction(elementTreeToStringFunc, "O", pValue);
                                    char* data = PyString_AsString(pElemString);

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
                                        operation.setReturnValue(dataObjectData);
                                    }
                                    else
                                    {
                                        string msg = "xml.etree.ElementTree.Element could not be converted to a DataObject";
                                        throwException(ServiceDataException, msg.c_str());
                                    }

                                    Py_DECREF(elementTreeToStringFunc);
                                    Py_DECREF(pElemString);
                                }
                                else
                                {
                                    PyObject* valueRepr = PyObject_Repr(pValue); 
                                    PyObject* valueType = PyObject_Type(pValue);             
                                    PyObject* valueTypeRepr = PyObject_Repr(valueType);    
                                    loginfo("Return value of unknown type (%s) has repr: %s", PyString_AsString(valueTypeRepr), PyString_AsString(valueRepr));
                                    Py_DECREF(valueTypeRepr);
                                    Py_DECREF(valueType);
                                    Py_DECREF(valueRepr);
                                }

                                Py_DECREF(pIsElement);                                
                            }

                            Py_DECREF(elementTreeModule);
                            Py_DECREF(elementTreeModuleName);
                            Py_DECREF(pValue);
                        }
                        else 
                        {
                            Py_DECREF(pFunc);
                            Py_XDECREF(elementTreeModule);
                            Py_XDECREF(elementTreeModuleName);

                            string msg = "Error whilst calling Python function "+operation.getName()+": ";
                            if(PyErr_Occurred())
                            {
                                PyObject *pErrorType, *pErrorValue, *pErrorTraceback;
                                PyErr_Fetch(&pErrorType, &pErrorValue, &pErrorTraceback);
 
                                if (pErrorType != NULL && pErrorValue != NULL)
                                {
                                    PyObject* pErrorTypeStr = PyObject_Str(pErrorType);    
                                    PyObject* pErrorValueStr = PyObject_Str(pErrorValue);    
                                    msg += PyString_AsString(pErrorTypeStr);
                                    msg += " : ";
                                    msg += PyString_AsString(pErrorValueStr);
                                    Py_DECREF(pErrorTypeStr);
                                    Py_DECREF(pErrorValueStr);                                    
                                }
                                else
                                {
                                    msg += "No Python Error information provided";                                    
                                }
                                Py_XDECREF(pErrorType);
                                Py_XDECREF(pErrorValue);         
                                Py_XDECREF(pErrorTraceback);    

                                PyErr_Print();
                            }
                            
                            throwException(ServiceInvocationException, msg.c_str());
                        }
                    }
                    else 
                    {
                        if (PyErr_Occurred())
                        {
                            PyErr_Print();
                        }
                        string msg = "Cannot find the operation named " + operation.getName() + " in the Python module";
                        throwException(ServiceInvocationException, msg.c_str());
                    }
                    Py_XDECREF(pFunc);
 
                }
                catch (...)
                {
                    releaseInstance();
                    runtime->unsetCurrentComponent();
                    throw;
                }
                releaseInstance();
                runtime->unsetCurrentComponent();
            }

 
            // ==========================================================================
            // Add any properties into the loaded implementation module as Python objects
            // ==========================================================================
            void PythonServiceWrapper::addProperties(PyObject* module)
            {
                logentry();

                // Set all the configured properties
                DataObjectPtr properties = component->getProperties();
                PropertyList pl = properties->getInstanceProperties();

                for (unsigned int i = 0; i < pl.size(); i++)
                {
                    if (properties->isSet(pl[i]))
                    {
                        string propName = pl[i].getName();
                        string propValue = properties->getCString(pl[i]);
                        PyObject* property;

                        if(pl[i].isMany())
                        {
                            //TODO - deal with properties that are many
                        }
                        
                        switch(pl[i].getTypeEnum())
                        {
                        case Type::BooleanType:
                            {
                                if(properties->getBoolean(pl[i]))
                                {
                                    property = Py_True;
                                }
                                else
                                {
                                    property = Py_False;
                                }
                                Py_INCREF(property);
                                break;
                            }
                        case Type::BigIntegerType:
                        case Type::BigDecimalType:
                        case Type::LongType:
                            {
                                property = PyLong_FromLongLong(properties->getLong(pl[i]));
                                break;
                            }
                        case Type::ShortType:
                        case Type::IntegerType:
                            {
                                property = PyInt_FromLong(properties->getInteger(pl[i]));
                                break;
                            }
                        case Type::DoubleType:
                        case Type::FloatType:
                            {
                                property = PyFloat_FromDouble(properties->getDouble(pl[i]));
                                break;
                            }
                        case Type::DataObjectType:
                        case Type::OpenDataObjectType:
                            {
                                // Serialize a DataObject and create a python string object from the XML
                                DataObjectPtr data = properties->getDataObject(pl[i]);                                
                                XMLHelperPtr helper = HelperProvider::getXMLHelper(properties->getDataFactory());
                                string serializedData = helper->save(data, 
                                        data->getType().getURI(),
                                        data->getType().getName());

                                loginfo("Converting SDO DataObject to Python ElementTree: %s", serializedData.c_str());

                                // Get the xml.etree.ElementTree.XML function
                                PyObject* elementTreeModuleName = PyString_FromString("xml.etree.ElementTree"); 
                                PyObject* elementTreeModule = PyImport_Import(elementTreeModuleName);                        
                                PyObject* elementTreeXMLFunc = PyObject_GetAttrString(elementTreeModule, "XML");

                                // Call the XML() function with the XML string 
                                property = PyObject_CallFunction(elementTreeXMLFunc, "s", serializedData.c_str());

                                Py_DECREF(elementTreeXMLFunc);
                                Py_DECREF(elementTreeModule);
                                Py_DECREF(elementTreeModuleName);
                                break;
                            }
                        case Type::CharacterType:
                        case Type::StringType:
                        case Type::TextType:
                        case Type::UriType:
                        default:
                            {
                                // For strings and by default create a python string object 
                                property = PyString_FromString(propValue.c_str());
                                break;
                            }
                        }
                                                                          
                        int success = PyModule_AddObject(module, (char*)propName.c_str(), property);

                        if(success == 0)
                        {
                            loginfo("Added property named %s with type %s and value %s to python module", propName.c_str(), pl[i].getType().getName(), propValue.c_str());
                        }
                        else
                        {
                            logwarning("Failed to add property named %s to python module", propName.c_str());
                        }
                    }
                }
            }


            // ======================================================================
            // Add any references into the loaded implementation module as class instances that look like
            // the classes defined in the interface.python xml
            // ======================================================================
            void PythonServiceWrapper::addReferences(PyObject* module)
            {
                logentry();

                // Import the TuscanySCA python-extension module
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
                    logwarning(msg.c_str());
                }
                else
                {
                    // Get the sca_proxy class
                    PyObject* sca_proxy_class = PyObject_GetAttrString(sca_proxy_module, "sca_proxy_class");

                    // Iterate through the references of the current component, adding
                    // each reference to the module
                    Component::REFERENCE_MAP references = component->getReferences();
                    Component::REFERENCE_MAP::iterator pos;
                    for( pos = references.begin(); pos != references.end(); ++pos)
                    {
                        ReferenceType* referenceType = ((Reference*) pos->second)->getType();
                        string referenceName = referenceType->getName();

                        PyObject* tuscanySCAArgs = PyTuple_New(2);
                        PyObject* refName = PyString_FromString(referenceType->getName().c_str());
                        PyTuple_SetItem(tuscanySCAArgs, 0, refName);
                        Py_INCREF(Py_True);
                        PyTuple_SetItem(tuscanySCAArgs, 1, Py_True);

                        // Create the instance of the TuscanySCAReference class
                        PyObject* sca_proxy_classInstance = PyInstance_New(sca_proxy_class, tuscanySCAArgs, NULL);
                        Py_DECREF(tuscanySCAArgs);

                        int success = PyModule_AddObject(module, (char*)referenceName.c_str(), sca_proxy_classInstance);

                        if(success == 0)
                        {
                            loginfo("Successfully added sca_proxy_class instance as %s to pythonModule", referenceName.c_str());
                        }
                        else
                        {
                            logwarning("Failed to add sca_proxy_class instance as %s to pythonModule", referenceName.c_str());
                        }
                    }                       
                    Py_DECREF(sca_proxy_module);
                }
            }
        } // End namespace python        
    } // End namespace sca
} // End namespace tuscany
