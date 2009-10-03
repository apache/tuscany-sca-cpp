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

#ifndef tuscany_item_hpp
#define tuscany_item_hpp

#include <string>
#include "service.hpp"

namespace store
{

typedef tuscany::value ItemType;

const ItemType makeItem(const std::string& name, const std::string& currencyCode, const std::string& currencySymbol, const double price) {
    return tuscany::makeList<tuscany::value>(name, currencyCode, currencySymbol, price);
}

const std::string itemName(const ItemType& item) {
    return car((tuscany::list<tuscany::value>)item);
}

const std::string itemCurrencyCode(const ItemType& item) {
    return car(cdr((tuscany::list<tuscany::value>)item));
}

const std::string itemCurrencySymbol(const ItemType& item) {
    return car(cdr(cdr((tuscany::list<tuscany::value>)item)));
}

const double itemPrice(const ItemType& item) {
    return car(cdr(cdr(cdr((tuscany::list<tuscany::value>)item))));
}

}
#endif /* tuscany_item_hpp */
