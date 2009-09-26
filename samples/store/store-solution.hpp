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

#ifndef store_storesolution_hpp
#define store_storesolution_hpp

#include "tuscany/list.hpp"
#include "store-composite.hpp"
#include "currency-composite.hpp"

namespace store
{

class StoreSolution : public Store {
};

class StoreSolutionImpl : public StoreSolution {
public:
    const CurrencyImpl currency;
    StoreImpl store;

    StoreSolutionImpl() :
        currency(CurrencyImpl()), store(StoreImpl(currency)) {
    }

    virtual const tuscany::list<Item> getCatalog() const {
        return store.getCatalog();
    }

    virtual const tuscany::list<Item> getCart() const {
        return store.getCart();
    }

    virtual const double getTotal() const {
        return store.getTotal();
    }

    virtual const bool post(const Item& item) {
        return store.post(item);
    }
};

}
#endif /* store_storesolution_hpp */
