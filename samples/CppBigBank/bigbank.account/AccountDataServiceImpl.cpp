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

#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif

#include "commonj/sdo/SDO.h"
#include "osoa/sca/sca.h"

#include "AccountDataServiceImpl.h"

using namespace commonj::sdo;
using namespace osoa::sca;

/**
 * AccountDataServiceImpl component implementation
 */

namespace services
{
    namespace accountdata
    {

        DataObjectPtr /* CheckingAccount*/
            AccountDataServiceImpl::getCheckingAccount(const string id) 
        {
            try {

                DataFactoryPtr factory  = ComponentContext::getCurrent().getDataFactory();

                DataObjectPtr newaccount = 
                    factory->create("http://www.bigbank.com/AccountService","CheckingAccount");

                char* name = new char[id.size() + 10];
                strcpy(name,id.c_str());
                strcat(name,"_CHA12345");

                newaccount->setCString("accountNumber",name);
                
                delete name;

                newaccount->setFloat("balance",1500.0f);
                
                return newaccount;

            }
            catch (SDORuntimeException& e)
            {
                return 0;
            }
        }



        DataObjectPtr /* SavingsAccount */
            AccountDataServiceImpl::getSavingsAccount(const string id) 
        {
            try {

                DataFactoryPtr factory  = ComponentContext::getCurrent().getDataFactory();

                DataObjectPtr newaccount = 
                    factory->create("http://www.bigbank.com/AccountService","SavingsAccount");

                char* name = new char[id.size() + 10];
                strcpy(name,id.c_str());
                strcat(name,"_SAA12345");

                newaccount->setCString("accountNumber",name);
                
                delete name;

                newaccount->setFloat("balance",1500.0f);

                return newaccount;

            }
            catch (SDORuntimeException& e)
            {
                return 0;
            }
        }


        DataObjectPtr /* StockAccount */
            AccountDataServiceImpl::getStockAccount (const string id)
        {
            try {

                DataFactoryPtr factory  = ComponentContext::getCurrent().getDataFactory();

                DataObjectPtr newaccount = 
                    factory->create("http://www.bigbank.com/AccountService","StockAccount");

                char* name = new char[id.size() + 10];
                strcpy(name,id.c_str());
                strcat(name,"_STA12345");

                newaccount->setCString("accountNumber",name);
                
                delete name;

                newaccount->setCString("symbol","IBM");

                newaccount->setInteger("quantity",100);

                return newaccount;

            }
            catch (SDORuntimeException& e)
            {
                return 0;
            }
        }
        
    } // End accountdata
} // End services
