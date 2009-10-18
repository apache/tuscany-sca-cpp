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

#ifndef tuscany_catalog_hpp
#define tuscany_catalog_hpp

#include <string>
#include "service.hpp"
#include "currency.hpp"
#include "item.hpp"

namespace store
{

std::string catalog_currencyCode = "USD";

double catalog_convert(const service& currencyConverter, const double price) {
    return currencyConverter(tuscany::mklist<tuscany::value>("convert", "USD", catalog_currencyCode, price));
}

const tuscany::list<ItemType> catalog_get(const service& currencyConverter) {
    const std::string currencySymbol = currencyConverter_service(tuscany::mklist<tuscany::value>("getSymbol", catalog_currencyCode));
    return tuscany::mklist(
            item("Apple", catalog_currencyCode, currencySymbol, catalog_convert(currencyConverter, 2.99)),
            item("Orange", catalog_currencyCode, currencySymbol, catalog_convert(currencyConverter, 3.55)),
            item("Pear", catalog_currencyCode, currencySymbol, catalog_convert(currencyConverter, 1.55)));
}

const tuscany::value catalog_service(const service& currencyConverter, const tuscany::list<tuscany::value>& args) {
    if (car(args) == "get")
        return catalog_get(currencyConverter);
    return tuscany::value();
}

}
#endif /* tuscany_catalog_hpp */
