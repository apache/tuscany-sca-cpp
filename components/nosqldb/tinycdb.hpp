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

#ifndef tuscany_tinycdb_hpp
#define tuscany_tinycdb_hpp

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cdb.h>

#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../../modules/scheme/eval.hpp"

namespace tuscany {
namespace tinycdb {

/**
 * A reallocatable buffer.
 */
class buffer {
public:
    operator void*() const throw() {
        return buf;
    }

    operator unsigned char*() const throw() {
        return (unsigned char*)buf;
    }

    operator char*() const throw() {
        return (char*)buf;
    }

private:
    buffer(const unsigned int size, void* buf) : size(size), buf(buf) {
    }

    unsigned int size;
    void* buf;

    friend const buffer mkbuffer(const unsigned int sz);
    friend const buffer mkbuffer(const buffer& b, const unsigned int newsz);
    friend const bool free(const buffer& b);
};

/**
 * Make a new buffer.
 */
const buffer mkbuffer(const unsigned int sz) {
    return buffer(sz, malloc(sz));
}

/**
 * Make a new buffer by reallocating an existing one.
 */
const buffer mkbuffer(const buffer& b, const unsigned int sz) {
    if (sz <= b.size)
        return b;
    return buffer(sz, realloc(b.buf, sz));
}

/**
 * Free a buffer.
 */
const bool free(const buffer&b) {
    ::free(b.buf);
    return true;
}

/**
 * Represents a TinyCDB connection.
 */
class TinyCDB {
public:
    TinyCDB() : owner(false), fd(-1) {
        st.st_ino = 0;
    }

    TinyCDB(const string& name) : owner(true), name(name), fd(-1) {
        st.st_ino = 0;
    }

    TinyCDB(const TinyCDB& c) : owner(false), name(c.name), fd(c.fd) {
        st.st_ino = c.st.st_ino;
    }

    ~TinyCDB() {
        if (!owner)
            return;
        if (fd == -1)
            return;
        close(fd);
    }

private:
    bool owner;
    string name;
    int fd;
    struct stat st;

    //friend const failable<bool> post(const value& key, const value& val, TinyCDB& cdb);
    //friend const failable<bool> put(const value& key, const value& val, TinyCDB& cdb);
    //friend const failable<value> get(const value& key, const TinyCDB& cdb);
    //friend const failable<bool> del(const value& key, TinyCDB& cdb);
    //friend const failable<bool> rewrite(const lambda<failable<bool>(buffer& buf, const unsigned int klen, const unsigned int vlen)>& update, const lambda<failable<bool>(struct cdb_make&)>& finish, TinyCDB& cdb);
    //friend const failable<bool> rewrite(const lambda<failable<bool>(buffer& buf, const unsigned int klen, const unsigned int vlen)>& update, const lambda<failable<bool>(struct cdb_make&)>& finish, buffer& buf, const int fd, TinyCDB& cdb);

