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

#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4091)
#endif

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/util/Logging.h"
#include "Axis2Client.h"
#include "WSServiceWrapper.h"
#include "tuscany/sca/core/Operation.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/ServiceType.h"

using namespace std;
using namespace commonj::sdo;
using namespace tuscany::sca;
using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        namespace ws
        {
            
            WSServiceWrapper::WSServiceWrapper(Service* service) : ServiceWrapper(service)
            {
                logentry();

                // Define the SOAP Body type and element to allow a SOAP body to
                // be loaded in a DataObject
                DataFactoryPtr dataFactory = service->getComponent()->getComposite()->getDataFactory();
                try {
                    const Type& bodyType = dataFactory->getType("http://www.w3.org/2003/05/soap-envelope", "Body");
                } catch (SDORuntimeException&)
                {
                    dataFactory->addType("http://www.w3.org/2003/05/soap-envelope", "RootType", false, false, false);                
                    dataFactory->addType("http://www.w3.org/2003/05/soap-envelope", "Body", false, true, false);                
                    dataFactory->addPropertyToType(
                        "http://www.w3.org/2003/05/soap-envelope", "RootType",
                        "Body",
                        "http://www.w3.org/2003/05/soap-envelope", "Body",
                        false, false, true);
    
                    dataFactory->addType("http://schemas.xmlsoap.org/soap/envelope/", "RootType", false, false, false);
                    dataFactory->addType("http://schemas.xmlsoap.org/soap/envelope/", "Body", false, true, false);
                    dataFactory->addPropertyToType(
                        "http://schemas.xmlsoap.org/soap/envelope/", "RootType",
                        "Body",
                        "http://schemas.xmlsoap.org/soap/envelope/", "Body",
                        false, false, true);
                }

                try {
                    const Type& tempType = dataFactory->getType("http://tempuri.org", "RootType");
                } catch (SDORuntimeException&)
                {
                    dataFactory->addType("http://tempuri.org", "RootType", false, false, false);                
                    dataFactory->addType("http://tempuri.org", "Wrapper", false, true, false);                
                    dataFactory->addPropertyToType(
                        "http://tempuri.org", "RootType",
                        "Wrapper",
                        "http://tempuri.org", "Wrapper",
                        false, false, true);
                    dataFactory->addType("http://tempuri.org", "Part", false, true, false);                
                    dataFactory->addPropertyToType(
                        "http://tempuri.org", "RootType",
                        "Part",
                        "http://tempuri.org", "Part",
                        false, false, true);
                }
            }
            
            WSServiceWrapper::~WSServiceWrapper()
            {
                logentry();
            }
            
            ///
            /// This method will be called when a web service call needs to be made.
            ///
            void WSServiceWrapper::invoke(Operation& operation) 
            {
                logentry();
            
                const string& operationName = operation.getName();
            
                loginfo("Service: %s, operation: %s", getService()->getType()->getName().c_str() , operationName.c_str());
            
                for (unsigned int i=0; i<operation.getNParms(); i++)
                {
                    loginfo("Parameter: %p, type: %u", operation.getParameterValue(i),(int) operation.getParameterType(i));
                }

                // Create the Axis2 client that will handle the Web Service invocation
                Service* service = getService();
                CompositeReference* compositeReference = (CompositeReference*)service->getComponent();
                
                Axis2Client client(compositeReference);
                client.invoke(operation);
            }

        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany
    


