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

#ifndef tuscany_stream_hpp
#define tuscany_stream_hpp

/**
 * Basic stream type and functions.
 */

#include <stdarg.h>
#include "config.hpp"
#include "gc.hpp"
#include "string.hpp"

namespace tuscany {

/**
 * Base output stream.
 */
class ostream {
public:
    virtual ostream& vprintf(const char* fmt, ...) = 0;
    virtual ostream& write(const string& s) = 0;
    virtual ostream& flush() = 0;
};

/**
 * Flush a stream.
 */
ostream& flush(ostream& os) {
    return os.flush();
}

/**
 * Write simple values to a stream.
 */
ostream& operator<<(ostream& os, const char* v) {
    return os.vprintf("%s", v);
}

ostream& operator<<(ostream& os, const unsigned char* v) {
    return os.vprintf("%s", v);
}

ostream& operator<<(ostream& os, const char v) {
    return os.vprintf("%c", v);
}

ostream& operator<<(ostream& os, const int v) {
    return os.vprintf("%d", v);
}

ostream& operator<<(ostream& os, const unsigned int v) {
    return os.vprintf("%u", v);
}

ostream& operator<<(ostream& os, const long int v) {
    return os.vprintf("%ld", v);
}

ostream& operator<<(ostream& os, const long unsigned int v) {
    return os.vprintf("%lu", v);
}

ostream& operator<<(ostream& os, const double v) {
    return os.vprintf("%g", v);
}

ostream& operator<<(ostream& os, const void* v) {
    return os.vprintf("%p", v);
}

ostream& operator<<(ostream& os, const string& v) {
    return os.write(v);
}

class stream_endl {
} endl;

ostream& operator<<(ostream& os, unused const stream_endl e) {
    os.write("\n");
    return os.flush();
}

/*
 * Input stream.
 */
class istream {
public:
    virtual const int read(void* buf, int size) = 0;
    virtual const bool eof() = 0;
    virtual const bool fail() = 0;
    virtual const int get() = 0;
    virtual const int peek() = 0;
};

/**
 * Read from an input stream.
 */
const int read(istream& is, void * buf, int size) {
    return is.read(buf, size);
}

/**
 * Return true if the end of an input stream has been reached.
 */
const bool eof(istream& is) {
    return is.eof();
}

/**
 * Return true if an input stream can't be accessed.
 */
const bool fail(istream& is) {
    return is.fail();
}

/**
 * Read a character from a stream.
 */
const int get(istream& is) {
    return is.get();
}

/**
 * Peek a character from a stream.
 */
const int peek(istream& is) {
    return is.peek();
}

template<typename T> ostream& operator<<(ostream& out, const gc_ptr<T>& p) {
    return out << p.ptr;
}

#ifdef WANT_MAINTAINER_MODE

/**
 * Debug stream implementation with no dependencies on anything else.
 */
class odebugstream : public ostream {
public:
    odebugstream() {
    }

    odebugstream& vprintf(const char* fmt, ...) {
        va_list args;
        va_start (args, fmt);
        string s;
        s.len = vsnprintf(NULL, 0, fmt, args);
        s.buf = gc_cnew(s.len + 1);
        vsnprintf(s.buf, s.len + 1, fmt, args);
        buf = buf + s;
        va_end (args);
        return *this;
    }

    odebugstream& write(const string& s) {
        buf = buf + s;
        return *this;
    }

    odebugstream& flush() {
        return *this;
    }

private:
    friend const string str(odebugstream& os);

    string buf;
};

const string str(odebugstream& os) {
    return os.buf;
}

#endif

}

#endif /* tuscany_stream_hpp */
