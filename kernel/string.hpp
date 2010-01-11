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

#ifndef tuscany_string_hpp
#define tuscany_string_hpp

/**
 * Simple and fast string type backed by a char buffer
 */

#include <assert.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include "gc.hpp"

namespace tuscany {

#ifdef WANT_MAINTAINER_MODE

/**
 * Debug utilities. Counters used to track string copies.
 */
long countStringCopies = 0;

bool resetStringCopyCounters() {
    countStringCopies = 0;
    return true;
}

bool checkStringCopyCounters(long c) {
    return countStringCopies == c;
}

bool printStringCopyCounters() {
    printf("countStringCopies %ld\n", countStringCopies);
    return true;
}

#else

#define resetStringCopyCounters()
#define checkStringCopyCounters(c) true
#define printStringCopyCounters()

#endif

/**
 * Instrumented memcpy.
 */
void* string_memcpy(void* t, const void* s, const size_t n) {
#ifdef WANT_MAINTAINER_MODE
    countStringCopies += 1;
#endif
    return memcpy(t, s, n);
}

char stringEmptyBuffer[1] = { '\0' };

/**
 * String class. The maximum string size is specified as a template parameter.
 */
class string {
public:
    string() : len(0) {
        buf = stringEmptyBuffer;
    }

    string(const char* s) {
        len = strlen(s);
        if (len == 0) {
            buf = stringEmptyBuffer;
            return;
        }
        buf = gc_cnew(len + 1);
        string_memcpy(buf, s, len + 1);
    }

    string(const char* s, const int n) {
        len = n;
        if (len == 0) {
            buf = stringEmptyBuffer;
            return;
        }
        buf = gc_cnew(len + 1);
        string_memcpy(buf, s, len);
        buf[len] = '\0';
    }

    string(const int n, const char c) {
        len = n;
        if (len == 0) {
            buf = stringEmptyBuffer;
            return;
        }
        buf = gc_cnew(len + 1);
        memset(buf, c, n);
        buf[len] = '\0';
    }

    string(const string& s) {
        len = s.len;
        buf = s.buf;
    }

    const string& operator=(const string& s) {
        if (&s == this)
            return *this;
        len = s.len;
        buf = s.buf;
        return *this;
    }

    const bool operator==(const string& s) const {
        if (len != s.len)
            return false;
        if (buf == s.buf)
            return true;
        return memcmp(buf, s.buf, len) == 0;
    }

    const bool operator!=(const string& s) const {
        return !(*this == s);
    }

    const bool operator==(const char* s) const {
        if (buf == s)
            return true;
        return strcmp(buf, s) == 0;
    }

    const bool operator!=(const char* s) const {
        return !(*this == s);
    }

    const bool operator<(const string& s) const {
        const int n = len < s.len? len : s.len;
        const int c = memcmp(buf, s.buf, n);
        if (c < 0)
            return true;
        if (c == 0)
            return len < s.len;
        return false;
    }

    const bool operator>(const string& s) const {
        const int n = len < s.len? len : s.len;
        int c = memcmp(buf, s.buf, n);
        if (c > 0)
            return true;
        if (c == 0)
            return len > s.len;
        return false;
    }

private:
#ifdef WANT_MAINTAINER_MODE
    friend class odebugstream;
#endif
    friend class ostringstream;
    friend const string operator+(const string& a, const string& b);
    friend const string operator+(const string& a, const char* b);
    friend const int length(const string& s);
    friend const char* c_str(const string& s);
    friend const int find(const string& s1, const char* s2, const int start);
    friend const string substr(const string& s, const int pos, const int n);

    int len;
    char* buf;
};

/**
 * Adds two strings.
 */
const string operator+(const string& a, const string& b) {
    string s;
    s.len = a.len + b.len;
    s.buf = gc_cnew(s.len + 1);
    string_memcpy(s.buf, a.buf, a.len);
    string_memcpy(s.buf + a.len, b.buf, b.len);
    s.buf[s.len] = '\0';
    return s;
}

const string operator+(const string& a, const char* b) {
    string s;
    const int blen = strlen(b);
    s.len = a.len + blen;
    s.buf = gc_cnew(s.len + 1);
    string_memcpy(s.buf, a.buf, a.len);
    string_memcpy(s.buf + a.len, b, blen);
    s.buf[s.len] = '\0';
    return s;
}

/**
 * Returns the length of a string.
 */
const int length(const string& s) {
    return s.len;
}

/**
 * Returns a string as a C zero terminated string.
 */
const char* c_str(const string& s) {
    return s.buf;
}

/**
 * Find the first occurrence of string s2 in s1, starting at the given position.
 */
const int find(const string& s1, const char* s2, const int start) {
    if (start >= s1.len)
        return s1.len;
    const char *f = strstr(s1.buf + start, s2);
    if (f == NULL)
        return s1.len;
    return f - s1.buf;
}

const int find(const string& s1, const char* s2) {
    return find(s1, s2, 0);
}

const bool contains(const string& s1, const char* s2) {
    return find(s1, s2) != length(s1);
}

/**
 * Find the first occurence of any character from a string in a string.
 */
const int find_first_of(const string& s1, const string& s2) {
    return strcspn(c_str(s1), c_str(s2));
}

/**
 * Find the last occurence of a character in a string.
 */
const int find_last(const string& s, const char c) {
    const char* cs = c_str(s);
    const char* f = strrchr(cs, c);
    if (f == NULL)
        return length(s);
    return f - cs;
}

/**
 * Return a substring of a string.
 */
const string substr(const string& s, const int pos, const int n) {
    if (pos >= s.len)
        return string();
    if (pos + n > s.len)
        return string(s.buf + pos, s.len - pos);
    return string(s.buf + pos, n);
}

const string substr(const string& s, const int pos) {
    return substr(s, pos, length(s));
}

/**
 * Common string constants.
 */

string trueString("true");
string falseString("false");
string emptyString("");

}

#endif /* tuscany_string_hpp */
