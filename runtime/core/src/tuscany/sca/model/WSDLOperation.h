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

#ifndef tuscany_sca_model_wsdloperation_h
#define tuscany_sca_model_wsdloperation_h

#include <string>

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/export.h"


namespace tuscany
{
    namespace sca
    {
        namespace model
        {

            /**
             * Represents a single.,bound WSDL defined operation.
             * This class includes information from the soapBinding
             * in addition to the WSDL definition of the operation.
             */ 
            class WSDLOperation
            {
            public:
                /**
                 * Constructor.
                 * @param operation The name of the operation.
                 * @param soapAction The soapAction associated with this operation
                 * in the SOAP binding of the operation.
                 * @param endpoint The endpoint address of the operation.
                 * @param responseName The name of the response message.
                 */
                 SCA_API WSDLOperation();
                 
                /**
                 * Destructor.
                 */ 
                SCA_API virtual ~WSDLOperation();

                /**
                 * Return the name of the operation for use when serializing an
                 * outgoing message.
                 * @return The name of the element in the request message.
                 */ 
                SCA_API const std::string& getOperationName() const {return operationName;}
                SCA_API void setOperationName(const std::string& opName) {operationName = opName;}

                /**
                 * The soap action string for this operation.
                 * @return The soap action.
                 */
                SCA_API const std::string& getSoapAction() const {return soapAction;}
                SCA_API void setSoapAction(const std::string& soapAct) {soapAction = soapAct;}

                /**
                 * Return the endpoint address for the target web service.
                 * @return The endpoint address.
                 */
                SCA_API const std::string& getEndpoint() const {return endpoint;}
                SCA_API void setEndpoint(const std::string& ep) {endpoint = ep;}

                enum soapVersion
                {
                	SOAP11,
                	SOAP12
                };
                
                SCA_API void setSoapVersion(soapVersion ver) {soapVer = ver;}
                SCA_API soapVersion getSoapVersion() const {return soapVer;}
                
                SCA_API void setDocumentStyle(bool docStyle) {documentStyle = docStyle;}
                SCA_API bool isDocumentStyle() const {return documentStyle;}

                SCA_API void setWrappedStyle(bool wrapStyle) {wrappedStyle = wrapStyle;}
                SCA_API bool isWrappedStyle() const {return wrappedStyle;}

                SCA_API void setEncoded(bool enc) {encoded = enc;}
                SCA_API bool isEncoded() const {return encoded;}

                
                SCA_API void setInputType(const std::string& inputType);
                SCA_API const std::string& getInputTypeUri() const {return inputTypeUri;}
                SCA_API const std::string& getInputTypeName() const {return inputTypeName;}
                SCA_API void setOutputType(const std::string& outputType);
                SCA_API const std::string& getOutputTypeUri() const {return outputTypeUri;}
                SCA_API const std::string& getOutputTypeName() const {return outputTypeName;}
                
            private:
                /**
                 * The name of the operation for use when serializing an
                 * outgoing message.
                 */ 
                std::string operationName;

                 /**
                 * The soap action string for this operation.
                 */
                std::string soapAction;

                /**
                 * The endpoint address of the target web service.
                 */
                std::string endpoint;

				bool documentStyle;
                bool wrappedStyle;
				bool encoded;
				soapVersion soapVer;
				
				std::string inputTypeUri;
				std::string inputTypeName;
				
				std::string outputTypeUri;
				std::string outputTypeName;
				
                commonj::sdo::DataObjectPtr inputMessage;
                commonj::sdo::DataObjectPtr outputMessage;
            };

         } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_wsdloperation_h

