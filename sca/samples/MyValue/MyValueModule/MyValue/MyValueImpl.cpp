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

//
// MyValueImpl.cpp: implementation of the MyValueImpl class.
//

#include "commonj/sdo/HelperProvider.h"
#include "commonj/sdo/SDO.h"
#include "osoa/sca/sca.h"
#include "MyValueImpl.h"
#include "CustomerInfo/CustomerInfo.h"
#include "StockQuoteService/StockQuoteService.h"
#include <iostream>

using namespace std;
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
DataObjectPtr MyValueImpl::getMyValue(DataObjectPtr inDataObject) 
{
	DataObjectPtr responseDO = 0;

	string customerID = inDataObject->getCString("customerID");
	cout << "In getMyValue with customerid: " << customerID << endl;

	// Get the CustomerInfo service.
	ComponentContext myContext = ComponentContext::getCurrent();
	CustomerInfo* customerInfoService = (CustomerInfo*)myContext.getService("customerInfo");

	if (customerInfoService == 0)
	{
		cout << "Unable to find customer info service" << endl;

		return responseDO;
	}

	// Get the name of the stock for customer 'customerID' from the customerInfoService.
	const char * stock;
	try
	{
		stock = customerInfoService->getCustomerInformation(customerID.c_str());
	}
	catch(const char* x)
	{
		cout << "Exception caught: " << x <<endl;
		throw;
	}


	// Check that we get the same stock back when iterating over the customerInfo services.
	ServiceList serviceList = myContext.getServices("customerInfo");
	for (int i=0; i<serviceList.size();i++)
	{
		CustomerInfo* service = (CustomerInfo*)(serviceList[i]);
		stock = service->getCustomerInformation(customerID.c_str());
		cout << "Stock from getServices " << i << " : " << stock <<endl;

	}

	// Use a second SCA reference to get the stock price for the customer's stock.
	StockQuoteService* stockQuoteService = (StockQuoteService*)myContext.getService("stockQuote");

	if (stockQuoteService == 0)
	{
		cout << "Unable to find stock quote service" << endl;

		return responseDO;
	}

	// Create a data object representing the requests (use dynamic API until static is available)
	DataObjectPtr stockQuoteRequestDO;
    DataFactoryPtr factory;
	try 
	{
		// Create the request DO for the StockQuoteService.
		factory = myContext.getDataFactory();
		stockQuoteRequestDO = factory->create("http://swanandmokashi.com", "GetQuotes");
		stockQuoteRequestDO->setCString("QuoteTicker", stock);
		
		// Invoke the StockQuoteService
		DataObjectPtr stockQuoteResponseDO = stockQuoteService->GetStockQuotes(stockQuoteRequestDO);
		
		// DEBUG (Print the response from the StockQuoteService)
		XMLHelperPtr xmlHelper = HelperProvider::getXMLHelper(factory);;
		XMLDocumentPtr responseDoc = 
			xmlHelper->createDocument(stockQuoteResponseDO, "http://swanandmokashi.com", "GetQuotesResponse");
		responseDoc->setXMLDeclaration(false);
		char *responseXML = xmlHelper->save(responseDoc);
		cout << "Response XML from StockQuoteService = " << endl;
		cout << responseXML << endl;
		cout << endl;
		// DEBUG
		
		// Create the MyValueService response DataObject using the results of the StockQuoteService.
		responseDO = factory->create("http://www.myvalue.org/MyValueService/", "getMyValueResponse");
		DataObjectPtr getQuotesResultDO = stockQuoteResponseDO->getDataObject("GetQuotesResult");
		DataObjectList& quoteList = getQuotesResultDO->getList("Quote");
		float fQuote = quoteList[0]->getFloat("StockQuote");
		
		responseDO->setFloat("myValue", fQuote);
	}
	catch (SDORuntimeException sdoE)
	{
		cout << sdoE;
	}
		
	return responseDO;

}

