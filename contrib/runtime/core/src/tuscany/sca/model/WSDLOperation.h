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

#include <map>
#include <list>
#include <string>

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/export.h"
#include "tuscany/sca/model/WSDLMessagePart.h"


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

                SCA_API void setInputEncoded(bool enc) {inputEncoded = enc;}
                SCA_API bool isInputEncoded() const {return inputEncoded;}

                SCA_API void setOutputEncoded(bool enc) {outputEncoded = enc;}
                SCA_API bool isOutputEncoded() const {return outputEncoded;}
                
                /**
                 * Input Message URI and Name. To get the message part URI and name, you
                 * will have to get the WSDLMessagePart by calling getInputMessagePart()
                 */
                SCA_API void setInputMessageType(const std::string& inputMessageType);
                SCA_API const std::string& getInputMessageUri() const {return inputMessageUri;}
                SCA_API const std::string& getInputMessageName() const {return inputMessageName;}

                /**
                 * Output Message URI and Name. To get the message part URI and name, you
                 * will have to get the WSDLMessagePart by calling getOutputMessagePart()
                 */
                SCA_API void setOutputMessageType(const std::string& outputMessageType);
                SCA_API const std::string& getOutputMessageUri() const {return outputMessageUri;}
                SCA_API const std::string& getOutputMessageName() const {return outputMessageName;}

                // TODO We should add an enum for REQUEST_ONLY, RESPONSE_ONLY, REQUEST_RESPONSE
                //      and a setter/getter. The enum would be checked by getOutputXXX

                /**
                 *  Parse a WSDL message represented by an SDO into the input/outputPartMap
                 *  @param inputMsg a data object which contains the WSDL message
                 *         ie. <wsdl:message name="getAccountReportResponse">
                 *                 <wsdl:part element="tns:getAccountReportResponse"
                 *                            name="getAccountReportResponse" />
                 *             </wsdl:message>
                 */
                SCA_API void setInputMessage( commonj::sdo::DataObjectPtr inputMsg );
                SCA_API void setOutputMessage( commonj::sdo::DataObjectPtr outputMsg );

                /**
                 * Manually set the input/outputPartMap, as opposed to passing in an SDO WSDL message.
                 * Populates the input/outputPartMap, setInput/OutputMessageType must still be called.
                 * @param partName the name of the message part
                 * @param part the message part
                 */
                SCA_API void setInputMessagePart( const std::string &partName, WSDLMessagePart part );
                SCA_API void setOutputMessagePart( const std::string &partName, WSDLMessagePart part );

                /**
                 * Return a message part keyed off of the message part name
                 * @param name the message part name
                 * @return the message part in a WSDLMessagePart object
                 */
                SCA_API const WSDLMessagePart &getInputMessagePart( const std::string &name ) const;
                SCA_API const WSDLMessagePart &getOutputMessagePart( const std::string &name ) const;

                /**
                 * Get all of the message part names
                 * @return a list of strings, each being a different message part name
                 */
                SCA_API const std::list<std::string> getInputMessagePartNames() const;
                SCA_API const std::list<std::string> getOutputMessagePartNames() const;

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
                bool inputEncoded;
                bool outputEncoded;
                soapVersion soapVer;

                std::string inputMessageUri;
                std::string inputMessageName;

                std::string outputMessageUri;
                std::string outputMessageName;

                typedef std::map<std::string, WSDLMessagePart> PART_MAP;
                PART_MAP inputPartMap;
                PART_MAP outputPartMap;
            };

         } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_wsdloperation_h

