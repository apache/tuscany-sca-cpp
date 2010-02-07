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

#include <apr_general.h>
#include <apr_uuid.h>
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
const list<value> getcart(const value& id, const lambda<value(const list<value>&)> cache) {
    const value cart = cache(mklist<value>("get", id));
    if (isNil(cart))
        return value(list<value>());
    return (list<value>)cart;
}

/**
 * Returns a UUID.
 */
const value uuid() {
    apr_uuid_t uuid;
    apr_uuid_get(&uuid);
    char buf[APR_UUID_FORMATTED_LENGTH];
    apr_uuid_format(buf, &uuid);
    return string(buf, APR_UUID_FORMATTED_LENGTH);
}

/**
 * Post a new item to the cart. Create a new cart if necessary.
 */
const failable<value> post(const value& item, const lambda<value(const list<value>&)> cache) {
    const value id(uuid());
    const list<value> newItem(mklist<value>(car<value>(item), id, caddr<value>(item)));
    const list<value> cart(cons<value>(newItem, getcart(cartId, cache)));
    cache(mklist<value>("put", cartId, cart));
    return id;
}

/**
 * Return the contents of the cart.
 */
const failable<value> getall(const lambda<value(const list<value>&)> cache) {
    return value(append(mklist<value>(string("Your Cart"), cartId), getcart(cartId, cache)));
}

/**
 * Find an item in the cart.
 */
const value find(const value& id, const list<value>& cart) {
    if (isNil(cart))
        return cons<value>(string("Item"), mklist<value>("0", list<value>()));
    if (id == cadr<value>(car(cart)))
        return car(cart);
    return find(id, cdr(cart));
}

const failable<value> get(const value& id, const lambda<value(const list<value>&)> cache) {
    return find(id, getcart(cartId, cache));
}

/**
 * Delete the whole cart.
 */
const failable<value> delall(const lambda<value(const list<value>&)> cache) {
    return cache(mklist<value>("delete", cartId));
}

/**
 * Delete an item from the cart.
 */
const failable<value> del(unused const value& id, unused const lambda<value(const list<value>&)> cache) {
    return value(true);
}

/**
 * Return the price of an item.
 */
const double price(const list<value>& item) {
    return cadr<value>(assoc<value>("price", caddr(item)));
}

/**
 * Sum the prices of a list of items.
 */
const double sum(const list<value>& items) {
    if (isNil(items))
        return 0;
    return price(car(items)) + sum(cdr(items));
}

/**
 * Return the total price of the items in the cart.
 */
const failable<value> gettotal(const lambda<value(const list<value>&)> cache) {
    const list<value> cart(getcart(cartId, cache));
    return value(sum(cart));
}

/**
 * TODO remove this JSON-RPC specific function.
 */
const failable<value> listMethods(unused const lambda<value(const list<value>&)> converter) {
    return value(mklist<value>(string("Service.gettotal")));
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "post")
        return tuscany::store::post(cadr(params), caddr(params));
    if (func == "getall")
        return tuscany::store::getall(cadr(params));
    if (func == "get")
        return tuscany::store::get(cadr(params), caddr(params));
    if (func == "deleteall")
        return tuscany::store::delall(cadr(params));
    if (func == "delete")
        return tuscany::store::del(cadr(params), caddr(params));
    if (func == "gettotal")
        return tuscany::store::gettotal(cadr(params));
    if (func == "listMethods")
        return tuscany::store::listMethods(cadr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
