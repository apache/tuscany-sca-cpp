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

#ifdef WANT_HTTPD_LOG
#include <apr_strings.h>
#include <apr_fnmatch.h>
#include <apr_lib.h>
#define APR_WANT_STRFUNC
#include <apr_want.h>
#include <apr_base64.h>

#include <httpd.h>
// Hack to workaround compile error with CLang/LLVM
#undef strtoul
// Hack to workaround compile error with HTTPD 2.3.8
#define new new_
#include <http_config.h>
#undef new
#include <http_main.h>
#include <http_log.h>

#else

#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef WANT_THREADS
#include <pthread.h>
#endif

#endif

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

#ifdef WANT_HTTPD_LOG

/*
 * HTTPD-based log stream.
 */
class loghstream : public ostream {
public:
    loghstream(const int level) : level(level), len(0) {
    }

    ~loghstream() {
    }

    ostream& vprintf(const char* fmt, ...) {
        va_list args;
        va_start (args, fmt);
        const int l = vsnprintf(buf + len, (sizeof(buf) - 1) - len, fmt, args);
        va_end (args);
        len += l;
        if (len > (int)(sizeof(buf) - 1))
            len = sizeof(buf) - 1;
        return *this;
    }

    ostream& write(const string& s) {
        if (s != "\n")
            return this->vprintf("%s", c_str(s));
        buf[len] = '\0';
        ap_log_error(NULL, 0, -1, level, 0, ap_server_conf, "%s", buf);
        len = 0;
        return *this;
    }

    ostream& flush() {
        return *this;
    }

private:
    const int level;
    int len;
    char buf[2049];
};

/**
 * Info and failure log streams.
 */
loghstream cinfo(APLOG_INFO);
loghstream cfailure(APLOG_ERR);

#ifdef WANT_MAINTAINER_LOG

/**
 * Debug log stream.
 */
loghstream cdebug(APLOG_DEBUG);

/**
 * Return true if debug log is enabled.
 */
#define debug_islogging() (bool)(APLOG_MODULE_IS_LEVEL(ap_server_conf, APLOG_NO_MODULE, APLOG_DEBUG))

#endif

#else

/**
 * Format the current time.
 */
const string logTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    const time_t t = tv.tv_sec;
    const tm* lt = localtime(&t);
    char ft[32];
    strftime(ft, 20, "%a %b %d %H:%M:%S", lt);
    sprintf(ft + 19, ".%06lu ", (unsigned long)tv.tv_usec);
    strftime(ft + 27, 5, "%Y", lt);
    return ft;
}

/*
 * File-based log stream.
 */
class logfstream : public ostream {
public:
    logfstream(FILE* file, const string& type) : file(file), type(type), head(false) {
    }

    logfstream(const logfstream& os) : file(os.file), type(os.type), head(os.head) {
    }

    ~logfstream() {
    }

    ostream& vprintf(const char* fmt, ...) {
        whead();
        va_list args;
        va_start (args, fmt);
        vfprintf (file, fmt, args);
        va_end (args);
        return *this;
    }

    ostream& write(const string& s) {
        whead();
        fwrite(c_str(s), 1, length(s), file);
        if (s == "\n")
            head = false;
        return *this;
    }

    ostream& flush() {
        fflush(file);
        return *this;
    }

private:
    FILE* file;
    const string type;
    bool head;

    const unsigned long tid() const {
#ifdef WANT_THREADS
        return (unsigned long)pthread_self();
#else
        return 0;
#endif
    }

    ostream& whead() {
        if (head)
            return *this;
        head = true;
        *this << "[" << logTime() << "] [" << type << "] [pid " << (unsigned long)getpid() << ":tid " << tid() << "] ";
        return *this;
    }
};

/**
 * Info and failure log streams.
 */
logfstream cinfo(stderr, "info");
logfstream cfailure(stderr, "error");

#ifdef WANT_MAINTAINER_LOG

/**
 * Debug log stream.
 */
logfstream cdebug(stderr, "debug");

/**
 * Return true if debug log is enabled.
 */
#define debug_islogging() true

#endif

#endif

#ifdef WANT_MAINTAINER_LOG

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

#define debug(...) if (debug_islogging()) tuscany::debugLog(__VA_ARGS__)

#else

#define debug_islogging() false
#define debug(...)

#endif

}

#endif /* tuscany_fstream_hpp */
