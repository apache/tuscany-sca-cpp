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

#ifndef tuscany_sca_model_wsdloperation_h
#define tuscany_sca_model_wsdloperation_h
#include "osoa/sca/export.h"
#include <string>
using std::string;


#include "commonj/sdo/SDO.h"
using namespace commonj::sdo;

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
                 WSDLOperation();
                 
                /**
                 * Destructor.
                 */ 
                ~WSDLOperation();

                /**
                 * Return the name of the operation for use when serializing an
                 * outgoing message.
                 * @return The name of the element in the request message.
                 */ 
                const string& getOperationName() const {return operationName;}
                void setOperationName(const string& opName) {operationName = opName;}

                /**
                 * The soap action string for this operation.
                 * @return The soap action.
                 */
                const string& getSoapAction() const {return soapAction;}
                void setSoapAction(const string& soapAct) {soapAction = soapAct;}

                /**
                 * Return the endpoint address for the target web service.
                 * @return The endpoint address.
                 */
                const string& getEndpoint() const {return endpoint;}
                void setEndpoint(const string& ep) {endpoint = ep;}

                enum soapVersion
                {
                	SOAP11,
                	SOAP12
                };
                
                void setSoapVersion(soapVersion ver) {soapVer = ver;}
                soapVersion getSoapVersion() const {return soapVer;}
                
                void setDocumentStyle(bool docStyle) {documentStyle = docStyle;}
                bool isDocumentStyle() const {return documentStyle;}

                void setEncoded(bool enc) {encoded = enc;}
                bool isEncoded() const {return encoded;}

                
                void setInputType(const string& inputType);
                const string& getInputTypeUri() const {return inputTypeUri;}
                const string& getInputTypeName() const {return inputTypeName;}
                void setOutputType(const string& outputType);
                const string& getOutputTypeUri() const {return outputTypeUri;}
                const string& getOutputTypeName() const {return outputTypeName;}
                
            private:
                /**
                 * The name of the operation for use when serializing an
                 * outgoing message.
                 */ 
                string operationName;

                 /**
                 * The soap action string for this operation.
                 */
                string soapAction;

                /**
                 * The endpoint address of the target web service.
                 */
                string endpoint;

				bool documentStyle;
				bool encoded;
				soapVersion soapVer;
				
				string inputTypeUri;
				string inputTypeName;
				
				string outputTypeUri;
				string outputTypeName;
				
                DataObjectPtr inputMessage;
                DataObjectPtr outputMessage;
            };

         } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_wsdloperation_h

