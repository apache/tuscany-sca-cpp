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

/* $Rev: 414501 $ $Date: 2006-06-15 00:45:29 -0700 (Thu, 15 Jun 2006) $ */
#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4091)
#endif

#include "tuscany/sca/ws/WSReferenceWrapper.h"

#include "Axis2Client.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;

#include "tuscany/sca/util/Logging.h"
using namespace tuscany::sca;
using namespace tuscany::sca::ws;




WSReferenceWrapper::WSReferenceWrapper(tuscany::sca::model::WireTarget* target) : CompositeReferenceWrapper(target)
{
    
}

WSReferenceWrapper::~WSReferenceWrapper()
{
    
}

///
/// This method will be called when a web service call needs to be made.
///
void WSReferenceWrapper::invoke(Operation& operation) 
{
    LOGENTRY(1, "WSReferenceWrapper::invoke");

    const string& operationName = operation.getName();

    LOGINFO_2(2,"External service: %s, Operation: %s", getTarget()->getName().c_str() , operationName.c_str());

	for (unsigned int i=0; i<operation.getNParms(); i++)
	{
		LOGINFO_2(2, "Parameter %p, Type %u", operation.getParameterValue(i),(int) operation.getParameterType(i));
	}
    
    // From the external service ws-binding, get the namespace of the wsdl endpoint
    CompositeReferenceType* externalService = getCompositeReferenceType();
    
	Axis2Client client(externalService);
	client.invoke(operation);
	
    LOGEXIT(1, "WSReferenceWrapper::invoke");
}

