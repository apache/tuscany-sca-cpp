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

#ifndef tuscany_sca_util_utils_h
#define tuscany_sca_util_utils_h
#include <string>
using std::string;
#include "commonj/sdo/SDO.h"

#include "tuscany/sca/export.h"

namespace tuscany
{
    namespace sca
    {
        /**
         * Utility methods to parse strings and provide debugging information.
         */
        class SCA_API Utils {

        public:
            static void tokeniseUri(const string& uri, string& token1, string& token2);
            static void tokeniseQName(const string& sdoname, string& uri, string& name);
            static void tokeniseString(
                const string& separator,
                const string& str,
                string& token1,
                string& token2);

            static void rTokeniseString(
                const string& separator,
                const string& str,
                string& token1,
                string& token2);

            static void breakpoint();

            static void printDO(commonj::sdo::DataObjectPtr dataObject, int increment=0);
            static void printTypes(commonj::sdo::DataFactoryPtr df);
            static void printType(const commonj::sdo::Type& type, int increment=0);

            static const bool compareDataObjects(commonj::sdo::DataObjectPtr dataObject1, commonj::sdo::DataObjectPtr dataObject2, string& diff);
            static const bool compareProperties(commonj::sdo::DataObjectPtr dataObject1, const commonj::sdo::Property& prop1, commonj::sdo::DataObjectPtr dataObject2, const commonj::sdo::Property& prop2, string& diff);

        private:
            static void tabs(int increment=0);
        };
        
    } // End namespace sca
} // End namespace tuscany
#endif // tuscany_sca_util_utils_h
