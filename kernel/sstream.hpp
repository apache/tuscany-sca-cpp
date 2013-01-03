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
inline void* stream_memcpy(void* t, const void* s, const size_t n) {
    return memcpy(t, s, n);
}

/**
 * Write a list of strings into a buffer.
 */
const bool writeList(const list<string>& l, char* const buf) {
    if (isNull(l))
        return true;
    const string c = car(l);
    char* b = buf - length(c);
    memcpy(b, c_str(c), length(c));
    return writeList(cdr(l), b);
}

/**
 * Output stream backed by a char buffer.
 */
class ostringstream : public ostream {
public:
    inline ostringstream() : len(0), accumbuf(NULL), accumlen(0), buf() {
    }

    inline ~ostringstream() {
    }

    inline ostringstream(const ostringstream& os) : len(os.len), accumbuf(os.accumbuf), accumlen(os.accumlen), buf(os.buf) {
    }

    inline ostringstream& vprintf(const char* const fmt, ...) {
        va_list args;
        va_start (args, fmt);
        const size_t l = vsnprintf(NULL, 0, fmt, args);
        if (l <= 256) {
            char c[256];
            va_start (args, fmt);
            vsnprintf(c, l + 1, fmt, args);
            va_end (args);
            accumulate(c, l);
            return *this;
        }
        spill();
        char* const sbuf = gc_cnew(l + 1);
        va_start (args, fmt);
        vsnprintf(sbuf, l + 1, fmt, args);
        va_end (args);
        buf = cons(string(sbuf, l, false), (const list<string>)buf);
        len += l;
        return *this;
    }

    inline ostringstream& write(const string& s) {
        if (s.len <= 256) {
            accumulate(s.buf, s.len);
            return *this;
        }
        spill();
        buf = cons(s, (const list<string>)buf);
        len += s.len;
        return *this;
    }

    inline ostringstream& write(const char c) {
        accumulate(&c, 1);
        return *this;
    }

    inline ostringstream& flush() {
        spill();
        return *this;
    }

private:
    inline const string str() {
        spill();
        if (isNull((const list<string>)buf))
            return emptyString;
        char* const sbuf = gc_cnew(len + 1);
        writeList(buf, sbuf + len);
        sbuf[len] = '\0';
        return string(sbuf, len, false);
    }

    inline const bool accumulate(const char* const c, const size_t l) {
        if (accumbuf == NULL)
            accumbuf = gc_cnew(65);
        for(size_t i = 0; i < l; i++) {
            accumbuf[accumlen] = c[i];
            accumlen++;
            if (accumlen == 64) {
                accumbuf[accumlen] = '\0';
                buf = cons(string(accumbuf, accumlen, false), (const list<string>)buf);
                accumbuf = i < l? gc_cnew(65) : NULL;
                accumlen = 0;
            }
        }
        accumbuf[accumlen] = '\0';
        len += l;
        return true;
    }

    inline const bool spill() {
        if (accumbuf == NULL)
            return true;
        buf = cons(string(accumbuf, accumlen), (const list<string>)buf);
        accumbuf = NULL;
        accumlen = 0;
        return true;
    }

    friend const string str(ostringstream& os);

    size_t len;
    char* accumbuf;
    size_t accumlen;
    gc_mutable_ref<list<string> > buf;
};

/**
 * Return a string representation of a stream.
 */
inline const string str(ostringstream& os) {
    return os.str();
}

/**
 * Input stream backed by a char buffer
 */
class istringstream : public istream {
public:
    inline istringstream(const string& s) : len(length(s)), cur(0), buf(c_str(s)) {
    }

    inline ~istringstream() {
    }

    inline istringstream(const istringstream& is) : len(is.len), cur(is.cur), buf(is.buf) {
    }

    inline const size_t read(void* const b, const size_t size) {
        const size_t n = len - cur;
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

    inline const bool eof() {
        return cur == len;
    }

    inline const bool fail() {
        return false;
    }

    inline const int get() {
        if (cur == len)
            return -1;
        return buf[cur++];
    }

    inline const int peek() {
        if (cur == len)
            return -1;
        return buf[cur];
    }

private:
    const size_t len;
    size_t cur;
    const char* buf;
};

/**
 * Tokenize a string into a list of strings.
 */
inline const list<string> tokenize(const char* const sep, const string& str) {
    const lambda<const list<string>(const char* const, const size_t, const string&, const size_t)> tokenize = [&tokenize](const char* const sep, const size_t slen, const string& str, const size_t start) -> const list<string> {
        if (start >= length(str))
            return list<string>();
        const size_t i = find(str, sep, start);
        if (i == length(str))
            return mklist(string(substr(str, start)));
        return cons(string(substr(str, start, i - start)), tokenize(sep, slen, str, i + slen));
    };
    return tokenize(sep, strlen(sep), str, 0);
}

/**
 * Join a list of strings into a single string.
 */
inline const string join(const char* const sep, const list<string>& l) {
    ostringstream os;
    const lambda<ostringstream&(const char* const, const list<string>&, ostringstream&)> join = [&join](const char* const sep, const list<string>& l, ostringstream& os) -> ostringstream& {
        if (isNull(l))
            return os;
        os << car(l);
        if (!isNull(cdr(l)))
            os << sep;
        return join(sep, cdr(l), os);
    };
    return str(join(sep, l, os));
}

/**
 * Returns a lazy list view of an input stream.
 */
inline const list<string> streamList(istream& is) {
    const lambda<const list<string>()> ilistRead = [&is, &ilistRead]() -> const list<string> {
        char buffer[1024];
        const size_t n = read(is, buffer, sizeof(buffer));
        if (n ==0)
            return list<string>();
        return cons(string(buffer, n), ilistRead());
    };
    return ilistRead();
}

/**
 * Fragment the first element of a list of strings to fit the given max length.
 */
inline const list<string> fragment(const list<string>& l, const size_t max) {
    const string s = car(l);
    if (length(s) <= max)
        return l;
    return cons(substr(s, 0, max), cons(substr(s, max), cdr(l)));
}

/**
 * Write a list of strings to an output stream.
 */
inline ostream& write(const list<string>& l, ostream& os) {
    if(isNull(l))
        return os;
    os << car(l);
    return write(cdr(l), os);
}

/**
 * Convert a list of strings to a string.
 */
inline const string write(const list<string>& l) {
    if (isNull(l))
        return emptyString;
    if (isNull(cdr(l)))
        return car(l);
    ostringstream os;
    write(l, os);
    return str(os);
}

}

#endif /* tuscany_sstream_hpp */
