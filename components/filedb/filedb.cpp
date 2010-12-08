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
const failable<value> get(const list<value>& params, filedb::FileDB& db) {
    return filedb::get(car(params), db);
}

/**
 * Post an item to the database.
 */
const failable<value> post(const list<value>& params, filedb::FileDB& db) {
    const value id = append<value>(car(params), mklist(mkuuid()));
    const failable<bool> val = filedb::post(id, cadr(params), db);
    if (!hasContent(val))
        return mkfailure<value>(reason(val));
    return id;
}

/**
 * Put an item into the database.
 */
const failable<value> put(const list<value>& params, filedb::FileDB& db) {
    const failable<bool> val = filedb::put(car(params), cadr(params), db);
    if (!hasContent(val))
        return mkfailure<value>(reason(val));
    return value(content(val));
}

/**
 * Delete an item from the database.
 */
const failable<value> del(const list<value>& params, filedb::FileDB& db) {
    const failable<bool> val = filedb::del(car(params), db);
    if (!hasContent(val))
        return mkfailure<value>(reason(val));
    return value(content(val));
}

/**
 * Component implementation lambda function.
 */
class applyfiledb {
public:
    applyfiledb(filedb::FileDB& db) : db(db) {
    }

    const value operator()(const list<value>& params) const {
        const value func(car(params));
        if (func == "get")
            return get(cdr(params), db);
        if (func == "post")
            return post(cdr(params), db);
        if (func == "put")
            return put(cdr(params), db);
        if (func == "delete")
            return del(cdr(params), db);
        return tuscany::mkfailure<tuscany::value>();
    }

private:
    filedb::FileDB& db;
};

/**
 * Start the component.
 */
const failable<value> start(unused const list<value>& params) {
    // Connect to the configured database and table
    const value dbname = ((lambda<value(list<value>)>)car(params))(list<value>());
    filedb::FileDB& db = *(new (gc_new<filedb::FileDB>()) filedb::FileDB(dbname));

    // Return the component implementation lambda function
    return value(lambda<value(const list<value>&)>(applyfiledb(db)));
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
