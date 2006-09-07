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

#include "StockQuoteServiceImpl.h"
#include "StockQuoteWebService.h"

#include "osoa/sca/sca.h"
#include "commonj/sdo/SDO.h"

using namespace osoa::sca;
using namespace commonj::sdo;

/**
 * StockQuoteServiceImpl component implementation
 */
namespace services {
    namespace stockquote {

        // Map the format returned by the StockQuote Web service to
        // the format expected in the BigBank application
        float StockQuoteServiceImpl::getQuote(const char *symbol)
        {
            ComponentContext myContext = ComponentContext::getCurrent();

            // Get the StockQuote Web service
            StockQuoteWebService* webService = 
                (StockQuoteWebService*)myContext.getService("webService");

            // Invoke the service
            const char* result = webService->GetQuote(symbol);

            float stockPrice = 0.0f;

            // Get the stock price out of the document returned by the StockQuote
            // Web service
            try
            {
	            XMLHelperPtr xmlHelper = HelperProvider::getXMLHelper(myContext.getDataFactory());
 	            XMLDocumentPtr stockDoc = xmlHelper->load(result);
                if (stockDoc->getRootDataObject())
                {
                    stockPrice=stockDoc->getRootDataObject()->getFloat("Stock.0/Last");
                }
            }
            catch (SDORuntimeException e)
            {
                std::cout << e << std::endl;
                return 0.0f;
            }
            return stockPrice;
        }

    } // End stockquote
} // End services
