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

#ifndef tuscany_sca_model_wsdlmessagepart_h
#define tuscany_sca_model_wsdlmessagepart_h

#include <map>
#include <list>
#include <string>

#include "tuscany/sca/export.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {

            /**
             * Represents a single WSDL defined message part.
             */ 
            class WSDLMessagePart
            {
            public:
                 /**
                  * Default Constructor
                  */
                 SCA_API WSDLMessagePart();

                /**
                 * Constructor.
                 * @param partName The name of the message part.
                 * @param partType The type of this message part.
                 * @param partUri  The namespace URI for the message part type.
                 */
                 SCA_API WSDLMessagePart( std::string partName,
                                          std::string partType,
                                          std::string partUri );
                 
                /**
                 * Destructor.
                 */ 
                SCA_API ~WSDLMessagePart();

                /**
                 * Return the name of the WSDL Message part
                 * @return The name of the WSDL Message part
                 */ 
                SCA_API const std::string& getPartName() const {return partName_;}
                SCA_API void setPartName(const std::string& partName) {partName_ = partName;}

                /**
                 * The type of this WSDL Message part
                 * @return The type of this WSDL Message part
                 */
                SCA_API const std::string& getPartType() const {return partType_;}
                SCA_API void setPartType(const std::string& partType) {partType_ = partType;}

                /**
                 * Set the WSDL message part type and namespace URI
                 * @return A prefixed namespace and type of the form prefix:type
                 */
                SCA_API void setMessagePartType(const std::string& partType);

                /**
                 * Return the namespace URI of the WSDL Message part
                 * @return The namespace URI of the WSDL Message part
                 */
                SCA_API const std::string& getPartUri() const {return partUri_;}
                SCA_API void setPartUri(const std::string& partUri) {partUri_ = partUri;}

            private:
                /**
                 * The name of this message part
                 */ 
                std::string partName_;

                 /**
                 * The type of this message part
                 */
                std::string partType_;

                /**
                 * The namespace URI of the message part type
                 */
                std::string partUri_;
            };

         } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_wsdlmessagepart_h

