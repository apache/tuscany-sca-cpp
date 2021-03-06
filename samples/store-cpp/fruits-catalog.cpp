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

/**
 * Catalog component implementation.
 */

#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"

namespace tuscany {
namespace store {

/**
 * Returns the catalog.
 */
const list<value> mkfruit(const string& name, const string& code, const string& symbol, const double price) {
    return nilListValue +
        mklist<value>("name", name) + mklist<value>("currencyCode", code) + mklist<value>("currencySymbol", symbol) + mklist<value>("price", price);
}

const failable<value> items(const lvvlambda& converter, const lvvlambda& currencyCode) {
    const string currency(currencyCode(nilListValue));
    const string symbol(converter(mklist<value>("symbol", currency)));
    const vvlambda conv = [converter, currency](const value& price) -> const value {
        return converter(mklist<value>("convert", string("USD"), currency, price));
    };

    return value(nilListValue +
        mkfruit("Apple", currency, symbol, conv(2.99)) +
        mkfruit("Orange", currency, symbol, conv(3.55)) +
        mkfruit("Pear", currency, symbol, conv(1.55)));
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "items")
        return tuscany::store::items(cadr(params), caddr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
