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
 * Patch an item in the database.
 */
const failable<value> patch(const list<value>& params, const pgsql::PGSql& pg) {
    // Read patch
    value p = assoc<value>("patch", assoc<value>("content", car<value>(cadr(params))));
    if (isNull(p))
        return mkfailure<value>("Couldn't read patch script");
    const string script = cadr<value>(p);
    debug(script, "pgsql::patch::script");

    const lambda<const failable<value>(const value&, const pgsql::PGSql&, const string&, const int)> tryPatch = [&tryPatch](const value& key, const pgsql::PGSql& pg, const string& script, const int count) -> const failable<value> {

        // Begin database transaction
        const failable<bool> brc = pgsql::begin(pg);
        if (!hasContent(brc))
            return mkfailure<value>(brc);

        // Get existing value from database
        const failable<value> ival = pgsql::get(key, pg);
        if (!hasContent(ival) && rcode(ival) != 404) {
            pgsql::rollback(pg);
            return mkfailure<value>(ival);
        }

        // Apply patch
        istringstream is(script);
        scheme::Env env = scheme::setupEnvironment();
        const value pval = scheme::evalScript(cons<value>("patch", scheme::quotedParameters(mklist<value>(key, hasContent(ival)? content(ival) : (value)list<value>()))), is, env);
        if (isNull(pval)) {
            ostringstream os;
            os << "Couldn't patch postgresql entry: " << key;
            return mkfailure<value>(str(os), 404, false);
        }

        // Push patched value to database
        const failable<bool> val = pgsql::patch(key, pval, pg);
        if (!hasContent(val)) {
            pgsql::rollback(pg);

            // Retry on a transaction serialization error
            if (rcode(val) == 409 && count > 0)
                return tryPatch(key, pg, script, count - 1);
            return mkfailure<value>(val);
        }

        // Commit database transaction
        const failable<bool> crc = pgsql::commit(pg);
        if (!hasContent(crc))
            return mkfailure<value>(crc);

        return value(content(val));
    };

    // Try patching the entry and automatically retry a few times on transaction
    // serialization errors
    return tryPatch(car(params), pg, script, 5);
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
        if (func == "patch")
            return patch(cdr(params), *pg);
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
