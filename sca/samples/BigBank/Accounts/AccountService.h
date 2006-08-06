/*
 *
 *  Copyright 2006 The Apache Software Foundation or its licensors, as applicable.
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

/**
 * AccountService.h
 *
 */


#include <commonj/sdo/SDO.h>


namespace com
{
    namespace bigbank
    {
        namespace account
        {

    /*
     *  AccountService C interface
     */

            class AccountService {
 		 
            public:

                //The account report data object has one many-valued
                // property "accountSummaries".
                // This had properties
                // String accountNumber
                // String accountType
                // Float balance.

                 virtual commonj::sdo::DataObjectPtr /*AccountReport*/
                     getAccountReport(const char* customerID) = 0;
         
        
            };
    
        }   
    }
} // namespaces 
    