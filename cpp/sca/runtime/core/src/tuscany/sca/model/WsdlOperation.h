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

#ifndef tuscany_sca_model_wsdlOperation_h
#define tuscany_sca_model_wsdlOperation_h

#include <string>
using std::string;


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
            class WsdlOperation
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
                 WsdlOperation(const string& operation,
                    const string& soapAction, 
                    const string& endpoint,
                    const string& responseName);

                /**
                 * Destructor.
                 */ 
                ~WsdlOperation();

                /**
                 * Return the name of the operation for use when serializing an
                 * outgoing message.
                 * @return The name of the element in the request message.
                 */ 
                const string& getOperationName() {return operationName;};

                /**
                 * Return the name of the response for use when deserializing an
                 * incoming message.
                 * @return The name of the element in the response message.
                 */
                const string& getResponseName() {return responseName;};

                /**
                 * The soap action string for this operation.
                 * @return The soap action.
                 */
                const string& getSoapAction() {return soapAction;};

                /**
                 * Return the endpoint address for the target web service.
                 * @return The endpoint address.
                 */
                const string& getEndpoint() {return endpoint;};


                
            private:
                /**
                 * The name of the operation for use when serializing an
                 * outgoing message.
                 */ 
                string operationName;

                /**
                 * The name of the response for use when deserializing an
                 * incoming message.
                 */
                string responseName;

                /**
                 * The soap action string for this operation.
                 */
                string soapAction;

                /**
                 * The endpoint address of the target web service.
                 */
                string endpoint;
            };

         } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_wsdlOperation_h

