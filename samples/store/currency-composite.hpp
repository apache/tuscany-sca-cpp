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

#ifndef store_currencycomposite_hpp
#define store_currencycomposite_hpp

#include <string.h>
#include <math.h>
#include "currency.hpp"

using namespace tuscany;

namespace store
{

class Currency : public CurrencyConverter {
};

class CurrencyImpl : public Currency {
public:
    const CurrencyConverterImpl currencyConverter;

    CurrencyImpl() : currencyConverter(CurrencyConverterImpl()) {
    }

    virtual const double convert(const string& fromCurrencyCode, const string& toCurrencyCode, const double amount) const {
        return currencyConverter.convert(fromCurrencyCode, toCurrencyCode, amount);
    }

    virtual const string getSymbol(const string& currencyCode) const {
        return currencyConverter.getSymbol(currencyCode);
    }
};

}
#endif /* store_currencycomposite_hpp */
