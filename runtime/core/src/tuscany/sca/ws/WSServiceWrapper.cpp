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

/* $Rev$ $Date: 2005/12/22 11:33:21 $ */

#include "tuscany/sca/ws/WSServiceWrapper.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/model/WSBinding.h"
#include "tuscany/sca/model/Wsdl.h"
#include "tuscany/sca/ws/SDOStub.h"
#include "tuscany/sca/util/Utils.h"
using namespace tuscany::sca;
using namespace tuscany::sca::ws;



extern "C" 
{
    ///
    /// Factory function for creating service wrappers for web services.
    ///
    SCA_API WSServiceWrapper* WSServiceWrapper_Factory(tuscany::sca::model::WireTarget* target)
    {
        return new WSServiceWrapper(target);
    }
}


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


    // Check one parameter in/ one parameter out and that they are data objects.

    // Get the data factory for the module (it will already have the types loaded for the xsds)

    // From the external service ws-binding, get the namespace of the wsdl endpoint
    ExternalService* externalService = getExternalService();
    WSBinding* binding = (WSBinding *)externalService->getBinding();

    string portNamespace = binding->getWSDLNamespaceURL();

    // Lookup the wsdl model from the module, keyed on the namespace 
    // (the wsdl will have been loaded at startup, based on the information in the config file)
    Wsdl* wsdlDefinition = externalService->getContainingModule()->findWsdl(portNamespace);

    // Match the operation in Operation to the operation in the wsdl port type.
    WsdlOperation wsdlOperation =  wsdlDefinition->findOperation(
                                                binding->getServiceName()
                                                , binding->getPortName()
                                                , operationName);

    // Get the name of the response by navigating  from the wsdl operation to the return type

    // Get the name of the SoapAction by navigating from the wsdl.

    // Create an SDOStub
    try
    {
        SDOStub wsStub(wsdlOperation.getEndpoint().c_str());
        wsStub.setTargetNamespace(portNamespace.c_str());
        wsStub.setOperationName(wsdlOperation.getOperationName().c_str());
        wsStub.setOperationResponseName(wsdlOperation.getResponseName().c_str());
        wsStub.setSoapAction(wsdlOperation.getSoapAction().c_str());
        
    
        // Call the service via the SDOStub
        *(DataObjectPtr *)operation.getReturnValue() = wsStub.invoke(*((DataObjectPtr*)operation.getParameter(0)),
            externalService->getContainingModule()->getDataFactory());

    }
    catch (AxisException& e)
    {
        throw ServiceRuntimeException("AxisException", ServiceRuntimeException::Error, e.what());
    }
    

    LOGEXIT(1, "WSServiceWrapper::invoke");
    
}
