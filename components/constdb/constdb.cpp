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
 * TinyCDB-based database component implementation.
 */

#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "tinycdb.hpp"

namespace tuscany {
namespace constdb {

/**
 * Get an item from the database.
 */
const failable<value> get(const list<value>& params, tinycdb::TinyCDB& cdb) {
    return tinycdb::get(car(params), cdb);
}

/**
 * Post an item to the database.
 */
const failable<value> post(const list<value>& params, tinycdb::TinyCDB& cdb) {
    const value id = append<value>(car(params), mklist(mkuuid()));
    const failable<bool> val = tinycdb::post(id, cadr(params), cdb);
    if (!hasContent(val))
        return mkfailure<value>(reason(val));
    return id;
}

/**
 * Put an item into the database.
 */
const failable<value> put(const list<value>& params, tinycdb::TinyCDB& cdb) {
    const failable<bool> val = tinycdb::put(car(params), cadr(params), cdb);
    if (!hasContent(val))
        return mkfailure<value>(reason(val));
    return value(content(val));
}

/**
 * Delete an item from the database.
 */
const failable<value> del(const list<value>& params, tinycdb::TinyCDB& cdb) {
    const failable<bool> val = tinycdb::del(car(params), cdb);
    if (!hasContent(val))
        return mkfailure<value>(reason(val));
    return value(content(val));
}

/**
 * Component implementation lambda function.
 */
class applyConstDb {
public:
    applyConstDb(tinycdb::TinyCDB& cdb) : cdb(cdb) {
    }

    const value operator()(const list<value>& params) const {
        const value func(car(params));
        if (func == "get")
            return get(cdr(params), cdb);
        if (func == "post")
            return post(cdr(params), cdb);
        if (func == "put")
            return put(cdr(params), cdb);
        if (func == "delete")
            return del(cdr(params), cdb);
        return tuscany::mkfailure<tuscany::value>();
    }

private:
    tinycdb::TinyCDB& cdb;
};

/**
 * Start the component.
 */
const failable<value> start(unused const list<value>& params) {
    // Connect to the configured database and table
    const value dbname = ((lambda<value(list<value>)>)car(params))(list<value>());
    tinycdb::TinyCDB& cdb = *(new (gc_new<tinycdb::TinyCDB>()) tinycdb::TinyCDB(dbname));

    // Return the component implementation lambda function
    return value(lambda<value(const list<value>&)>(applyConstDb(cdb)));
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "start")
        return tuscany::constdb::start(cdr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
