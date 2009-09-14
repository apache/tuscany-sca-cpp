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

#ifndef store_currency_hpp
#define store_currency_hpp

#include <string.h>
#include <math.h>

using std::string;
using namespace tuscany;

namespace store
{

class CurrencyConverter {
public:

    virtual const double convert(const string& fromCurrencyCode, const string& toCurrencyCode, const double amount) const = 0;

    virtual const string getSymbol(const string& currencyCode) const = 0;
};

class CurrencyConverterImpl : public CurrencyConverter {
public:

    virtual const double convert(const string& fromCurrencyCode, const string& toCurrencyCode, const double amount) const {
        if(toCurrencyCode == "USD")
            return amount;
        if(toCurrencyCode == "EUR")
            return round(amount * 0.7256 * 100) / 100;
        return amount;
    }

    virtual const string getSymbol(const string& currencyCode) const {
        if(currencyCode == "USD")
            return "$";
        if(currencyCode == "EUR")
            return "E";
        return "?";
    }
};

}
#endif /* store_currency_hpp */
