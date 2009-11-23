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

#include <map>
#include <list>
#include <string>
#include <sstream>

#include "tuscany/sca/model/WSDLOperation.h"
#include "tuscany/sca/model/WSDLMessagePart.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/core/Exceptions.h"

using namespace std;
using namespace commonj::sdo;
using namespace tuscany::sca::util;

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            
            
            // Constructor
            WSDLOperation::WSDLOperation() 
            {
                logentry(); 
            }

            WSDLOperation::~WSDLOperation()
            {
                logentry(); 
            }


            void WSDLOperation::setInputMessageType(const string& inputType)
            {
                logentry(); 
            	Utils::tokeniseQName(inputType, inputMessageUri, inputMessageName); 
            }

            void WSDLOperation::setOutputMessageType(const string& outputType)
            {
                logentry(); 
            	Utils::tokeniseQName(outputType, outputMessageUri, outputMessageName); 
            }

            void WSDLOperation::setInputMessage( commonj::sdo::DataObjectPtr inputMsg )
            {
                logentry();

                DataObjectList &partList = inputMsg->getList("part");
                for( unsigned int i=0; i < partList.size(); i++)
                {
                    string partUri;
                    string partType;
                    string partName = partList[0]->getCString("name");

                    Utils::tokeniseQName(partList[0]->getCString("element"), partUri, partType);
                    WSDLMessagePart part(partName, partType, partUri);

                    inputPartMap[partName] = part;
                }
            }

            void WSDLOperation::setOutputMessage( commonj::sdo::DataObjectPtr outputMsg )
            {
                logentry();

                DataObjectList &partList = outputMsg->getList("part");
                for( unsigned int i=0; i < partList.size(); i++)
                {
                    string partUri;
                    string partType;
                    string partName = partList[0]->getCString("name");

                    Utils::tokeniseQName(partList[0]->getCString("element"), partUri, partType);
                    WSDLMessagePart part(partName, partType, partUri);

                    outputPartMap[partName] = part;
                }
            }

            const std::list<std::string> WSDLOperation::getInputMessagePartNames() const
            {
                logentry();

                std::list<std::string> inputPartNames;

                PART_MAP::const_iterator iter = inputPartMap.begin();
                PART_MAP::const_iterator iterEnd = inputPartMap.end();

                for( ; iter != iterEnd; ++iter )
                {
                    inputPartNames.push_back( iter->first );
                }

                return inputPartNames;
            }

            const std::list<std::string> WSDLOperation::getOutputMessagePartNames() const
            {
                logentry();

                std::list<std::string> outputPartNames;

                PART_MAP::const_iterator iter = outputPartMap.begin();
                PART_MAP::const_iterator iterEnd = outputPartMap.end();

                for( ; iter != iterEnd; ++iter )
                {
                    outputPartNames.push_back( iter->first );
                }

                return outputPartNames;
            }

            const WSDLMessagePart&
            WSDLOperation::getInputMessagePart( const std::string &name ) const
            {
                logentry();

                PART_MAP::const_iterator iter = inputPartMap.find( name );
                if( iter == inputPartMap.end() )
                {
                    stringstream errMessage;
                    errMessage
                      << "Unable to find input WSDLMessagePart for partName: " << name;

                    throwException(SystemConfigurationException, errMessage.str().c_str());
                }

                return iter->second;
            }

            const WSDLMessagePart&
            WSDLOperation::getOutputMessagePart( const std::string &name ) const
            {
                logentry();

                PART_MAP::const_iterator iter = outputPartMap.find( name );
                if( iter == outputPartMap.end() )
                {
                    stringstream errMessage;
                    errMessage
                      << "Unable to find output WSDLMessagePart for partName: " << name;

                    throwException(SystemConfigurationException, errMessage.str().c_str());
                }

                return iter->second;
            }

            void WSDLOperation::setInputMessagePart( const std::string &partName,
                                                     WSDLMessagePart part )
            {
                logentry();

                inputPartMap[partName] = part;
            }

            void WSDLOperation::setOutputMessagePart( const std::string &partName,
                                                      WSDLMessagePart part )
            {
                logentry();

                outputPartMap[partName] = part;
            }

       } // End namespace model
    } // End namespace sca
} // End namespace tuscany
