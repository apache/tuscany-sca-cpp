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

#ifndef tuscany_fstream_hpp
#define tuscany_fstream_hpp

/**
 * File based streams.
 */

#include <stdio.h>
#include <stdarg.h>
#include "string.hpp"
#include "stream.hpp"

namespace tuscany {

/*
 * Output stream backed by a FILE.
 */
class ofstream : public ostream {
public:
    ofstream(const string& path) : file(fopen(c_str(path), "wb")), owner(true) {
    }

    ofstream(FILE* file) : file(file), owner(false) {
    }

    ofstream(const ofstream& os) : file(os.file), owner(false) {
    }

    ~ofstream() {
        if (!owner)
            return;
        if (file == NULL)
            return;
        fclose(file);
    }

    ofstream& vprintf(const char* fmt, ...) {
        va_list args;
        va_start (args, fmt);
        vfprintf (file, fmt, args);
        va_end (args);
        return *this;
    }

    ofstream& write(const string& s) {
        fwrite(c_str(s), 1, length(s), file);
        return *this;
    }

    ofstream& flush() {
        fflush(file);
        return *this;
    }

private:
    FILE* file;
    bool owner;
};

/*
 * Input stream backed by a FILE.
 */
class ifstream : public istream {
public:
    ifstream(const string& path) : file(fopen(c_str(path), "rb")), owner(true) {
    }

    ifstream(FILE* file) : file(file), owner(false) {
    }

    ifstream(const ifstream& is) : file(is.file), owner(false) {
    }

    ~ifstream() {
        if (!owner)
            return;
        if (file == NULL)
            return;
        fclose(file);
    }

    const int read(void* buf, int size) {
        return fread(buf, 1, size, file);
    }

    const bool eof() {
        return feof(file);
    }

    const bool fail() {
        return file == NULL;
    }

    const int get() {
        return fgetc(file);
    }

    const int peek() {
        int c = fgetc(file);
        if (c == -1)
            return c;
        ungetc(c, file);
        return c;
    }

private:
    FILE* file;
    bool owner;
};

/**
 * Standard streams.
 */
ofstream cout(stdout);
ofstream cerr(stderr);
ifstream cin(stdin);

/**
 * Debug log stream.
 */
#ifdef WANT_MAINTAINER_MODE

template<typename V> const bool debug(const V& v, const string& msg) {
    cerr << msg << ": " << v << endl;
    return true;
}

const bool debug(const string& msg) {
    cerr << msg << endl;
    return true;
}

#else

#define debug(v, msg)
#define debug(msg)

#endif

}

#endif /* tuscany_fstream_hpp */
