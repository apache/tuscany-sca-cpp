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

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/model/Wsdl.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            
            
            // Constructor
            WsdlOperation::WsdlOperation(
                const string& operationNam, 
                const string& soapAct, 
                const string& endpointAddress,
                DataObjectPtr inMsg,
                DataObjectPtr outMsg) 
                :
                operationName(operationNam),
                soapAction(soapAct),  
                endpoint(endpointAddress),
                inputMessage(inMsg),
                outputMessage(outMsg)
            {
                LOGENTRY(1, "WsdlOperation::constructor");                
                LOGEXIT(1, "WsdlOperation::constructor");
            }

            WsdlOperation::~WsdlOperation()
            {
            }

            string WsdlOperation::getResponseName()
            {
                string responseType(outputMessage->getList("part")[0]->getCString("element"));
                string responseTypeName;
                string responseTypeUri;
                Utils::tokeniseQName(responseType, responseTypeUri, responseTypeName); 
                return responseTypeName; 
            }

            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany
