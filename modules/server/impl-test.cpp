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
 * Test component implementation.
 */

#include <string>

#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "debug.hpp"

namespace tuscany {
namespace server {

const failable<value, std::string> get(unused const list<value>& params) {
    return value(std::string("Hey"));
}

const failable<value, std::string> post(unused const list<value>& params) {
    return value(std::string("1234"));
}

const failable<value, std::string> put(unused const list<value>& params) {
    return value(true);
}

const failable<value, std::string> del(unused const list<value>& params) {
    return value(true);
}

const failable<value, std::string> hello(const list<value>& params) {
    return value(std::string("hello ") + std::string(car(params)));
}

}
}

extern "C" {

const tuscany::value eval(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "get")
        return tuscany::server::get(cdr(params));
    if (func == "post")
        return tuscany::server::post(cdr(params));
    if (func == "put")
        return tuscany::server::put(cdr(params));
    if (func == "del")
        return tuscany::server::del(cdr(params));
    if (func == "hello")
        return tuscany::server::hello(cdr(params));
    return tuscany::mkfailure<tuscany::value, std::string>(std::string("Function not supported: ") + std::string(func));
}

}
