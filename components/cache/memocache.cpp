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
 * A cache component implementation which memoizes the value of function
 * applications, keyed by the function arguments, in a key/value cache passed
 * as a reference.
 *
 * This is useful if your functions are idempotent and applied many times to
 * the same arguments. The results can then be retrieved quickly from the
 * cache without actually applying the function.
 */

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"

namespace tuscany {
namespace memocache {

/**
 * Memoize the value of a function application in a cache.
 */
const failable<value> memoize(const list<value>& params, const lambda<value(const list<value>&)>& relay, const lambda<value(const list<value>&)>& cache) {
    debug(params, "memocache::memoize::params");

    // Lookup memoized value from cache
    const value val = cache(mklist<value>("get", params));
    if (!isNil(val)) {
        debug(val, "memocache::memoize::cached");
        return val;
    }

    // Apply the given function
    const value res = relay(params);
    debug(res, "memocache::memoize::res");

    // Store the result value in the cache
    cache(mklist<value>("put", params, res));

    return res;
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "start" || func == "stop")
        return tuscany::mkfailure<tuscany::value>();
    const tuscany::list<tuscany::value> rev = tuscany::reverse(params);
    return tuscany::memocache::memoize(tuscany::reverse(tuscany::cddr(rev)), tuscany::cadr(rev), tuscany::car(rev));
}

}
