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

#ifndef tuscany_slist_hpp
#define tuscany_slist_hpp

/**
 * Useful functions to work with lists of strings and character streams.
 */

#include <iostream>
#include <string>
#include "function.hpp"
#include "list.hpp"

namespace tuscany {

/**
 * Tokenize a string into a list of strings.
 */
const list<std::string> tokenize(const std::string& sep, const std::string& str) {
    struct nested {
        static const list<std::string> tokenize(const std::string& sep, const std::string& str, const unsigned int start = 0) {
            if (start >= str.length())
                return list<std::string>();
            const unsigned int i = str.find_first_of(sep, start);
            if (i == std::string::npos)
                return makeList(str.substr(start));
            return cons(str.substr(start, i - start), tokenize(sep, str, i + 1));
        }
    };
    return nested::tokenize(sep, str, 0);
}

/**
 * Returns a lazy list view of an input stream.
 */
struct ilistRead{
    std::istream &is;
    ilistRead(std::istream& is) : is(is) {
    }
    const list<std::string> operator()() {
        char buffer[1024];
        is.read(buffer, 1024);
        const int n = is.gcount();
        if (n ==0)
            return list<std::string>();
        return cons(std::string(buffer, n), (*this)());
    }
};

const list<std::string> makeStreamList(std::istream& is) {
    return ilistRead(is)();
}

/**
 * Fragment the first element of a list of strings to fit the given max length.
 */
const list<std::string> fragment(list<std::string> l, unsigned int max) {
    const std::string s = car(l);
    if (s.length() <= max)
        return l;
    return cons(s.substr(0, max), cons(s.substr(max), cdr(l)));
}

/**
 * Write a list of strings to an output stream.
 */
std::ostream& write(const list<std::string>& l, std::ostream& os) {
    if(isNil(l))
        return os;
    os << car(l);
    return write(cdr(l), os);
}

}

#endif /* tuscany_slist_hpp */
