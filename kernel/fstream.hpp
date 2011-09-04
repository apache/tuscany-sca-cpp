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
#include <time.h>
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

    const bool fail() {
        return file == NULL;
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

    const size_t read(void* buf, size_t size) {
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
 * Streams used for logging.
 */

/**
 * Format the current time.
 */
const string logTime() {
    const time_t t = ::time(NULL);
    const tm* lt = localtime(&t);
    char ft[32];
    strftime(ft, 31, "%a %b %d %H:%M:%S %Y", lt);
    return ft;
}

/*
 * Log stream.
 */
class logfstream : public ostream {
public:
    logfstream(FILE* file, const string& type) : file(file), type(type), owner(false), head(false) {
    }

    logfstream(const logfstream& os) : file(os.file), type(os.type), owner(false), head(os.head) {
    }

    ~logfstream() {
        if (!owner)
            return;
        if (file == NULL)
            return;
        fclose(file);
    }

    logfstream& vprintf(const char* fmt, ...) {
        whead();
        va_list args;
        va_start (args, fmt);
        vfprintf (file, fmt, args);
        va_end (args);
        return *this;
    }

    logfstream& write(const string& s) {
        whead();
        fwrite(c_str(s), 1, length(s), file);
        if (s == "\n")
            head = false;
        return *this;
    }

    logfstream& flush() {
        fflush(file);
        return *this;
    }

private:
    FILE* file;
    const string type;
    bool owner;
    bool head;

    logfstream& whead() {
        if (head)
            return *this;
        head = true;
        *this << "[" << logTime() << "] [" << type << "] ";
        return *this;
    }
};

/**
 * Info and failure log streams.
 */
logfstream cinfo(stderr, "info");
logfstream cfailure(stderr, "error");

#ifdef WANT_MAINTAINER_MODE

/**
 * Debug log stream and debug functions.
 */
logfstream cdebug(stderr, "debug");

/**
 * Log a debug message.
 */
const bool debugLog(const string& msg) {
    gc_scoped_pool();
    cdebug << msg << endl;
    return true;
}

/**
 * Log a debug message and a value.
 */
template<typename V> const bool debugLog(const V& v, const string& msg) {
    gc_scoped_pool();
    cdebug << msg << ": " << v << endl;
    return true;
}

#define debug(...) tuscany::debugLog(__VA_ARGS__)

#else

#define debug(...)

#endif

}

#endif /* tuscany_fstream_hpp */
