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

#if !defined(SDOSTUB_H_INCLUDED)
#define SDOSTUB_H_INCLUDED

#include "osoa/sca/export.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "commonj/sdo/SDO.h"

// For stub code
#include <axis/client/Stub.hpp>
#include <axis/OtherFaultException.hpp>
#include <axis/ISoapFault.hpp>

#include <string>

AXIS_CPP_NAMESPACE_USE
using namespace commonj::sdo;

namespace tuscany
{
    namespace sca
    {
        namespace ws
        {

           /**
            * The SDOStub class is an Axis Stub that is used to invoke operations on a Web Service
            * using SDO objects as input parameters and return values.
            */
            class SDOStub : public Stub
            {
               public:

                   /** Construct an SDOStub using the default HTTP 1.1 transport.
                    */
                   SCA_API SDOStub();

                   /** Construct an SDOStub using the default HTTP 1.1 transport 
                    *  and set the web service endpoint URI.
                    *  @param endpointURI - web service endpoint URI
                    */
                   SCA_API SDOStub(const char* endpointURI);

                   /** Destruct the SDOStub
                    */
                   SCA_API virtual ~SDOStub();

                   /** Set the web service endpoint URI.
                    *  The endpoint is the value of the soap:address element in the WSDL.
                    *  @param endpoint - web service endpoint URI.
                    */
                   SCA_API void setEndpoint(const char* endpoint);

                   /** Get the web service endpoint URI. 
                    *  @return web service endpoint URI.
                    */
                   SCA_API const char* getEndpoint();

                   /** Set the target namespace for elements and operations.
                    *  @param targetNamespace - the target namespace.
                     */
                   SCA_API void setTargetNamespace(const char* targetNamespace);

                   /** Get the target namespace.
                    *  @return - target namespace.
                    */
                   SCA_API const char* getTargetNamesapce();

                   /** Set the name of the operation to invoke. 
                    *  @param operationName - operation name
                    */
                   SCA_API void setOperationName(const char* operationName);

                   /** Get the operation name.
                    *  @return operation name.
                    */
                   SCA_API const char* getOperationName();

                   /** Set the name of the element that is returned as the result of a soap method.
                    *  @param operationResponseName - name of the response element.
                    */
                   SCA_API void setOperationResponseName(const char* operationResponseName);

                   /** Get the name of the element that is returned as the result of a soap method.
                    *  @return operation response name.
                    */
                   SCA_API const char* getOperationResponseName();

                   /** Set the SOAPAction header.
                    *  @param soapAction - the soap action.
                    */
                   SCA_API void setSoapAction(const char* soapAction);

                   /** Get the SoapAction header.
                    *  @return soap action.
                    */
                   SCA_API const char* getSoapAction();

                   /** Invoke the operation on the web service.
                    *  @param requestDO - SDO containing the input parameters for the operation.
                    *  @param dataFactory - DataFactory for de/serialzing the SDOs.
                    *  @return DataObjectPtr - SDO containg the response from the web service.
                    */
                   SCA_API DataObjectPtr invoke(DataObjectPtr requestDO, DataFactoryPtr dataFactory);

               private:
                   // Deserialize the soap response from the web service.
                    string getSoapResponse();

                    string m_strEndpoint;
                    string m_strTargetNamespace;
                    string m_strOperationName;
                    string m_strOperationResponseName;
                    string m_strSoapAction;
            };

        } // end namespace ws
    } // end namespace sca
} // end namespace tuscany


#endif // !defined(SDOSTUB_H_INCLUDED)
