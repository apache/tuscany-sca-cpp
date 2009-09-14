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

#ifndef SCA_STOREUI_HPP_
#define SCA_STOREUI_HPP_

#include <string.h>
#include "tuscany/list.hpp"
#include "currency.hpp"
#include "item.hpp"
#include "catalog.hpp"
#include "cart.hpp"

namespace tuscany
{

class StoreUI {
public:

    virtual const list<Item> getCatalog() const =0;

    virtual const list<Item> getCart() const = 0;

    virtual const double getTotal() const =0;

    virtual const bool post(const Item& item) = 0;
};

class StoreUIImpl : public StoreUI {
public:
    const Catalog& catalog;
    ShoppingCart& cart;

    StoreUIImpl(const Catalog& catalog, ShoppingCart& cart) : catalog(catalog), cart(cart) {
    }

    virtual const list<Item> getCatalog() const {
        return catalog.get();
    }

    virtual const list<Item> getCart() const {
        return cart.getAll();
    }

    virtual const double getTotal() const {
        return cart.getTotal();
    }

    virtual const bool post(const Item& item) {
        return cart.post(item);
    }
};

}
#endif /* SCA_STOREUI_HPP_ */
