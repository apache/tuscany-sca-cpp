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

#ifndef tuscany_sstream_hpp
#define tuscany_sstream_hpp

/**
 * Char buffer based streams.
 */

#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
#include "string.hpp"
#include "stream.hpp"
#include "list.hpp"

namespace tuscany {

/**
 * Instrumentable memcpy.
 */
void* stream_memcpy(void* t, const void* s, const size_t n) {
    return memcpy(t, s, n);
}

/**
 * Output stream backed by a char buffer.
 */
class ostringstream : public ostream {
public:
    ostringstream() : len(0) {
    }

    ~ostringstream() {
    }

    ostringstream(const ostringstream& os) {
        len = os.len;
        buf = os.buf;
    }

    ostringstream& vprintf(const char* fmt, ...) {
        va_list args;
        va_start (args, fmt);
        string s;
        s.len = vsnprintf(NULL, 0, fmt, args);
        s.buf = gc_cnew(s.len + 1);
        vsnprintf(s.buf, s.len + 1, fmt, args);
        buf = cons(s, buf);
        len += s.len;
        va_end (args);
        return *this;
    }

    ostringstream& write(const string& s) {
        buf = cons(s, buf);
        len += s.len;
        return *this;
    }

    ostringstream& flush() {
        return *this;
    }

private:
    static const bool strHelper(const list<string> l, char* buf) {
        if (isNil(l))
            return true;
        const string c = car(l);
        char* b = buf - length(c);
        memcpy(b, c_str(c), length(c));
        return strHelper(cdr(l), b);
    }

    const string str() {
        if (isNil(buf))
            return string();
        string s;
        s.len = len;
        s.buf = gc_cnew(s.len + 1);
        strHelper(buf, s.buf + len);
        s.buf[s.len] = '\0';
        return s;
    }

    friend const string str(ostringstream& os);

    int len;
    list<string> buf;
};

/**
 * Return a string representation of a stream.
 */
const string str(ostringstream& os) {
    return os.str();
}

/**
 * Input stream backed by a char buffer
 */
class istringstream : public istream {
public:
    istringstream(const string& s) {
        cur = 0;
        const int slen = length(s);
        len = slen;
        buf = c_str(s);
      }

    ~istringstream() {
    }

    istringstream(const istringstream& is) {
        len = is.len;
        cur = is.cur;
        buf = is.buf;
    }

    const int read(void* b, int size) {
        const int n = len - cur;
        if (n == 0)
            return 0;
        if (n > size) {
            stream_memcpy(b, buf + cur, size);
            cur = cur + size;
            return size;
        }
        stream_memcpy(b, buf + cur, n);
        cur = cur + n;
        return n;
    }

    const bool eof() {
        return cur == len;
    }

    const bool fail() {
        return false;
    }

    const int get() {
        if (eof())
            return -1;
        const int c = buf[cur];
        cur += 1;
        return c;
    }

    const int peek() {
        if (eof())
            return -1;
        return buf[cur];
    }

private:
    int len;
    int cur;
    const char* buf;
};

/**
 * Tokenize a string into a list of strings.
 */
const list<string> tokenize(const char* sep, const string& str) {
    struct nested {
        static const list<string> tokenize(const char* sep, const string& str, const int start = 0) {
            if (start >= length(str))
                return list<string>();
            const int i = find(str, sep, start);
            if (i == length(str))
                return mklist(string(substr(str, start)));
            return cons(string(substr(str, start, i - start)), tokenize(sep, str, i + 1));
        }
    };
    return nested::tokenize(sep, str, 0);
}

/**
 * Returns a lazy list view of an input stream.
 */
struct ilistRead{
    istream &is;
    ilistRead(istream& is) : is(is) {
    }
    const list<string> operator()() {
        char buffer[1024];
        const int n = read(is, buffer, sizeof(buffer));
        if (n ==0)
            return list<string>();
        return cons(string(buffer, n), (*this)());
    }
};

const list<string> streamList(istream& is) {
    return ilistRead(is)();
}

/**
 * Fragment the first element of a list of strings to fit the given max length.
 */
const list<string> fragment(list<string> l, int max) {
    const string s = car(l);
    if (length(s) <= max)
        return l;
    return cons(substr(s, 0, max), cons(substr(s, max), cdr(l)));
}

/**
 * Write a list of strings to an output stream.
 */
ostream& write(const list<string>& l, ostream& os) {
    if(isNil(l))
        return os;
    os << car(l);
    return write(cdr(l), os);
}

}

#endif /* tuscany_sstream_hpp */
