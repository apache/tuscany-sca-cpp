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

#ifndef tuscany_sca_util_utils_h
#define tuscany_sca_util_utils_h

#include <string>
#include <ostream>

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/export.h"

namespace tuscany
{
    namespace sca
    {
        namespace util
        {
            /**
             * Utility methods to parse strings and provide debugging information.
             */
            class SCA_API Utils {
    
            public:
                static void tokeniseUri(const std::string& uri, std::string& token1, std::string& token2);
                static void tokeniseQName(const std::string& sdoname, std::string& uri, std::string& name);
                static void tokeniseString(
                    const std::string& separator,
                    const std::string& str,
                    std::string& token1,
                    std::string& token2);
    
                static void rTokeniseString(
                    const std::string& separator,
                    const std::string& str,
                    std::string& token1,
                    std::string& token2);
    
                static void breakpoint();

                static void printLastError(std::ostream& os);
    
                static void printDO(commonj::sdo::DataObjectPtr dataObject, int increment=0);
                static void printTypes(commonj::sdo::DataFactoryPtr df);
                static void printType(const commonj::sdo::Type& type, int increment=0);
    
                static const bool compareDataObjects(commonj::sdo::DataObjectPtr dataObject1, commonj::sdo::DataObjectPtr dataObject2, std::string& diff);
                static const bool compareProperties(commonj::sdo::DataObjectPtr dataObject1, const commonj::sdo::Property& prop1, commonj::sdo::DataObjectPtr dataObject2, const commonj::sdo::Property& prop2, std::string& diff);
    
            private:
                static void tabs(int increment=0);
            };
        
        } // End namespace util
    } // End namespace sca
} // End namespace tuscany
#endif // tuscany_sca_util_utils_h
