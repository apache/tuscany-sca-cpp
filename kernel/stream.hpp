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
    virtual ostream& vprintf(const char* const fmt, ...) = 0;
    virtual ostream& write(const string& s) = 0;
    virtual ostream& write(const char c) = 0;
    virtual ostream& flush() = 0;
};

/**
 * Flush a stream.
 */
inline ostream& flush(ostream& os) {
    return os.flush();
}

/**
 * Write simple values to a stream.
 */
inline ostream& operator<<(ostream& os, const char* const v) {
    return os.vprintf("%s", v);
}

inline ostream& operator<<(ostream& os, const unsigned char* const v) {
    return os.vprintf("%s", v);
}

inline ostream& operator<<(ostream& os, const char v) {
    return os.write(v);
}

inline ostream& operator<<(ostream& os, const int v) {
    return os.vprintf("%d", v);
}

inline ostream& operator<<(ostream& os, const unsigned int v) {
    return os.vprintf("%u", v);
}

inline ostream& operator<<(ostream& os, const long int v) {
    return os.vprintf("%ld", v);
}

inline ostream& operator<<(ostream& os, const long unsigned int v) {
    return os.vprintf("%lu", v);
}

inline ostream& operator<<(ostream& os, const double v) {
    return os.vprintf("%.16g", v);
}

inline ostream& operator<<(ostream& os, const void* const v) {
    return os.vprintf("%p", v);
}

inline ostream& operator<<(ostream& os, const string& v) {
    return os.write(v);
}

class stream_endl {
} endl;

inline ostream& operator<<(ostream& os, unused const stream_endl e) {
    os.write('\n');
    return os.flush();
}

/*
 * Input stream.
 */
class istream {
public:
    virtual const size_t read(void* const buf, const size_t size) = 0;
    virtual const bool eof() = 0;
    virtual const bool fail() = 0;
    virtual const int get() = 0;
    virtual const int peek() = 0;
};

/**
 * Read from an input stream.
 */
inline const size_t read(istream& is, void* const buf, const size_t size) {
    return is.read(buf, size);
}

/**
 * Return true if the end of an input stream has been reached.
 */
inline const bool eof(istream& is) {
    return is.eof();
}

/**
 * Return true if an input stream can't be accessed.
 */
inline const bool fail(istream& is) {
    return is.fail();
}

/**
 * Read a character from a stream.
 */
inline const int get(istream& is) {
    return is.get();
}

/**
 * Peek a character from a stream.
 */
inline const int peek(istream& is) {
    return is.peek();
}

#ifndef WANT_RAW_PTR

/**
 * Write a pointer.
 */
template<typename T> inline ostream& operator<<(ostream& out, const gc_ptr<T>& p) {
    return out << (T*)p;
}

/**
 * Write a mutable pointer.
 */
template<typename T> inline ostream& operator<<(ostream& out, const gc_mutable_ptr<T>& p) {
    return out << (T*)p;
}

#endif

/**
 * Write a mutable reference.
 */
template<typename T> inline ostream& operator<<(ostream& out, const gc_mutable_ref<T>& r) {
    return out << (T)r;
}

#ifdef WANT_MAINTAINER_LOG

/**
 * Debug stream implementation with no dependencies on anything else.
 */
class odebugstream : public ostream {
public:
    inline odebugstream() : buf() {
    }

    inline odebugstream& vprintf(const char* const fmt, ...) {
        va_list args;
        string s;
        va_start (args, fmt);
        const size_t slen = vsnprintf(NULL, 0, fmt, args);
        char* const sbuf = gc_cnew(slen + 1);
        va_start (args, fmt);
        vsnprintf(sbuf, slen + 1, fmt, args);
        buf = buf + string(sbuf, slen, false);
        va_end (args);
        return *this;
    }

    inline odebugstream& write(const string& s) {
        buf = buf + s;
        return *this;
    }

    inline odebugstream& write(const char c) {
        buf = buf + c;
        return *this;
    }

    inline odebugstream& flush() {
        return *this;
    }

private:
    friend const string str(odebugstream& os);

    gc_mutable_ref<string> buf;
};

const string str(odebugstream& os) {
    return os.buf;
}

#endif

}

#endif /* tuscany_stream_hpp */
