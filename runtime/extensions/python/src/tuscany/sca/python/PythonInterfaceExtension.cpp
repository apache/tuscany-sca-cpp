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

#include "tuscany/sca/python/PythonInterfaceExtension.h"
#include "tuscany/sca/python/model/PythonInterface.h"
#include "tuscany/sca/util/Logging.h"

//#include "commonj/sdo/SDO.h"
//using commonj::sdo::DataObjectList; 

namespace tuscany
{
    namespace sca
    {
        namespace python
        {
            // ===================================================================
            // Constructor for the PythonInterfaceExtension class. 
            // ===================================================================
            PythonInterfaceExtension::PythonInterfaceExtension()
            { 
                LOGENTRY(1, "PythonInterfaceExtension::constructor");     
                LOGEXIT(1, "PythonInterfaceExtension::constructor");
            }
            
            // ===================================================================
            // Destructor for the PythonInterfaceExtension class.
            // ===================================================================
            PythonInterfaceExtension::~PythonInterfaceExtension()
            { 
                LOGENTRY(1, "PythonInterfaceExtension::destructor");;           
                LOGEXIT(1, "PythonInterfaceExtension::destructor");
            }

            const string PythonInterfaceExtension::extensionName("python");
            const string PythonInterfaceExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#PythonInterface");

            // ===================================================================
            // loadModelElement - load the info from interface.python 
            // ===================================================================
            tuscany::sca::model::Interface* PythonInterfaceExtension::getInterface(Composite* composite, DataObjectPtr scdlInterface)
            {
                // Determine the type
                string ifType = scdlInterface->getType().getName();
                if (ifType == "PythonInterface")
                {
                    bool remotable = scdlInterface->getBoolean("remotable");                    
                    bool conversational = scdlInterface->getBoolean("conversational");                    

                    return new PythonInterface(remotable, conversational);
                    //DataObjectList& operationList = scdlInterface->getList("operation");

                    //for(int i=0; i<operationList.size(); i++)
                    //{
                    //    string opName = operationList[i]->getCString("name");

                    //    if(operationList[i]->hasProperty("returnType"))
                    //    {
                    //        string returnType = operationList[i]->getCString("returnType");

                    //        if(returnType == "string")
                    //        {
                    //            pythonInterface->addOperation(opName, PythonInterface::STRING);
                    //        }
                    //        else if(returnType == "int")
                    //        {
                    //            pythonInterface->addOperation(opName, PythonInterface::INT);
                    //        }
                    //        else if(returnType == "long")
                    //        {
                    //            pythonInterface->addOperation(opName, PythonInterface::LONG);
                    //        }
                    //        else if(returnType == "boolean")
                    //        {
                    //            pythonInterface->addOperation(opName, PythonInterface::BOOLEAN);
                    //        }
                    //        else if(returnType == "float")
                    //        {
                    //            pythonInterface->addOperation(opName, PythonInterface::FLOAT);
                    //        }
                    //        else if(returnType == "other")
                    //        {
                    //            pythonInterface->addOperation(opName, PythonInterface::OTHER);
                    //        }
                    //        else
                    //        {
                    //            pythonInterface->addOperation(opName, PythonInterface::NONE);
                    //        }
                    //    }
                    //    else
                    //    {
                    //        // No return Type provided - set as NONE
                    //        pythonInterface->addOperation(opName, PythonInterface::NONE);
                    //    }
                    //}
                    //
                    //return pythonInterface;
                }
                return 0;
            }

        } // End namespace python
    } // End namespace sca
} // End namespace tuscany
