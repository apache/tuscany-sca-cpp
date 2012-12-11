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

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "parallel.hpp"
#include "pgsql.hpp"

namespace tuscany {
namespace sqldb {

/**
 * Get an item from the database.
 */
const failable<value> get(const list<value>& params, const pgsql::PGSql& pg) {
    return pgsql::get(car(params), pg);
}

/**
 * Post an item to the database.
 */
const failable<value> post(const list<value>& params, const pgsql::PGSql& pg) {
    const value id = append<value>(car(params), mklist(mkuuid()));
    const failable<bool> val = pgsql::post(id, cadr(params), pg);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return id;
}

/**
 * Put an item into the database.
 */
const failable<value> put(const list<value>& params, const pgsql::PGSql& pg) {
    const failable<bool> val = pgsql::put(car(params), cadr(params), pg);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return value(content(val));
}

/**
 * Delete an item from the database.
 */
const failable<value> del(const list<value>& params, const pgsql::PGSql& pg) {
    const failable<bool> val = pgsql::del(car(params), pg);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return value(content(val));
}

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {
    // Connect to the configured database and table
    debug("sqldb::start");
    const gc_pool cp(gc_current_pool());
    const value conninfo = ((lvvlambda)car(params))(nilListValue);
    const value table = ((lvvlambda)cadr(params))(nilListValue);
    const lambda<const gc_ptr<pgsql::PGSql>()> newPGSql = [conninfo, table, cp]() -> const gc_ptr<pgsql::PGSql> {
        debug("sqldb::newPGSql");
        const gc_scoped_pool sp(pool(cp));
        return new (gc_new<pgsql::PGSql>()) pgsql::PGSql(conninfo, table);
    };
    const perthread_ptr<pgsql::PGSql> pg = *(new (gc_new<perthread_ptr<pgsql::PGSql> >()) perthread_ptr<pgsql::PGSql>(newPGSql));

    // Return the component implementation lambda function
    const lvvlambda applySqldb = [pg](const list<value>& params) -> const value {
        const value func(car(params));
        if (func == "get")
            return get(cdr(params), *pg);
        if (func == "post")
            return post(cdr(params), *pg);
        if (func == "put")
            return put(cdr(params), *pg);
        if (func == "delete")
            return del(cdr(params), *pg);
        return mkfailure<value>();
    };
    return value(applySqldb);
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
