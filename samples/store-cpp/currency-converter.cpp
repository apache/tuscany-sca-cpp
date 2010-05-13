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
 * Currency converter component implementation.
 */

#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"

namespace tuscany {
namespace store {

/**
 * Convert an amount from USD to a currency.
 */
const failable<value> convert(unused const value& from, const value& to, const value& amount) {
    if (to == string("EUR"))
        return value(0.70 * (double)amount);
    return amount;
}

/**
 * Return a currency symbol.
 */
const failable<value> symbol(const value& currency) {
    if (currency == string("EUR"))
        return value(string("E"));
    return value(string("$"));
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "convert")
        return tuscany::store::convert(cadr(params), caddr(params), cadddr(params));
    if (func == "symbol")
        return tuscany::store::symbol(cadr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
