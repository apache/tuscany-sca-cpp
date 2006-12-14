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

#ifndef tuscany_sca_util_sdovisitor_h
#define tuscany_sca_util_sdovisitor_h

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <time.h>

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/export.h"

namespace tuscany
{
    namespace sca
    {
        /**
         * An SDO visitor interface
         */
        class SCA_API SDOVisitor
        {
        public:
            /**
             * Constructor.
             */
            SDOVisitor();

            /**
             * Destructor.
             */ 
            virtual ~SDOVisitor();

            /**
             * Visit depth
             */
            enum DEPTH
            {
                DEPTH_ZERO = 0,
                DEPTH_ONE,
                DEPTH_INFINITE
            };

            /**
             * Visit status
             */
            enum RESULT
            {
                RESULT_CONTINUE = 0,
                RESULT_STOP,
                RESULT_NOT_SUPPORTED
            };

            /**
             * Visit an SDO.
             */
            virtual RESULT visit(commonj::sdo::DataObjectPtr parent, const commonj::sdo::Property& property, bool boolData);
            virtual RESULT  visit(commonj::sdo::DataObjectPtr parent, const commonj::sdo::Property& property, char byteData);
            virtual RESULT  visit(commonj::sdo::DataObjectPtr parent, const commonj::sdo::Property& property, const char* bytesData);
            virtual RESULT  visit(commonj::sdo::DataObjectPtr parent, const commonj::sdo::Property& property, wchar_t charData);
            virtual RESULT  visit(commonj::sdo::DataObjectPtr parent, const commonj::sdo::Property& property, const commonj::sdo::SDODate& dateData);
            virtual RESULT  visit(commonj::sdo::DataObjectPtr parent, const commonj::sdo::Property& property, long double doubleData);
            virtual RESULT  visit(commonj::sdo::DataObjectPtr parent, const commonj::sdo::Property& property, float floatData);
            virtual RESULT  visit(commonj::sdo::DataObjectPtr parent, const commonj::sdo::Property& property, long intData);
            virtual RESULT  visit(commonj::sdo::DataObjectPtr parent, const commonj::sdo::Property& property, short shortData);
            virtual RESULT  visit(commonj::sdo::DataObjectPtr parent, const commonj::sdo::Property& property, const string& stringData);
            virtual RESULT  visit(commonj::sdo::DataObjectPtr parent, const commonj::sdo::Property& property, commonj::sdo::DataObjectPtr dataObjectData);
        };

        /**
         * SDO utilities.
         */
        class SCA_API SDOUtils
        {
        public:

            static SDOVisitor::RESULT accept(commonj::sdo::DataObjectPtr dataObject, SDOVisitor& visitor, SDOVisitor::DEPTH depth);

        };

    } // End namespace sca
} // End namespace tuscany
#endif // tuscany_sca_sdovisitor_file_h
