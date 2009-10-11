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
#include <iostream>
#include <fstream>
#include <string>
#include "driver.hpp"

namespace store {

bool contains(const std::string& str, const std::string& pattern) {
    return str.find(pattern) != str.npos;
}

bool testScript() {
    std::ifstream is("store.scm", std::ios_base::in);
    std::ostringstream os;
    tuscany::evalDriverRun(is, os);
    assert(contains(os.str(), "List::(List::(List::(Symbol::name, (String::\"apple\", ())), (List::(Symbol::currency, (String::\"USD\", ())), (List::(Symbol::symbol, (String::\"$\", ())), (List::(Symbol::price, (Number::2.99, ())), ())))), ())"));
    return true;
}

const tuscany::value evalLoop(std::istream& is, const tuscany::value& req, tuscany::Env& globalEnv) {
    tuscany::value in = tuscany::read(is);
    if(tuscany::isNil(in))
        return tuscany::eval(req, globalEnv);
    tuscany::eval(in, globalEnv);
    return evalLoop(is, req, globalEnv);
}

bool testEval() {
    std::ifstream is("store.scm", std::ios_base::in);
    std::ostringstream os;

    tuscany::setupEvalOut(os);
    tuscany::Env globalEnv = tuscany::setupEnvironment();

    const tuscany::value req(tuscany::makeList<tuscany::value>("storeui_service", std::string("getcatalog")));
    const tuscany::value res = evalLoop(is, req, globalEnv);

    std::ostringstream rs;
    rs << res;
    assert(contains(rs.str(), "List::(List::(List::(Symbol::name, (String::\"apple\", ())), (List::(Symbol::currency, (String::\"USD\", ())), (List::(Symbol::symbol, (String::\"$\", ())), (List::(Symbol::price, (Number::2.99, ())), ())))), (List::(List::(Symbol::name, (String::\"orange\", ())), (List::(Symbol::currency, (String::\"USD\", ())), (List::(Symbol::symbol, (String::\"$\", ())), (List::(Symbol::price, (Number::3.55, ())), ())))), (List::(List::(Symbol::name, (String::\"pear\", ())), (List::(Symbol::currency, (String::\"USD\", ())), (List::(Symbol::symbol, (String::\"$\", ())), (List::(Symbol::price, (Number::1.55, ())), ())))), ())))"));

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
