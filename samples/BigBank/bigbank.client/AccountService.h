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

#include "commonj/sdo/SDO.h"

#include <string>
using std::string;

namespace services
{
    namespace account
    {

        /**
         *  AccountService business interface
         */
        class AccountService
        {
        public:

            // The account report data object has one many-valued
            // property "accountSummaries", containing the following
            // properties:
            // string accountNumber
            // string accountType
            // float balance.

             virtual commonj::sdo::DataObjectPtr /*AccountReport*/
                 getAccountReport(const string customerID) = 0;
    
        };

    } // End account
} // End services
