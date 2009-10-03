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

#ifndef tuscany_cart_hpp
#define tuscany_cart_hpp

#include <string>
#include "service.hpp"
#include "item.hpp"

namespace store
{

const double accumTotal(const double total, const ItemType& item) {
    return total + itemPrice(item);
}

tuscany::list<ItemType> cart;

const tuscany::list<ItemType> shoppingCart_getAll() {
    return cart;
}

const bool shoppingCart_post(const ItemType& item) {
    cart = cons(item, cart);
    return true;
}

const bool shoppingCart_deleteAll() {
    cart = tuscany::list<ItemType>();
    return true;
}

const double shoppingCart_getTotal() {
    tuscany::lambda<double(double, ItemType)> a(accumTotal);
    return reduce(a, 0.0, cart);
}

const tuscany::value shoppingCart_service(const tuscany::list<tuscany::value>& args) {
    if (car(args) == tuscany::value("getAll"))
        return tuscany::value(shoppingCart_getAll());
    if (car(args) == tuscany::value("post"))
        return tuscany::value(shoppingCart_post(cadr(args)));
    if (car(args) == tuscany::value("deleteAll"))
        return tuscany::value(shoppingCart_deleteAll());
    if (car(args) == tuscany::value("getTotal"))
        return tuscany::value(shoppingCart_getTotal());
    return tuscany::value();
}

}
#endif /* tuscany_cart_hpp */
