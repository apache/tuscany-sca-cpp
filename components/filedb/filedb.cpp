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
 * File based database component implementation.
 */

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "filedb.hpp"

namespace tuscany {
namespace filedb {

/**
 * Get an item from the database.
 */
const failable<value> get(const list<value>& params, const filedb::FileDB& db) {
    return filedb::get(car(params), db);
}

/**
 * Post an item to the database.
 */
const failable<value> post(const list<value>& params, const filedb::FileDB& db) {
    const value id = append<value>(car(params), mklist(mkuuid()));
    const failable<bool> val = filedb::post(id, cadr(params), db);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return id;
}

/**
 * Put an item into the database.
 */
const failable<value> put(const list<value>& params, const filedb::FileDB& db) {
    const failable<bool> val = filedb::put(car(params), cadr(params), db);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return value(content(val));
}

/**
 * Delete an item from the database.
 */
const failable<value> del(const list<value>& params, const filedb::FileDB& db) {
    const failable<bool> val = filedb::del(car(params), db);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return value(content(val));
}

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {
    // Connect to the configured database and table
    const value dbname = ((lvvlambda)car(params))(nilListValue);
    const value format = ((lvvlambda)cadr(params))(nilListValue);

    const filedb::FileDB& db = *(new (gc_new<filedb::FileDB>()) filedb::FileDB(absdbname(dbname), format));

    // Return the component implementation lambda function
    const lvvlambda applyfiledb = [db](const list<value>& params) -> const value {
        const value func(car(params));
        if (func == "get")
            return get(cdr(params), db);
        if (func == "post")
            return post(cdr(params), db);
        if (func == "put")
            return put(cdr(params), db);
        if (func == "delete")
            return del(cdr(params), db);
        return mkfailure<value>();
    };
    return value(applyfiledb);
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "start")
        return tuscany::filedb::start(cdr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
