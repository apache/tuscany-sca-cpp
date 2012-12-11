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
 * Store Test case.
 */

#include <assert.h>
#include <regex.h>
#include "stream.hpp"
#include "string.hpp"
#include "list.hpp"
#include "../../modules/xml/xml.hpp"
#include "../../modules/json/json.hpp"
#include "../../modules/scheme/driver.hpp"

namespace store {

using namespace tuscany;

const bool testScript() {
    const gc_scoped_pool pool;

    ifstream is("script-test.scm");
    ostringstream os;
    scheme::evalDriverRun(is, os);
    assert(contains(str(os), "(\"Sample Feed\" \""));
    assert(contains(str(os), "\" (\"Item\" \""));
    assert(contains(str(os), "\" ((name \"Orange\") (currencyCode \"USD\") (currencySymbol \"$\") (price 3.55))) (\"Item\" \""));
    assert(contains(str(os), "\" ((name \"Apple\") (currencyCode \"USD\") (currencySymbol \"$\") (price 2.99))))"));
    return true;
}

const bool testEval() {
    {
        const gc_scoped_pool pool;
        ifstream is("script-test.scm");
        ostringstream os;
        scheme::setupDisplay(os);
        scheme::Env globalEnv = scheme::setupEnvironment();
        const value exp(mklist<value>("storeui_service", string("items")));
        const value val = scheme::evalScript(exp, is, globalEnv);

        ostringstream vs;
        vs << val;
        assert(contains(str(vs), "(((name \"Apple\") (currencyCode \"USD\") (currencySymbol \"$\") (price 2.99)) ((name \"Orange\") (currencyCode \"USD\") (currencySymbol \"$\") (price 3.55)) ((name \"Pear\") (currencyCode \"USD\") (currencySymbol \"$\") (price 1.55)))"));
    }

    {
        const gc_scoped_pool pool;
        ifstream is("script-test.scm");
        ostringstream os;
        scheme::setupDisplay(os);

        scheme::Env globalEnv = scheme::setupEnvironment();
        const value exp(mklist<value>("storeui_service", string("total")));
        const value res = scheme::evalScript(exp, is, globalEnv);

        ostringstream rs;
        rs << res;
        assert(contains(str(rs), "10"));
    }
    return true;
}

}

int main() {

    tuscany::cout << "Testing..." << tuscany::endl;

    store::testScript();
    store::testEval();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
