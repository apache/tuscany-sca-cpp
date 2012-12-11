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
 * Test string functions.
 */

#include <assert.h>
#include <string>
#include "sstream.hpp"
#include "string.hpp"
#include "list.hpp"
#include "perf.hpp"

namespace tuscany {

const bool testCopies() {
    resetStringCopyCounters();
    const string x("abcd");
    assert(checkStringCopyCounters(1));
    resetStringCopyCounters();
    const string y = string("abcd");
    assert(checkStringCopyCounters(1));
    resetStringCopyCounters();
    unused const string z = y;
    assert(checkStringCopyCounters(0));
    resetStringCopyCounters();
    const list<string> pl = list<string>() + "abcd" + "efgh";
    printStringCopyCounters();
    resetStringCopyCounters();
    const list<string> cl = cons<string>("efgh", mklist<string>("abcd"));
    printStringCopyCounters();
    return true;
}

const bool testString() {
    const string s("abcd");
    assert(length(s) == 4);
    assert(!strcmp(c_str(s), "abcd"));

    assert(s == "abcd");
    assert(s == string("abcd"));
    assert(s != "zbcd");

    assert(s < "zbcd");
    assert(s < "zbc");
    assert(s < "abzd");
    assert(s < "abcdz");

    assert(s > "Abcd");
    assert(s > "Abc");
    assert(s > "abCd");
    assert(s > "Abcdz");

    const string x = "abcd";
    assert(!strcmp(c_str(x), "abcd"));

    const string y = string("abcd");
    assert(!strcmp(c_str(y), "abcd"));

    assert(string("ab") + "cd" == "abcd");

    assert(find("abcd", "cd") == 2);
    assert(find("abcd", "xy") == length("abcd"));
    assert(find_first_of("abcd", "cd") == 2);
    assert(find_first_of("abcd", "xy") == length("abcd"));
    assert(substr("abcdef", 4) == "ef");
    assert(substr("abcdef", 4, 2) == "ef");
    assert(substr("abcdef", 4, 3) == "ef");
    assert(substr("abcdef", 6, 3) == "");
    return true;
}

const bool testStream() {
    ostringstream os;
    os << "ab" << "cd";
    assert(str(os) == "abcd");

    ostringstream bos;
    bos << "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    bos << "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ";
    cout << str(bos) << endl;
    assert(str(bos) == 
    "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ");

    ostringstream cs;
    cs << "\'";
    assert(str(cs) == "\'");
    cs << '\'';
    assert(str(cs) == "\'\'");

    istringstream is("abcd");
    char b[2];
    assert(read(is, b, 2) == 2);
    assert(string("ab") == string(b, 2));
    assert(eof(is) == false);
    assert(read(is, b, 2) == 2);
    assert(string("cd") == string(b, 2));
    assert(eof(is) == true);
    assert(read(is, b, 2) == 0);
    return true;
}

const std::string stdAdd(const std::string& x, const std::string& y) {
    return x + y;
}

const string add(const string& x, const string& y) {
    return x + y;
}

char charBuffer[16385];

const bool testStringPerf() {
    memset(charBuffer, 'A', 16384);
    charBuffer[16384] = '\0';

    const int count = 10000;
    {
        const lambda<const blambda(const size_t size)> addStrings = [](const size_t size) -> const blambda {
            const blambda l = [size]() -> const bool {
                const size_t sz = size / 4;
                const string x(charBuffer, sz);
                const string y(charBuffer, sz);
                assert(length(add(x, y)) == sz * 2);
                return true;
            };
            return l;
        };

        const blambda a16 = addStrings(16);
        cout << "string test " << time(a16, 5, count) << " ms" << endl;
        const blambda a32 =addStrings(32);
        cout << "string test " << time(a32, 5, count) << " ms" << endl;
        const blambda a256 =addStrings(256);
        cout << "string test " << time(a256, 5, count) << " ms" << endl;
        const blambda a1024 =addStrings(1024);
        cout << "string test " << time(a1024, 5, count) << " ms" << endl;
        const blambda a4096 =addStrings(4096);
        cout << "string test " << time(a4096, 5, count) << " ms" << endl;
    }
    {
        const lambda<const blambda(const size_t size)> addStdStrings = [](const size_t size) -> const blambda {
            const blambda l = [size]() -> const bool {
                const size_t sz = size / 4;
                const std::string x(charBuffer, sz);
                const std::string y(charBuffer, sz);
                assert(stdAdd(x, y).length() == (unsigned int)(sz * 2));
                return true;
            };
            return l;
        };

        const blambda a16 =addStdStrings(16);
        cout << "Std string test " << time(a16, 5, count) << " ms" << endl;
        const blambda a32 =addStdStrings(32);
        cout << "Std string test " << time(a32, 5, count) << " ms" << endl;
        const blambda a256 =addStdStrings(256);
        cout << "Std string test " << time(a256, 5, count) << " ms" << endl;
        const blambda a1024 =addStdStrings(1024);
        cout << "Std string test " << time(a1024, 5, count) << " ms" << endl;
        const blambda a4096 =addStdStrings(4096);
        cout << "Std string test " << time(a4096, 5, count) << " ms" << endl;
    }

    return true;
}

}

int main() {
    const tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::testCopies();
    tuscany::testString();
    tuscany::testStream();
    tuscany::testStringPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
