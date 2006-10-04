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

#include "AccountService.h"
#include "osoa/sca/sca.h"
#include <iostream>
#include <stdlib.h>

using namespace osoa::sca;
using namespace std;
using namespace services::account;
using namespace commonj::sdo;

int main(int argc, char* argv[])
{
	
    if (argc != 2)
    {
        cout << "account_client: Argument must be the name of the account" << endl;
    	return 0;
    }
	try
	{

        // Locate a service
		CompositeContext myContext = CompositeContext::getCurrent();
		AccountService *accountService = 
            (AccountService*) myContext.locateService("AccountServiceComponent");
		if (accountService == 0)
		{
			cout << "account_client: Unable to find Account service" << endl;
		}
		else
		{
			try
			{
                // exercise the account service

                DataObjectPtr report = 
                    accountService->getAccountReport(argv[1]);
                    
                XMLHelperPtr xmlHelper = HelperProvider::getXMLHelper(myContext.getDataFactory()); 
                char* doc = xmlHelper->save(report, report->getType().getURI(), report->getType().getName());
                cout << "XML: " << doc <<endl;
                
                if (report != 0)
                {
                    int i;
                    char balanceStr[20];
                    float balance;

                    cout << "My Account Report" << endl;
                    cout << "=================" << endl;

                    DataObjectList& stocks = report->getList("stocks");

                    for (i=0;i< stocks.size() ;i++)
                    {
                        cout << "Stock Account " << stocks[i]->getCString("accountNumber") << endl;
                        cout << "Stock Symbol : " << stocks[i]->getCString("symbol") << endl;
                        cout << "     Holding : " << stocks[i]->getCString("quantity") << endl;
                        balance = stocks[i]->getFloat("balance");
                        sprintf(balanceStr,"%.2f",balance);
                        cout << "       Value : " << balanceStr << endl << endl;
                    }

                    DataObjectList& checking = report->getList("checking");
    
                    for (i=0;i< checking.size() ;i++)
                    {
                        cout << "Checking Account " << checking[i]->getCString("accountNumber") << endl;
                        balance = checking[i]->getFloat("balance");
                        sprintf(balanceStr,"%.2f",balance);
                        cout << "      Balance  : " << balanceStr << endl << endl;
                    }

                    DataObjectList& savings = report->getList("savings");

                    for (i=0;i< savings.size() ;i++)
                    {
                        cout << "Savings Account " <<  savings[i]->getCString("accountNumber") << endl;
                        balance = savings[i]->getFloat("balance");
                        sprintf(balanceStr,"%.2f",balance);
                        cout << "      Balance  : " << balanceStr << endl << endl;
                    }

                    cout << endl;
                }
			}
            catch (SDORuntimeException& e)
            {
                cout << e << endl;
            }
			catch (char* x)
			{
				cout << "account_client: exception caught: " << x << endl;
			}
		}
		
	}
	catch (ServiceRuntimeException& ex)
	{
		cout << ex << endl;
	}
	return 0;
}

