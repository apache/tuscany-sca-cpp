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
 * Test memory allocation functions.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "gc.hpp"
#include "function.hpp"
#include "perf.hpp"

namespace tuscany {

int countElements = 0;
int maxElements = 0;

class Element {
public:
    Element() : i(0) {
        countElements++;
        if (countElements > maxElements)
            maxElements = countElements;
    }

    Element(int i) : i(i) {
        countElements++;
        if (countElements > maxElements)
            maxElements = countElements;
    }

    Element(const Element& o) : i(o.i) {
        countElements++;
        if (countElements > maxElements)
            maxElements = countElements;
    }

    ~Element() {
        countElements--;
    }

    const bool operator==(const Element& o) const {
        return o.i == i;
    }

private:
    friend ostream& operator<<(ostream& out, const Element& v);

    int i;
    char c[20];
};

ostream& operator<<(ostream& out, const Element& v) {
    out << v.i ;
    return out;
}

bool poolAlloc(Element** p, const int count) {
    if (count == 0)
        return true;
    p[count - 1] = new (gc_new<Element>()) Element();
    return poolAlloc(p, count - 1);
};

bool poolFree(Element** p, const int count) {
    if (count == 0)
        return true;
    // Do nothing to free the element, but cycle through them just
    // to get a fair comparison with the other memory alloc tests
    return poolFree(p, count - 1);
};

struct poolAllocPerf {
    const int n;
    Element** p;
    poolAllocPerf(const int n) : n(n), p(new Element*[n]) {
    }
    const bool operator()() const {
        gc_scoped_pool gc;
        poolAlloc(p, n);
        return true;
    }
};

bool testPoolAllocPerf() {
    const int count = 10000;
    const lambda<bool()> pl = poolAllocPerf(count);
    maxElements = 0;
    cout << "Memory pool alloc test " << (time(pl, 1, 1) / count) << " ms" << endl;
    assert(countElements == 0);
    assert(maxElements == count);
    return true;
}

bool stdAlloc(Element** p, const int count) {
    if (count == 0)
        return true;
    p[count - 1] = new Element();
    return stdAlloc(p, count - 1);
};

bool stdFree(Element** p, const int count) {
    if (count == 0)
        return true;
    delete p[count -1];
    return stdFree(p, count - 1);
};

struct stdAllocPerf {
    const int n;
    Element** p;
    stdAllocPerf(const int n) : n(n), p(new Element*[n]) {
    }
    const bool operator()() const {
        stdAlloc(p, n);
        stdFree(p, n);
        return true;
    }
};

bool testStdAllocPerf() {
    const int count = 10000;
    const lambda<bool()> sl = stdAllocPerf(count);
    maxElements = 0;
    cout << "Memory standard alloc test " << (time(sl, 1, 1) / count) << " ms" << endl;
    assert(countElements == 0);
    assert(maxElements == count);
    return true;
}

}

int main() {
    tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::testPoolAllocPerf();
    tuscany::testStdAllocPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
