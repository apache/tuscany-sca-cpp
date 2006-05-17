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

#include "Axis2Client.h"
#include "tuscany/sca/ws/WSServiceWrapper.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;

#include "tuscany/sca/util/Logging.h"
using namespace tuscany::sca;
using namespace tuscany::sca::ws;




WSServiceWrapper::WSServiceWrapper(tuscany::sca::model::WireTarget* target) : ExternalServiceWrapper(target)
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

    LOGINFO_2(2,"External service: %s, Operation: %s", getTarget()->getName().c_str() , operationName.c_str());

	for (unsigned int i=0; i<operation.getNParms(); i++)
	{
		LOGINFO_2(2, "Parameter %p, Type %u", operation.getParameterValue(i),(int) operation.getParameterType(i));
	}
    
    // From the external service ws-binding, get the namespace of the wsdl endpoint
    ExternalService* externalService = getExternalService();
    
	Axis2Client client(externalService);
	client.invoke(operation);
	
    LOGEXIT(1, "WSServiceWrapper::invoke");
}

