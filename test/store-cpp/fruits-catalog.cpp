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
struct convert {
    const lambda<value(const list<value>&)> converter;
    const string currency;
    convert(const lambda<value(const list<value>&)>& converter, const string& currency) : converter(converter), currency(currency) {
    }
    const value operator()(const value& price) const {
        return converter(mklist<value>("convert", string("USD"), currency, price));
    }
};

const list<value> mkfruit(const string& name, const string& code, const string& symbol, const double price) {
    return list<value>() +
        mklist<value>("javaClass", string("services.Item")) + mklist<value>("name", name) + mklist<value>("currencyCode", code) + mklist<value>("currencySymbol", symbol) + mklist<value>("price", price);
}

const failable<value> get(const lambda<value(const list<value>&)> converter, const lambda<value(const list<value>&)> currencyCode) {
    const string currency(currencyCode(list<value>()));
    const string symbol(converter(mklist<value>("symbol", currency)));
    const lambda<value(const value&)> conv(convert(converter, currency));

    return value(list<value>() +
        mkfruit("Apple", currency, symbol, conv(2.99)) +
        mkfruit("Orange", currency, symbol, conv(3.55)) +
        mkfruit("Pear", currency, symbol, conv(1.55)));
}

/**
 * TODO remove this JSON-RPC specific function.
 */
const failable<value> listMethods(unused const lambda<value(const list<value>&)> converter, unused const lambda<value(const list<value>&)> currencyCode) {
    return value(mklist<value>(string("Service.get")));
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "get")
        return tuscany::store::get(cadr(params), caddr(params));
    if (func == "listMethods")
        return tuscany::store::listMethods(cadr(params), caddr(params));
    return tuscany::mkfailure<tuscany::value>(tuscany::string("Function not supported: ") + func);
}

}
