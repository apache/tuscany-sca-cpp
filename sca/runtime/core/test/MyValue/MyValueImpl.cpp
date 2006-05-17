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

#include "MyValueImpl.h"
#include "CustomerInfo.h"
#include "StockQuoteService.h"
#include <iostream>
using namespace std;

#include "osoa/sca/sca.h"
#include "commonj/sdo/SDO.h"
using namespace osoa::sca;
using namespace commonj::sdo;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyValueImpl::MyValueImpl()
{
    cout << "Construct MyValueImpl" << endl;

}

MyValueImpl::~MyValueImpl()
{
    cout << "Destroy MyValueImpl" << endl;
}

//////////////////////////////////////////////////////////////////////
// Other methods
//////////////////////////////////////////////////////////////////////
float MyValueImpl::getMyValue(const char *customerID) 
{
    cout << "In getMyValue with customerid: " << customerID << endl;


    ComponentContext myContext = ComponentContext::getCurrent();
    CustomerInfo* customerInfoService = (CustomerInfo*)myContext.getService("customerInfo");

    if (customerInfoService == 0)
    {
        cout << "unable to find customer info service" << endl;

        return 0.0f;
    }

    const char * stock;
    try
    {
        stock = customerInfoService->getCustomerInformation(customerID);
    }
    catch(const char* x)
    {
        cout << "Exception caught: " << x <<endl;
        throw;
    }

    ServiceList serviceList = myContext.getServices("customerInfo");

    for (unsigned int i=0; i<serviceList.size();i++)
    {
        CustomerInfo* service = (CustomerInfo*)(serviceList[i]);
        stock = service->getCustomerInformation(customerID);
        cout << "Stock from getServices " << i << " : " << stock <<endl;

    }

    // Use a second SCA reference to get the stock price
    StockQuoteService* stockQuoteService = (StockQuoteService*)myContext.getService("stockQuote");

    if (stockQuoteService == 0)
    {
        cout << "unable to find stock quote service" << endl;

        return 0.1f;
    }

    // Create a data object representing the requests (use dynamic API until static is available)
    DataObjectPtr requestDO;


    // Invoke the service
    DataObjectPtr result = stockQuoteService->GetQuotes("IBM");

    float stockPrice = 0.2f;

    try {
        float stockPrice = result->getDataObject("StockQuotes")->getList("StockQuote")[0]->getFloat("PrevClose");
        //dataobjectlist& dlist = result->getdataobject("getquotesresult")->getlist("quote");
        //DataObjectList& dlist = result->getDataObject("GetQuotesResult")->getList((unsigned int)0);
        //float stockPrice = result->getDataObject("GetQuotesResult")->getList(0u)[0]->getFloat("StockQuote");
        cout << "My stock price is " << stockPrice << endl;
        //float stockPrice=((DataObject)result.getDataObject("GetQuotesResult").getList(0).get(0)).getFloat("StockQuote");
        //        System.out.println("    [getMyValue] stock quote = "+stockPrice);
        } catch (SDORuntimeException e)
    {
        cout << e;
    }

    return stockPrice;

    /*
    // Dummy method

    if (!strcmp(stock, "IBM"))
        return 99.999f;
    else
        return 31.05f;
    */

}

float MyValueImpl::getMyValueS(const string& customerID) 
{
    cout << "In getMyValueS with customerid: " << customerID.c_str() << endl;
    return 87.35f;
}

string MyValueImpl::getCustname(string& customerID) 
{
    cout << "In getCustname with customerid: " << customerID.c_str() << endl;
    customerID = "FRED";
    cout << "setting customer name to " << customerID.c_str() <<endl;
    return "done";
}

static string fred("FEREDDEYY");
const string& MyValueImpl::getCustnamecs(string customerID) 
{
    cout << "In getCustname with customerid: " << customerID.c_str() << endl;
    customerID = "FRED";
    cout << "setting customer name to " << customerID.c_str() <<endl;
    return fred;
}
