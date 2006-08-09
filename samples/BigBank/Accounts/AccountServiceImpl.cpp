/**
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

#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif

#include "AccountServiceImpl.h"
#include "StockQuoteService.h"
#include "AccountDataService.h"

#include "osoa/sca/sca.h"

using namespace osoa::sca;


namespace com
{
    namespace bigbank
    {
        namespace account
        {

            AccountServiceImpl::AccountServiceImpl() 
            {
                initialized = false;
                // Note - cannot use the component context within the constructor - need to move this
                // code out to lazy init on first use.

                //ComponentContext myContext = ComponentContext::getCurrent();
    
                //dataService = 
                //    (AccountDataService*)myContext.getService("AccountDataService");
        
                //stockService = 
                //    (StockQuoteService*)myContext.getService("StockQuoteService");

            }

            //void AccountServiceImpl::lazyGetDependencies() 
            //{
                // cant do this either - the context cannot be instantiated 
                // resolve the references on call only
                
                //theContext = ComponentContext::getCurrent();
    
                //dataService = 
                //    (AccountDataService*)theContext.getService("AccountDataService");
        
                //stockService = 
                //    (StockQuoteService*)theContext.getService("stockQuote");

                //initialized = true;
            //}

            AccountServiceImpl::~AccountServiceImpl() 
            {
            }

            DataObjectPtr /*AccountReport**/ 
                AccountServiceImpl::getAccountReport(const char* customerID)
            {
                try {

                    ComponentContext theContext = ComponentContext::getCurrent();
    
                    commonj::sdo::DataFactoryPtr factory = theContext.getDataFactory();
 
                    commonj::sdo::DataObjectPtr newReport = 
                        factory->create("http://www.bigbank.com/AccountService","AccountReport");
                     
                    com::bigbank::account::AccountDataService *dataService = 
                       (com::bigbank::account::AccountDataService*)theContext.getService("accountData");
        
                    if (dataService == 0)
                    {
                            std::cout << "No Account Data Service registered" << std::endl;
                            return newReport;
                    }

                    // would be better to return a list of accounts - this only
                    // gets the first of each.

                    commonj::sdo::DataObjectPtr checking =                     
                        dataService->getCheckingAccount(customerID);

                    if (checking != 0)
                    {
                        commonj::sdo::DataObjectList& dl = newReport->getList("checking");
                        checking->setFloat("balance",
                            fromUSDollarToCurrency(checking->getFloat("balance")));
                        dl.append(checking);
                    }

                    commonj::sdo::DataObjectPtr savings = dataService->getSavingsAccount(customerID);

                    if (savings != 0)
                    {
                        commonj::sdo::DataObjectList& dl = newReport->getList("savings");
                        savings->setFloat("balance",
                            fromUSDollarToCurrency(savings->getFloat("balance")));
                        dl.append(savings);
                    }

                    com::bigbank::stockquote::StockQuoteService* stockService = 
                      (StockQuoteService*)theContext.getService("stockQuote");
                    
                    if (stockService == 0)
                    {
                        std::cout << "No Stock Quote Service registered" << std::endl;
                        return newReport;
                    }

                    commonj::sdo::DataObjectPtr stocks = dataService->getStockAccount(customerID);

                    if (stocks != 0)
                    {
                        commonj::sdo::DataObjectList& dl = newReport->getList("stocks");

                        float value = 10.0f;
                        try {
                            value = stockService->getQuote(
                            stocks->getCString("symbol"));
                        }
                        catch (ServiceRuntimeException& e)
                        {
                            std::cout << "Fault from stock quote service" << e << std::endl;
                        }
                        stocks->setFloat("balance",
                        fromUSDollarToCurrency(stocks->getInteger("quantity") *
                        value));
                        dl.append(stocks);
                    }
                    return newReport;
                }
                catch (SDORuntimeException& e)
                {
                    std::cout << "Error from SDO: " << e << std::endl;
                    return 0;
                }
            }


            float AccountServiceImpl::fromUSDollarToCurrency(float value) 
            {
                // get the property "currency"
                ComponentContext myContext = ComponentContext::getCurrent();
        
                commonj::sdo::DataObjectInstance properties = myContext.getProperties();
                if (properties)
                {
                    const char* currency = properties->getCString("currency");
                    if (currency != 0)
                    {
                        std::cout << "Currency is : " << currency << std::endl;
                        if (!strcmp(currency,"USD")) return value;
                        if (!strcmp(currency,"EURO")) return 0.8f * value;
                    }
                }
                return 0.0f;
            }
        }
    }
} // namespaces
