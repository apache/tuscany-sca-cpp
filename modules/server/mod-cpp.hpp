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

#ifndef tuscany_modcpp_hpp
#define tuscany_modcpp_hpp

/**
 * Evaluation functions used by mod-eval to evaluate implementation.cpp
 * component implementations.
 */

#include <string>
#include <iostream>
#include <fstream>

#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "dynlib.hpp"
#include "cache.hpp"
#include "../eval/driver.hpp"
#include "../http/httpd.hpp"
#include "mod-eval.hpp"

namespace tuscany {
namespace server {
namespace modeval {
namespace cpp {

/**
 * Evaluate a C++ component implementation function.
 */
struct evalImplementation {
    const lib ilib;
    const ilambda impl;
    evalImplementation(const lib& ilib, const ilambda& impl) : ilib(ilib), impl(impl) {
    }
    const failable<value, std::string> operator()(const value& func, const list<value>& params) const {
        httpd::logValue(cons<value>(func, params), "expr");
        const failable<value, std::string> val = impl(func, params);
        httpd::logValue(content(val), "val");
        return val;
    }
};

/**
 * Read a C++ component implementation.
 */
const failable<ilambda, std::string> readLatestImplementation(const std::string path) {
    const failable<lib, std::string> ilib(dynlib(path));
    if (!hasContent(ilib))
        return mkfailure<ilambda, std::string>(reason(ilib));

    const failable<ilambda, std::string> impl(dynlambda<failable<value, std::string>(value, list<value>)>("eval", content(ilib)));
    if (!hasContent(impl))
        return impl;
    return ilambda(evalImplementation(content(ilib), content(impl)));
}

const cached<failable<ilambda, std::string> > readImplementation(const std::string& path) {
    const lambda<failable<ilambda, std::string>(std::string)> ri(readLatestImplementation);
    const lambda<unsigned long(std::string)> ft(latestFileTime);
    const std::string p(path + dynlibExt);
    return cached<failable<ilambda, std::string> >(curry(ri, p), curry(ft, p));
}

}
}
}
}

#endif /* tuscany_modcpp_hpp */
