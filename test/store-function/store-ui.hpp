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

#ifndef tuscany_storeui_hpp
#define tuscany_storeui_hpp

#include <string>
#include "service.hpp"
#include "currency.hpp"
#include "item.hpp"
#include "catalog.hpp"
#include "cart.hpp"

namespace store
{

const tuscany::list<ItemType> storeUI_getCatalog(const service& catalog) {
    return catalog(tuscany::makeList<tuscany::value>("get"));
}

const tuscany::list<ItemType> storeUI_getCart(const service& cart) {
    return cart(tuscany::makeList<tuscany::value>("getAll"));
}

const double storeUI_getTotal(const service& cart) {
    return cart(tuscany::makeList<tuscany::value>("getTotal"));
}

const bool storeUI_post(const service& cart, const ItemType& item) {
    return cart(tuscany::makeList<tuscany::value>("post", item));
}

const tuscany::value storeUI_service(const service& catalog, const service& cart, const tuscany::list<tuscany::value>& args) {
    if (car(args) == "getCatalog")
        return storeUI_getCatalog(catalog);
    if (car(args) == "getCart")
        return storeUI_getCart(cart);
    if (car(args) == "getTotal")
        return storeUI_getTotal(cart);
    if (car(args) == "post")
        return storeUI_post(cart, cadr(args));
    return tuscany::value();
}

}
#endif /* STOREUI_HPP_ */
