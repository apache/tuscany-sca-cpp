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
    TinyCDB() : owner(false) {
    }

    TinyCDB(const string& file) : owner(true), file(file) {
        fd = open(c_str(file), O_RDONLY);
    }

    TinyCDB(const TinyCDB& c) : owner(false) {
        file = c.file;
        fd = c.fd;
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
    string file;
    int fd;

    friend const failable<bool> post(const value& key, const value& val, TinyCDB& cdb);
    friend const failable<bool> put(const value& key, const value& val, const TinyCDB& cdb);
    friend const failable<value> get(const value& key, const TinyCDB& cdb);
    friend const failable<bool> del(const value& key, const TinyCDB& cdb);
    friend const failable<bool> rewrite(const lambda<failable<bool>(buffer& buf, const unsigned int klen, const unsigned int vlen)>& update, const lambda<failable<bool>(struct cdb_make&)>& finish, TinyCDB& cdb);
    friend const failable<bool> rewrite(const lambda<failable<bool>(buffer& buf, const unsigned int klen, const unsigned int vlen)>& update, const lambda<failable<bool>(struct cdb_make&)>& finish, buffer& buf, const int fd, TinyCDB& cdb);
};

/**
 * Rewrite a database. The given update function is passed each entry, and
 * can return true to let the entry added to the new db, false to skip the
 * entry, or a failure.
 */
const failable<bool> rewrite(const lambda<failable<bool>(buffer& buf, const unsigned int klen, const unsigned int vlen)>& update, const lambda<failable<bool>(struct cdb_make&)>& finish, buffer& buf, const int fd, TinyCDB& cdb) {

    // Initialize new db structure
    struct cdb_make cdbm;
    cdb_make_start(&cdbm, fd);

    // Read the db header
    unsigned int pos = 0;
    if (lseek(cdb.fd, 0, SEEK_SET) != 0)
        return mkfailure<bool>("Could not seek to database start");
    if (::read(cdb.fd, buf, 2048) != 2048)
        return mkfailure<bool>("Could not read database header");
    pos += 2048;
    unsigned int eod = cdb_unpack(buf);
    debug(pos, "tinycdb::post::eod");

    // Read and add the existing entries
    while(pos < eod) {
        if (eod - pos < 8)
            return mkfailure<bool>("Invalid database format, couldn't read entry header");
        if (::read(cdb.fd, buf, 8) != 8)
            return mkfailure<bool>("Couldn't read entry header");
        pos += 8;
        unsigned int klen = cdb_unpack(buf);
        unsigned int vlen = cdb_unpack(((unsigned char*)buf) + 4);
        unsigned int elen = klen + vlen;

        // Read existing entry
        buf = mkbuffer(buf, elen);
        if (eod - pos < elen)
            return mkfailure<bool>("Invalid database format, couldn't read entry");
        if ((unsigned int)::read(cdb.fd, buf, elen) != elen)
            return mkfailure<bool>("Couldn't read entry");
        pos += elen;

        // Apply the update function to the entry
        debug(string((char*)buf, klen), "tinycdb::post::existing key");
        debug(string(((char*)buf) + klen, vlen), "tinycdb::post::existing value");
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
    if (cdb.fd == -1)
        return mkfailure<bool>("Could not open database");

    // Create a new temporary db file
    const char* tmpfile = c_str(cdb.file + ".tmp");
    unlink(tmpfile);
    int fd = open(tmpfile, O_RDWR | O_CREAT | O_EXCL | O_NOFOLLOW, 0666);
    if (fd == -1)
        return mkfailure<bool>("Could not create temporary database");

    // Rewrite the db, apply the update function to each entry
    buffer buf = mkbuffer(2048);
    const failable<bool> r = rewrite(update, finish, buf, fd, cdb);
    if (!hasContent(r)) {
        close(fd);
        free(buf);
        return r;
    }

    // Atomically replace the db and reopen it in read mode
    if (rename(tmpfile, c_str(cdb.file)) == -1)
        return mkfailure<bool>("Could not rename temporary database");
    close(cdb.fd);
    cdb.fd = open(c_str(cdb.file), O_RDONLY);

    return true;
}

/**
 * Post a new item to the database.
 */
const failable<bool> post(const value& key, const value& val, TinyCDB& cdb) {
    debug(key, "tinycdb::post::key");
    debug(val, "tinycdb::post::value");

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
const failable<value> get(const value& key, const TinyCDB& cdb) {
    debug(key, "tinycdb::get::key");
    if (cdb.fd == -1)
        return mkfailure<value>("Could not open database");

    const string ks(scheme::writeValue(key));

    cdbi_t vlen;
    if (cdb_seek(cdb.fd, c_str(ks), length(ks), &vlen) <= 0)
        return mkfailure<value>("Could not get entry");
    char* data = gc_cnew(vlen + 1);
    cdb_bread(cdb.fd, data, vlen);
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
