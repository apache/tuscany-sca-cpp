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

#include "AccountDataService.h"

namespace services
{
    namespace accountdata
    {

        /**
         *  AccountDataServiceImpl component implementation class
         */
        class AccountDataServiceImpl : public AccountDataService
        {
        public:

            // CheckingAccount is a data object containing
            // string accountNumber
            // float balance

            virtual DataObjectPtr /* CheckingAccount*/ getCheckingAccount(
                const string id);

            // SavingsAccount is a data object containing
            // string accountNumber
            // float balance

            virtual DataObjectPtr /* SavingsAccount */ getSavingsAccount(
                const string id);

            // StockAccount is a data object containing
            // string accountNumber
            // string symbol
            // int quantity

            virtual DataObjectPtr /* StockAccount */ getStockAccount (
                const string id);

        };
        
    } // End accountdata
} // End services