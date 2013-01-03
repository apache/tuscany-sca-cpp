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

#define WANT_HTTPD_LOG 1
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
const failable<value> get(const list<value>& params, const tinycdb::TinyCDB& cdb) {
    return tinycdb::get(car(params), cdb);
}

/**
 * Post an item to the database.
 */
const failable<value> post(const list<value>& params, const tinycdb::TinyCDB& cdb) {
    const value id = append<value>(car(params), mklist(mkuuid()));
    const failable<bool> val = tinycdb::post(id, cadr(params), cdb);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return id;
}

/**
 * Put an item into the database.
 */
const failable<value> put(const list<value>& params, const tinycdb::TinyCDB& cdb) {
    const failable<bool> val = tinycdb::put(car(params), cadr(params), cdb);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return value(content(val));
}

/**
 * Patch an item in the database.
 */
const failable<value> patch(const list<value>& params, const tinycdb::TinyCDB& cdb) {
    // Read patch
    value p = assoc<value>("patch", assoc<value>("content", car<value>(cadr(params))));
    if (isNull(p))
        return mkfailure<value>("Couldn't read patch script");
    const string script = cadr<value>(p);
    debug(script, "tinycdb::patch::script");
    istringstream is(script);

    // Get existing value from database
    const failable<value> ival = tinycdb::get(car(params), cdb);
    if (!hasContent(ival) && rcode(ival) != 404)
        return mkfailure<value>(ival);

    // Apply patch
    scheme::Env env = scheme::setupEnvironment();
    const value pval = scheme::evalScript(cons<value>("patch", scheme::quotedParameters(mklist<value>(car(params), hasContent(ival)? content(ival) : (value)list<value>()))), is, env);
    if (isNull(pval)) {
        ostringstream os;
        os << "Couldn't patch tinycdb entry: " << car(params);
        return mkfailure<value>(str(os), 404, false);
    }

    // Push patched value to database
    const failable<bool> val = tinycdb::patch(car(params), pval, cdb);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return value(content(val));
}

/**
 * Delete an item from the database.
 */
const failable<value> del(const list<value>& params, const tinycdb::TinyCDB& cdb) {
    const failable<bool> val = tinycdb::del(car(params), cdb);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return value(content(val));
}

/**
 * Start the component.
 */
const failable<value> start(unused const list<value>& params) {
    // Connect to the configured database and table
    const value dbname = ((lvvlambda)car(params))(nilListValue);
    const tinycdb::TinyCDB& cdb = *(new (gc_new<tinycdb::TinyCDB>()) tinycdb::TinyCDB(dbname));

    // Return the component implementation lambda function
    const lvvlambda applyConstDb = [cdb](const list<value>& params) -> const value {
        const value func(car(params));
        if (func == "get")
            return get(cdr(params), cdb);
        if (func == "post")
            return post(cdr(params), cdb);
        if (func == "put")
            return put(cdr(params), cdb);
        if (func == "patch")
            return patch(cdr(params), cdb);
        if (func == "delete")
            return del(cdr(params), cdb);
        return mkfailure<value>();
    };
    return value(applyConstDb);
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
