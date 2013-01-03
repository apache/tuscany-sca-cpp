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
 * Shopping cart component implementation.
 */

#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"

namespace tuscany {
namespace store {

const string cartId("1234");

/**
 * Get the shopping cart from the cache. Return an empty
 * cart if not found.
 */
const list<value> getcart(const value& id, const lvvlambda& cache) {
    const value cart = cache(mklist<value>("get", mklist<value>(id)));
    cerr << "cart value: " << cart << "\n";
    const failable<value> fcart = cart;
    cerr << "cart fvalue: " << fcart << "\n";
    cerr << "cart content: " << content(fcart) << "\n";
    cerr << "cart reason: " << reason(fcart) << "\n";
    if (isNull(cart))
        return value(nilListValue);
    return (list<value>)cart;
}

/**
 * Post a new item to the cart. Create a new cart if necessary.
 */
const failable<value> post(unused const list<value>& collection, const value& item, const lvvlambda& cache) {
    const value id(mkuuid());
    const list<value> newItem(mklist<value>("entry", cadr<value>(car<value>(item)), mklist<value>("id", id), cadddr<value>(car<value>(item))));
    const list<value> cart(cons<value>(newItem, getcart(cartId, cache)));
    cache(mklist<value>("put", mklist<value>(cartId), cart));
    return value(mklist<value>(id));
}

/**
 * Find an item in the cart.
 */
const value find(const value& id, const list<value>& cart) {
    if (isNull(cart))
        return mklist<value>(mklist<value>("entry", mklist<value>("title", string("Item")), mklist<value>("id", "0")));
    if (id == cadr<value>(caddr<value>(car(cart))))
        return mklist<value>(car(cart));
    return find(id, cdr(cart));
}

/**
 * Return items from the cart.
 */
const failable<value> get(const list<value>& id, const lvvlambda& cache) {
    if (isNull(id))
        return value(mklist<value>(append(mklist<value>("feed", mklist<value>("title", string("Your Cart")), mklist<value>("id", cartId)), getcart(cartId, cache))));
    return find(car(id), getcart(cartId, cache));
}

/**
 * Delete items from the cart.
 */
const failable<value> del(const list<value>& id, unused const lvvlambda& cache) {
    if (isNull(id))
        return cache(mklist<value>("delete", mklist<value>(cartId)));
    return trueValue;
}

/**
 * Return the price of an item.
 */
const double price(const list<value>& item) {
    return cadr<value>(assoc<value>("price", cdr<value>(cadr<value>(cadddr(item)))));
}

/**
 * Sum the prices of a list of items.
 */
const double sum(const list<value>& items) {
    if (isNull(items))
        return 0;
    return price(car(items)) + sum(cdr(items));
}

/**
 * Return the total price of the items in the cart.
 */
const failable<value> total(const lvvlambda& cache) {
    const list<value> cart(getcart(cartId, cache));
    return value(sum(cart));
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "post")
        return tuscany::store::post(cadr(params), caddr(params), cadddr(params));
    if (func == "get")
        return tuscany::store::get(cadr(params), caddr(params));
    if (func == "delete")
        return tuscany::store::del(cadr(params), caddr(params));
    if (func == "total")
        return tuscany::store::total(cadr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
