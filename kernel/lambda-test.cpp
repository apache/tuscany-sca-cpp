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
 * Test C++0x lambda expressions.
 */

#include <assert.h>
#include "function.hpp"
#include "sstream.hpp"
#include "fstream.hpp"

namespace tuscany {

const lambda<const int(const int)> inc(const int i) {
    return [=](const int x)->const int {
        return x + i;
    };
}

const int square(const int x) {
    return x * x;
}

int mapLambda(const lambda<const int(const int)> f, int v) {
    return f(v);
}

bool testLambda() {
    const lambda<const int(const int)> sq  = square;
    assert(sq(2) == 4);
    assert(mapLambda(square, 2) == 4);
    assert(mapLambda(sq, 2) == 4);
    assert(mapLambda([](const int x)->const int { return x * x; }, 2) == 4);

    const lambda<int(int)> incf = inc(10);
    assert(incf(1) == 11);
    assert(mapLambda(incf, 1) == 11);
    assert(mapLambda(inc(10), 1) == 11);

    lambda<const int(const int)> l;
    l = incf;
    assert(l(1) == 11);
    l = square;
    assert(l(2) == 4);
    return true;
}

}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::testLambda();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
