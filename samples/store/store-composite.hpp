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

#ifndef SCA_STORECOMPOSITE_HPP_
#define SCA_STORECOMPOSITE_HPP_

#include <string.h>
#include "tuscany/list.hpp"
#include "currency.hpp"
#include "currency-composite.hpp"
#include "item.hpp"
#include "catalog.hpp"
#include "cart.hpp"
#include "store-ui.hpp"

namespace tuscany
{

class Store : public StoreUI {
};

class StoreImpl : public Store {
public:
    const CatalogImpl catalog;
    ShoppingCartImpl cart;
    StoreUIImpl storeUI;

    StoreImpl(const Currency& currency) :
        catalog(CatalogImpl(currency)), cart(ShoppingCartImpl()), storeUI(StoreUIImpl(catalog, cart)) {
    }

    virtual const list<Item> getCatalog() const {
        return storeUI.getCatalog();
    }

    virtual const list<Item> getCart() const {
        return storeUI.getCart();
    }

    virtual const double getTotal() const {
        return storeUI.getTotal();
    }

    virtual const bool post(const Item& item) {
        return storeUI.post(item);
    }

};

}
#endif /* SCA_STORECOMPOSITE_HPP_ */
