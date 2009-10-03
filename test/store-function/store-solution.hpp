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

#ifndef tuscany_storesolution_hpp
#define tuscany_storesolution_hpp

#include <string>
#include "service.hpp"
#include "store-composite.hpp"
#include "currency-composite.hpp"

namespace store
{

const tuscany::value storeSolution_service(const tuscany::list<tuscany::value>& args) {
    const service currency(currency_service);
    const tuscany::lambda<tuscany::value(service, tuscany::list<tuscany::value>)> storeService(store_service);
    const service configuredStoreService(curry(storeService, currency));
    return configuredStoreService(args);
}

}
#endif /* tuscany_storesolution_hpp */
