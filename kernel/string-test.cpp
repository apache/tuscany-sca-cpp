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

bool testCopies() {
    resetStringCopyCounters();
    string x("abcd");
    assert(checkStringCopyCounters(1));
    resetStringCopyCounters();
    string y = string("abcd");
    assert(checkStringCopyCounters(1));
    resetStringCopyCounters();
    string z = y;
    assert(checkStringCopyCounters(0));
    resetStringCopyCounters();
    const list<string> pl = list<string>() + "abcd" + "efgh";
    printStringCopyCounters();
    resetStringCopyCounters();
    const list<string> cl = cons<string>("efgh", mklist<string>("abcd"));
    printStringCopyCounters();
    return true;
}

bool testString() {
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
    assert(substr("abcdef", 4) == "ef");
    assert(substr("abcdef", 4, 2) == "ef");
    assert(substr("abcdef", 4, 3) == "ef");
    assert(substr("abcdef", 6, 3) == "");
    return true;
}

bool testStream() {
    ostringstream os;
    os << "ab" << "cd";
    cout << str(os) << endl;
    assert(str(os) == "abcd");

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

std::string stdAdd(std::string& x, std::string& y) {
    return x + y;
}

string add(string& x, string& y) {
    return x + y;
}

char charBuffer[16384];

struct addStrings{
    const int size;
    addStrings(const int size) : size(size) {
    }
    bool operator()() const {
        const int sz = size / 4;
        string x(charBuffer, sz);
        string y(charBuffer, sz);
        assert(length(add(x, y)) == sz * 2);
        return true;
    }
};

struct addStdStrings{
    const int size;
    addStdStrings(const int size) : size(size) {
    }
    bool operator()() const {
        const int sz = size / 4;
        std::string x(charBuffer, sz);
        std::string y(charBuffer, sz);
        assert(stdAdd(x, y).length() == (unsigned int)(sz * 2));
        return true;
    }
};

bool testStringPerf() {
    memset(charBuffer, 'A', 16384);
    charBuffer[16384] = '\0';

    const int count = 100000;
    {
        const lambda<bool()> a16 = addStrings(16);
        cout << "string test " << time(a16, 5, count) << " ms" << endl;
        const lambda<bool()> a32 =addStrings(32);
        cout << "string test " << time(a32, 5, count) << " ms" << endl;
        const lambda<bool()> a256 =addStrings(256);
        cout << "string test " << time(a256, 5, count) << " ms" << endl;
        const lambda<bool()> a1024 =addStrings(1024);
        cout << "string test " << time(a1024, 5, count) << " ms" << endl;
        const lambda<bool()> a4096 =addStrings(4096);
        cout << "string test " << time(a4096, 5, count) << " ms" << endl;
    }
    {
        const lambda<bool()> a16 =addStdStrings(16);
        cout << "Std string test " << time(a16, 5, count) << " ms" << endl;
        const lambda<bool()> a32 =addStdStrings(32);
        cout << "Std string test " << time(a32, 5, count) << " ms" << endl;
        const lambda<bool()> a256 =addStdStrings(256);
        cout << "Std string test " << time(a256, 5, count) << " ms" << endl;
        const lambda<bool()> a1024 =addStdStrings(1024);
        cout << "Std string test " << time(a1024, 5, count) << " ms" << endl;
        const lambda<bool()> a4096 =addStdStrings(4096);
        cout << "Std string test " << time(a4096, 5, count) << " ms" << endl;
    }

    return true;
}

}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::testCopies();
    tuscany::testString();
    tuscany::testStream();
    tuscany::testStringPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
