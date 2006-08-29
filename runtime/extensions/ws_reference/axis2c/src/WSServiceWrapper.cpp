/*
 *
 *  Copyright 2005 The Apache Software Foundation or its licensors, as applicable.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/* $Rev$ $Date$ */
#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4091)
#endif

#include "osoa/sca/sca.h"
#include "tuscany/sca/util/Logging.h"
#include "Axis2Client.h"
#include "WSServiceWrapper.h"
#include "tuscany/sca/core/Operation.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/ServiceType.h"

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
            }
            
            WSServiceWrapper::~WSServiceWrapper()
            {
            }
            
            ///
            /// This method will be called when a web service call needs to be made.
            ///
            void WSServiceWrapper::invoke(Operation& operation) 
            {
                LOGENTRY(1, "WSServiceWrapper::invoke");
            
                const string& operationName = operation.getName();
            
                LOGINFO_2(2,"Service: %s, Operation: %s", getService()->getType()->getName().c_str() , operationName.c_str());
            
                for (unsigned int i=0; i<operation.getNParms(); i++)
                {
                    LOGINFO_2(2, "Parameter %p, Type %u", operation.getParameterValue(i),(int) operation.getParameterType(i));
                }

                // Create the Axis2 client that will handle the Web Service invocation
                Service* service = getService();
                CompositeReference* compositeReference = (CompositeReference*)service->getComponent();
                
                Axis2Client client(compositeReference);
                client.invoke(operation);
                
                LOGEXIT(1, "WSServiceWrapper::invoke");
            }

        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany
    


