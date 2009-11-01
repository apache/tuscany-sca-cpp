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
#include <iostream>
#include <fstream>
#include <string>
#include "list.hpp"
#include "driver.hpp"
#include "slist.hpp"
#include "xml.hpp"
#include "../json/json.hpp"

namespace store {

using namespace tuscany;

bool contains(const std::string& str, const std::string& pattern) {
    return str.find(pattern) != std::string::npos;
}

bool testScript() {
    std::ifstream is("store.scm", std::ios_base::in);
    std::ostringstream os;
    eval::evalDriverRun(is, os);

    assert(contains(os.str(), "(\"Sample Feed\" \""));
    assert(contains(os.str(), "\" (\"Item\" \""));
    assert(contains(os.str(), "\" ((javaClass \"services.Item\") (name \"Orange\") (currency \"USD\") (symbol \"$\") (price 3.55))) (\"Item\" \""));
    assert(contains(os.str(), "\" ((javaClass \"services.Item\") (name \"Apple\") (currency \"USD\") (symbol \"$\") (price 2.99))))"));
    return true;
}

const value evalLoop(std::istream& is, const value& req, eval::Env& globalEnv, const gc_pool& pool) {
    value in = eval::readValue(is);
    if(isNil(in))
        return eval::evalExpr(req, globalEnv, pool);
    eval::evalExpr(in, globalEnv, pool);
    return evalLoop(is, req, globalEnv, pool);
}

bool testEval() {
    {
        std::ifstream is("store.scm", std::ios_base::in);
        std::ostringstream os;
        eval::setupDisplay(os);

        gc_pool pool;
        eval::Env globalEnv = eval::setupEnvironment(pool);
        const value req(mklist<value>("storeui_service", std::string("getcatalog")));
        const value val = evalLoop(is, req, globalEnv, pool);

        std::ostringstream vs;
        vs << val;
        assert(contains(vs.str(), "(((javaClass \"services.Item\") (name \"Apple\") (currency \"USD\") (symbol \"$\") (price 2.99)) ((javaClass \"services.Item\") (name \"Orange\") (currency \"USD\") (symbol \"$\") (price 3.55)) ((javaClass \"services.Item\") (name \"Pear\") (currency \"USD\") (symbol \"$\") (price 1.55)))"));
    }

    {
        std::ifstream is("store.scm", std::ios_base::in);
        std::ostringstream os;
        eval::setupDisplay(os);

        gc_pool pool;
        eval::Env globalEnv = eval::setupEnvironment(pool);
        const value req(mklist<value>("storeui_service", std::string("gettotal")));
        const value res = evalLoop(is, req, globalEnv, pool);

        std::ostringstream rs;
        rs << res;
        assert(contains(rs.str(), "10"));
    }
    return true;
}

}

int main() {

    std::cout << "Testing..." << std::endl;

    store::testScript();
    store::testEval();

    std::cout << "OK" << std::endl;

    return 0;
}
