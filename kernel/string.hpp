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

#ifdef WANT_MAINTAINER_COUNTERS

/**
 * Debug utilities. Counters used to track string copies.
 */
long countStringCopies = 0;

inline const bool resetStringCopyCounters() {
    countStringCopies = 0;
    return true;
}

inline const bool checkStringCopyCounters(long c) {
    return countStringCopies == c;
}

inline const bool printStringCopyCounters() {
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
#ifdef WANT_MAINTAINER_COUNTERS

inline void* string_memcpy(void* t, const void* s, const size_t n) {
    countStringCopies += 1;
    return memcpy(t, s, n);
}

#else

#define string_memcpy(t, s, n) memcpy(t, s, n)

#endif

char stringEmptyBuffer[1] = { '\0' };

/**
 * String class. The maximum string size is specified as a template parameter.
 */
class string {
public:
    inline string() noexcept : len(0), buf(stringEmptyBuffer) {
    }

    inline string(const char* const s) noexcept : len(strlen(s)), buf(mksbuf(s, len)) {
    }

    inline string(const char* const s, const size_t n) noexcept : len(n), buf(mksbuf(s, n)) {
    }

    inline string(const char* const s, const size_t n, const bool copy) noexcept : len(n), buf(mksbuf(s, n, copy)) {
    }

    inline string(const size_t n, const char c) noexcept : len(n), buf(mkcbuf(c, n)) {
    }

    inline string(const string& s) noexcept : len(s.len), buf(s.buf) {
    }

    string& operator=(const string& s) = delete;

    inline const bool operator==(const string& s) const noexcept {
        if (len != s.len)
            return false;
        if (buf == s.buf)
            return true;
        return memcmp(buf, s.buf, len) == 0;
    }

    inline const bool operator!=(const string& s) const noexcept {
        return !(*this == s);
    }

    inline const bool operator==(const char* const s) const noexcept {
        if (buf == s)
            return true;
        return strcmp(buf, s) == 0;
    }

    inline const bool operator!=(const char* const s) const noexcept {
        return !(*this == s);
    }

    inline const bool operator<(const string& s) const noexcept {
        const size_t n = len < s.len? len : s.len;
        const int c = memcmp(buf, s.buf, n);
        if (c < 0)
            return true;
        if (c == 0)
            return len < s.len;
        return false;
    }

    inline const bool operator>(const string& s) const noexcept {
        const size_t n = len < s.len? len : s.len;
        const int c = memcmp(buf, s.buf, n);
        if (c > 0)
            return true;
        if (c == 0)
            return len > s.len;
        return false;
    }

private:
#ifdef WANT_MAINTAINER_LOG
    friend class odebugstream;
#endif
    friend class ostringstream;
    friend const string operator+(const string& a, const string& b) noexcept;
    friend const string operator+(const string& a, const char* const b) noexcept;
    friend const string operator+(const string& a, const char c) noexcept;
    friend const size_t length(const string& s) noexcept;
    friend const char* const c_str(const string& s) noexcept;
    friend const size_t find(const string& s1, const char* const s2, const size_t start) noexcept;
    friend const string substr(const string& s, const size_t pos, const size_t n) noexcept;
    friend const bool setstring(string& target, const string& s);

    const size_t len;
    const char* const buf;

    const char* const mkbuf(const size_t n) {
        if (n == 0)
            return stringEmptyBuffer;
        char* const b = gc_cnew(n + 1);
        b[n] = '\0';
        return b;
    }

    const char* const mksbuf(const char* const s, const size_t n) {
        if (n == 0)
            return stringEmptyBuffer;
        char* const b = gc_cnew(n + 1);
        string_memcpy(b, s, n);
        b[n] = '\0';
        return b;
    }

    const char* const mksbuf(const char* const s, const size_t n, const bool copy) {
        if (!copy)
            return s;
        if (n == 0)
            return stringEmptyBuffer;
        char* const b = gc_cnew(n + 1);
        string_memcpy(b, s, n);
        b[n] = '\0';
        return b;
    }

    const char* const mkcbuf(const char c, const size_t n) {
        if (n == 0)
            return stringEmptyBuffer;
        char* const b = gc_cnew(n + 1);
        memset(b, c, n);
        b[n] = '\0';
        return b;
    }
};

/**
 * Adds two strings.
 */
inline const string operator+(const string& sa, const string& sb) noexcept {
    const size_t n = sa.len + sb.len;
    char* const b = gc_cnew(n + 1);
    string_memcpy(b, sa.buf, sa.len);
    string_memcpy(b + sa.len, sb.buf, sb.len);
    b[n] = '\0';
    return string(b, n, false);
}

inline const string operator+(const string& sa, const char* const sb) noexcept {
    const size_t bn = strlen(sb);
    const size_t n = sa.len + bn;
    char* const b = gc_cnew(n + 1);
    string_memcpy(b, sa.buf, sa.len);
    string_memcpy(b + sa.len, sb, bn);
    b[n] = '\0';
    return string(b, n, false);
}

inline const string operator+(const string& sa, const char c) noexcept {
    const size_t n = sa.len + 1;
    char* const b = gc_cnew(n + 1);
    string_memcpy(b, sa.buf, sa.len);
    b[n - 1] = c;
    b[n] = '\0';
    return string(b, n, false);
}

/**
 * Returns the length of a string.
 */
inline const size_t length(const string& s) noexcept {
    return s.len;
}

/**
 * Returns a string as a C zero terminated string.
 */
inline const char* const c_str(const string& s) noexcept {
    return s.buf;
}

/**
 * Find the first occurrence of string s2 in s1, starting at the given position.
 */
inline const size_t find(const string& s1, const char* const s2, const size_t start) noexcept {
    if (start >= s1.len)
        return s1.len;
    const char* const f = strstr(s1.buf + start, s2);
    if (f == NULL)
        return s1.len;
    return f - s1.buf;
}

inline const size_t find(const string& s1, const char* const s2) noexcept {
    return find(s1, s2, 0);
}

/**
 * Return true if string s1 contains s2.
 */
inline const bool contains(const string& s1, const char* const s2) noexcept {
    return find(s1, s2) != length(s1);
}

/**
 * Find the first occurence of any character from a string in a string.
 */
inline const size_t find_first_of(const string& s1, const string& s2) noexcept {
    return strcspn(c_str(s1), c_str(s2));
}

/**
 * Find the first occurence of a character in a string.
 */
inline const size_t find(const string& s, const char c) noexcept {
    const char* const cs = c_str(s);
    const char* const f = strchr(cs, c);
    if (f == NULL)
        return length(s);
    return f - cs;
}

/**
 * Find the last occurence of a character in a string.
 */
inline const size_t find_last(const string& s, const char c) noexcept {
    const char* const cs = c_str(s);
    const char* const f = strrchr(cs, c);
    if (f == NULL)
        return length(s);
    return f - cs;
}

/**
 * Return a substring of a string.
 */
inline const string substr(const string& s, const size_t pos, const size_t n) noexcept {
    if (pos >= s.len)
        return string();
    if (pos + n > s.len)
        return string(s.buf + pos, s.len - pos);
    return string(s.buf + pos, n);
}

inline const string substr(const string& s, const size_t pos) noexcept {
    return substr(s, pos, length(s));
}

/**
 * Set a string. Use with moderation.
 */
/*
inline const bool setstring(string& target, const string& s) {
    if (&target == &s)
        return true;
    memcpy((void*)&target.len, (void*)&s.len, sizeof(size_t));
    memcpy((void*)&target.buf, (void*)&s.buf, sizeof(char*));
    return true;
}
*/

/**
 * Common string constants.
 */
const string trueString("true");
const string falseString("false");
const string emptyString("");

}

#endif /* tuscany_string_hpp */
