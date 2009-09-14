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

#ifndef store_catalog_hpp
#define store_catalog_hpp

#include <string.h>
#include "tuscany/list.hpp"
#include "currency.hpp"
#include "item.hpp"

using namespace tuscany;

namespace store
{

class Catalog {
public:

    virtual const double convert(const double price) const = 0;

    virtual const list<Item> get() const = 0;
};

class CatalogImpl : public Catalog {
public:

    const string currencyCode;
    const CurrencyConverter& currencyConverter;

    CatalogImpl(const CurrencyConverter& currencyConverter) :
        currencyCode("USD"), currencyConverter(currencyConverter) {
    }

    virtual const double convert(const double price) const {
        return currencyConverter.convert("USD", currencyCode, price);
    }

    virtual const list<Item> get() const {
        const string currencySymbol = currencyConverter.getSymbol(currencyCode);
        return makeList(
                Item("Apple", currencyCode, currencySymbol, convert(2.99)),
                Item("Orange", currencyCode, currencySymbol, convert(3.55)),
                Item("Pear", currencyCode, currencySymbol, convert(1.55)));
    }
};

}
#endif /* store_catalog_hpp */
