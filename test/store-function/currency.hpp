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

#ifndef tuscany_currency_hpp
#define tuscany_currency_hpp

#include <math.h>
#include <string>
#include "debug.hpp"
#include "service.hpp"
#include "item.hpp"

namespace store
{

const double currencyConverter_convert(unused const std::string& fromCurrencyCode, const std::string& toCurrencyCode, const double amount) {
    if(toCurrencyCode == "USD")
        return amount;
    if(toCurrencyCode == "EUR")
        return round(amount * 0.7256 * 100) / 100;
    return amount;
}

const std::string currencyConverter_getSymbol(const std::string& currencyCode) {
    if(currencyCode == "USD")
        return "$";
    if(currencyCode == "EUR")
        return "E";
    return "?";
}

const tuscany::value currencyConverter_service(const tuscany::list<tuscany::value>& args) {
    if (car(args) == "convert")
        return currencyConverter_convert(cadr(args), car(cdr(cdr(args))), car(cdr(cdr(cdr(args)))));
    if (car(args) == "getSymbol")
        return currencyConverter_getSymbol(cadr(args));
    return tuscany::value();
}

}
#endif /* tuscany_currency_hpp */
