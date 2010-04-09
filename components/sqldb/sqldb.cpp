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
 * PostgreSQL-based database component implementation.
 */

#include <apr_uuid.h>

#include "string.hpp"

#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "pgsql.hpp"

namespace tuscany {
namespace sqldb {

/**
 * Get an item from the database.
 */
const failable<value> get(const list<value>& params, pgsql::PGSql& pg) {
    return pgsql::get(car(params), pg);
}

/**
 * Post an item to the database.
 */
const value uuidValue() {
    apr_uuid_t uuid;
    apr_uuid_get(&uuid);
    char buf[APR_UUID_FORMATTED_LENGTH];
    apr_uuid_format(buf, &uuid);
    return value(string(buf, APR_UUID_FORMATTED_LENGTH));
}

const failable<value> post(const list<value>& params, pgsql::PGSql& pg) {
    const value id = append<value>(car(params), mklist(uuidValue()));
    const failable<bool> val = pgsql::post(id, cadr(params), pg);
    if (!hasContent(val))
        return mkfailure<value>(reason(val));
    return id;
}

/**
 * Put an item into the database.
 */
const failable<value> put(const list<value>& params, pgsql::PGSql& pg) {
    const failable<bool> val = pgsql::put(car(params), cadr(params), pg);
    if (!hasContent(val))
        return mkfailure<value>(reason(val));
    return value(content(val));
}

/**
 * Delete an item from the database.
 */
const failable<value> del(const list<value>& params, pgsql::PGSql& pg) {
    const failable<bool> val = pgsql::del(car(params), pg);
    if (!hasContent(val))
        return mkfailure<value>(reason(val));
    return value(content(val));
}

/**
 * Component implementation lambda function.
 */
class applySqldb {
public:
    applySqldb(pgsql::PGSql& pg) : pg(pg) {
    }

    const value operator()(const list<value>& params) const {
        const value func(car(params));
        if (func == "get")
            return get(cdr(params), pg);
        if (func == "post")
            return post(cdr(params), pg);
        if (func == "put")
            return put(cdr(params), pg);
        if (func == "delete")
            return del(cdr(params), pg);
        return tuscany::mkfailure<tuscany::value>();
    }

private:
    pgsql::PGSql& pg;
};

/**
 * Start the component.
 */
const failable<value> start(unused const list<value>& params) {
    // Connect to the configured database and table
    const value conninfo = ((lambda<value(list<value>)>)car(params))(list<value>());
    const value table = ((lambda<value(list<value>)>)cadr(params))(list<value>());
    pgsql::PGSql& pg = *(new (gc_new<pgsql::PGSql>()) pgsql::PGSql(conninfo, table));

    // Return the component implementation lambda function
    return value(lambda<value(const list<value>&)>(applySqldb(pg)));
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "start")
        return tuscany::sqldb::start(cdr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