    friend const string dbname(const TinyCDB& cdb);
    friend const failable<int> cdbopen(TinyCDB& cdb);
    friend const failable<bool> cdbclose(TinyCDB& cdb);
};

/**
 * Return the name of the database.
 */
const string dbname(const TinyCDB& cdb) {
    return cdb.name;
}

/**
 * Open a database.
 */
const failable<int> cdbopen(TinyCDB& cdb) {

    // Get database file serial number
    struct stat st;
    const int s = stat(c_str(cdb.name), &st);
    if (s == -1)
        return mkfailure<int>(string("Couldn't read database stat ") + cdb.name);

    // Open database for the first time
    if (cdb.fd == -1) {
        cdb.fd = open(c_str(cdb.name), O_RDONLY);
        if (cdb.fd == -1)
            return mkfailure<int>(string("Couldn't open database file ") + cdb.name);
        debug(cdb.fd, "tinycdb::open::fd");
        cdb.st = st;
        return cdb.fd;
    }

    // Close and reopen database after a change
    if (st.st_ino != cdb.st.st_ino) {

        // Close current fd
        close(cdb.fd);

        // Reopen database
        const int newfd = open(c_str(cdb.name), O_RDONLY);
        if (newfd == -1)
            return mkfailure<int>(string("Couldn't open database file ") + cdb.name);
        if (newfd == cdb.fd) {
            debug(cdb.fd, "tinycdb::open::fd");
            cdb.st = st;
            return cdb.fd;
        }

        // We got a different fd, dup it to the current fd then close it
        if (fcntl(newfd, F_DUPFD, cdb.fd) == -1)
            return mkfailure<int>(string("Couldn't dup database file handle ") + cdb.name);
        close(newfd);

        debug(cdb.fd, "tinycdb::open::fd");
        cdb.st = st;
        return cdb.fd;
    }

    // No change, just return the current fd
    return cdb.fd;
}

/**
 * Close a database.
 */
const failable<bool> cdbclose(TinyCDB& cdb) {
    close(cdb.fd);
    cdb.fd = -1;
    return true;
}

/**
 * Rewrite a database. The given update function is passed each entry, and
 * can return true to let the entry added to the new db, false to skip the
 * entry, or a failure.
 */
const failable<bool> rewrite(const lambda<failable<bool>(buffer& buf, const unsigned int klen, const unsigned int vlen)>& update, const lambda<failable<bool>(struct cdb_make&)>& finish, buffer& buf, const int tmpfd, TinyCDB& cdb) {

    // Initialize new db structure
    struct cdb_make cdbm;
    cdb_make_start(&cdbm, tmpfd);

    // Open existing db
    failable<int> ffd = cdbopen(cdb);
    if (!hasContent(ffd))
        return mkfailure<bool>(reason(ffd));
    const int fd = content(ffd);

    // Read the db header
    unsigned int pos = 0;
    if (lseek(fd, 0, SEEK_SET) != 0)
        return mkfailure<bool>("Could not seek to database start");
    if (::read(fd, buf, 2048) != 2048)
        return mkfailure<bool>("Could not read database header");
    pos += 2048;
    unsigned int eod = cdb_unpack(buf);
    debug(pos, "tinycdb::rewrite::eod");

    // Read and add the existing entries
    while(pos < eod) {
        if (eod - pos < 8)
            return mkfailure<bool>("Invalid database format, couldn't read entry header");
        if (::read(fd, buf, 8) != 8)
            return mkfailure<bool>("Couldn't read entry header");
        pos += 8;
        unsigned int klen = cdb_unpack(buf);
        unsigned int vlen = cdb_unpack(((unsigned char*)buf) + 4);
        unsigned int elen = klen + vlen;

        // Read existing entry
        buf = mkbuffer(buf, elen);
        if (eod - pos < elen)
            return mkfailure<bool>("Invalid database format, couldn't read entry");
        if ((unsigned int)::read(fd, buf, elen) != elen)
            return mkfailure<bool>("Couldn't read entry");
        pos += elen;

        // Apply the update function to the entry
        debug(string((char*)buf, klen), "tinycdb::rewrite::existing key");
        debug(string(((char*)buf) + klen, vlen), "tinycdb::rewrite::existing value");
        const failable<bool> u = update(buf, klen, vlen);
        if (!hasContent(u))
            return u;

        // Skip the entry if the update function returned false
        if (u == false)
            continue;

        // Add the entry to the new db
        if (cdb_make_add(&cdbm, buf, klen, ((unsigned char*)buf)+klen, vlen) == -1)
            return mkfailure<bool>("Could not add entry");
    }
    if (pos != eod)
        return mkfailure<bool>("Invalid database format");

    // Call the finish function
    const failable<bool> f = finish(cdbm);
    if (!hasContent(f))
        return f;

    // Save the new db
    if (cdb_make_finish(&cdbm) == -1)
        return mkfailure<bool>("Could not save database");

    return true;
}

const failable<bool> rewrite(const lambda<failable<bool>(buffer& buf, const unsigned int klen, const unsigned int vlen)>& update, const lambda<failable<bool>(struct cdb_make&)>& finish, TinyCDB& cdb) {

    // Create a new temporary db file
    string tmpname = dbname(cdb) + ".XXXXXX";
    int tmpfd = mkstemp(const_cast<char*>(c_str(tmpname)));
    if (tmpfd == -1)
        return mkfailure<bool>("Could not create temporary database");

    // Rewrite the db, apply the update function to each entry
    buffer buf = mkbuffer(2048);
    const failable<bool> r = rewrite(update, finish, buf, tmpfd, cdb);
    if (!hasContent(r)) {
        close(tmpfd);
        free(buf);
        return r;
    }

    // Atomically replace the db and reopen it in read mode
    if (rename(c_str(tmpname), c_str(dbname(cdb))) == -1)
        return mkfailure<bool>("Could not rename temporary database");
    cdbclose(cdb);
    failable<int> ffd = cdbopen(cdb);
    if (!hasContent(ffd))
        return mkfailure<bool>(reason(ffd));

    return true;
}

/**
 * Post a new item to the database.
 */
const failable<bool> post(const value& key, const value& val, TinyCDB& cdb) {
    debug(key, "tinycdb::post::key");
    debug(val, "tinycdb::post::value");
    debug(dbname(cdb), "tinycdb::post::dbname");

    const string ks(scheme::writeValue(key));
    const string vs(scheme::writeValue(val));

    // Process each entry and detect existing key
    auto update = [=](buffer& buf, const unsigned int klen, unused const unsigned int vlen)->const failable<bool> {
        if (ks == string((char*)buf, klen))
            return mkfailure<bool>("Key already exists");
        return true;
    };

    // Add the new entry to the db
    auto finish = [=](struct cdb_make& cdbm)->const failable<bool> {
        if (cdb_make_add(&cdbm, c_str(ks), length(ks), c_str(vs), length(vs)) == -1)
            return mkfailure<bool>("Could not add entry");
        return true;
    };

    // Rewrite the db
    const failable<bool> r = rewrite(update, finish, cdb);
    debug(r, "tinycdb::post::result");
    return r;
}

/**
 * Update an item in the database. If the item doesn't exist it is added.
 */
const failable<bool> put(const value& key, const value& val, TinyCDB& cdb) {
    debug(key, "tinycdb::put::key");
    debug(val, "tinycdb::put::value");
    debug(dbname(cdb), "tinycdb::put::dbname");

    const string ks(scheme::writeValue(key));
    const string vs(scheme::writeValue(val));

    // Process each entry and skip existing key
    auto update = [&](buffer& buf, const unsigned int klen, unused const unsigned int vlen)->const failable<bool> {
        if (ks == string((char*)buf, klen))
            return false;
        return true;
    };

    // Add the new entry to the db
    auto finish = [&](struct cdb_make& cdbm)->const failable<bool> {
        if (cdb_make_add(&cdbm, c_str(ks), length(ks), c_str(vs), length(vs)) == -1)
            return mkfailure<bool>("Could not add entry");
        return true;
    };

    // Rewrite the db
    const failable<bool> r = rewrite(update, finish, cdb);
    debug(r, "tinycdb::put::result");
    return r;
}

/**
 * Get an item from the database.
 */
const failable<value> get(const value& key, TinyCDB& cdb) {
    debug(key, "tinycdb::get::key");
    debug(dbname(cdb), "tinycdb::get::dbname");

    const failable<int> ffd = cdbopen(cdb);
    if (!hasContent(ffd))
        return mkfailure<value>(reason(ffd));
    const int fd = content(ffd);

    const string ks(scheme::writeValue(key));

    cdbi_t vlen;
    if (cdb_seek(fd, c_str(ks), length(ks), &vlen) <= 0)
        return mkfailure<value>("Could not get entry");
    char* data = gc_cnew(vlen + 1);
    cdb_bread(fd, data, vlen);
    data[vlen] = '\0';
    const value val(scheme::readValue(string(data)));

    debug(val, "tinycdb::get::result");
    return val;
}

/**
 * Delete an item from the database
 */
const failable<bool> del(const value& key, TinyCDB& cdb) {
    debug(key, "tinycdb::delete::key");
    debug(dbname(cdb), "tinycdb::delete::dbname");

    const string ks(scheme::writeValue(key));

    // Process each entry and skip existing key
    auto update = [=](buffer& buf, const unsigned int klen, unused const unsigned int vlen)->const failable<bool> {
        if (ks == string((char*)buf, klen))
            return false;
        return true;
    };

    // Nothing to do to finish
    auto finish = [=](unused struct cdb_make& cdbm)->const failable<bool> {
        // hack: reference a variable from outer scope to workaround GCC internal error
        const string xs(ks);
        return true;
    };

    // Rewrite the db
    const failable<bool> r = rewrite(update, finish, cdb);
    debug(r, "tinycdb::delete::result");
    return r;
}

}
}

#endif /* tuscany_tinycdb_hpp */
