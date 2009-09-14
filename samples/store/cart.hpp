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

#ifndef store_cart_hpp
#define store_cart_hpp

#include <string.h>
#include "tuscany/function.hpp"
#include "tuscany/list.hpp"
#include "currency.hpp"
#include "item.hpp"
#include "catalog.hpp"

using namespace tuscany;

namespace store
{

const double accum(const double total, const Item& item) {
    return total + item.price;
}

class ShoppingCart {
public:
    virtual const list<Item> getAll() const = 0;

    virtual const bool post(const Item& item) = 0;

    virtual const bool deleteAll() = 0;

    virtual const double getTotal() const = 0;
};

class ShoppingCartImpl : public ShoppingCart {
public:
    list<Item> cart;

    virtual const list<Item> getAll() const {
        return cart;
    }

    virtual const bool post(const Item& item) {
        cart = cons(item, cart);
        return true;
    }

    virtual const bool deleteAll() {
        cart = list<Item>();
        return true;
    }

    virtual const double getTotal() const {
        lambda<double(double, Item)> a(accum);
        return reduce(a, 0.0, cart);
    }
};

}
#endif /* store_cart_hpp */
