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

#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif

#include "commonj/sdo/SDO.h"

#include "AccountDataServiceImpl.h"

using namespace commonj::sdo;

namespace com
{
    namespace bigbank
    {
        namespace account
        {

            DataObjectPtr /* CheckingAccount*/ AccountDataServiceImpl::getCheckingAccount(
            const char *id) 
            {
                try {

                    DataFactoryPtr factory  = DataFactory::getDataFactory();

                    XSDHelperPtr helper = HelperProvider::getXSDHelper(factory);
    
                    helper->defineFile("AccountService.wsdl");

                    DataObjectPtr newaccount = 
                        factory->create("http://www.bigbank.com/AccountService","CheckingAccount");

                    char* name = new char[strlen(id) + 10];
                    strcpy(name,id);
                    strcat(name,"_CHA12345");

                    newaccount->setCString("accountNumber",name);
                    
                    delete name;

                    newaccount->setFloat("balance",1500.0f);

                    return newaccount;

                }
                catch (SDORuntimeException e)
                {
                    return 0;
                }
            }



            DataObjectPtr /* SavingsAccount */ AccountDataServiceImpl::getSavingsAccount(
            const char *id) 
            {
                try {

                    DataFactoryPtr factory  = DataFactory::getDataFactory();

                    XSDHelperPtr helper = HelperProvider::getXSDHelper(factory);
    
                    helper->defineFile("AccountService.wsdl");

                    DataObjectPtr newaccount = 
                        factory->create("http://www.bigbank.com/AccountService","SavingsAccount");

                    char* name = new char[strlen(id) + 10];
                    strcpy(name,id);
                    strcat(name,"_SAA12345");

                    newaccount->setCString("accountNumber",name);
                    
                    delete name;

                    newaccount->setFloat("balance",1500.0f);

                    return newaccount;

                }
                catch (SDORuntimeException e)
                {
                    return 0;
                }
            }


            DataObjectPtr /* StockAccount */ AccountDataServiceImpl::getStockAccount (
            const char* id)
            {
                try {

                    DataFactoryPtr factory  = DataFactory::getDataFactory();

                    XSDHelperPtr helper = HelperProvider::getXSDHelper(factory);
    
                    helper->defineFile("AccountService.wsdl");

                    DataObjectPtr newaccount = 
                        factory->create("http://www.bigbank.com/AccountService","StockAccount");

                    char* name = new char[strlen(id) + 10];
                    strcpy(name,id);
                    strcat(name,"_STA12345");

                    newaccount->setCString("accountNumber",name);
                    
                    delete name;

                    newaccount->setCString("symbol","IBM");

                    newaccount->setInteger("quantity",100);

                    return newaccount;

                }
                catch (SDORuntimeException e)
                {
                    return 0;
                }
            }
        }
    }
}