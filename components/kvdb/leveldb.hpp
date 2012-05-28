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

#ifndef tuscany_leveldb_hpp
#define tuscany_leveldb_hpp

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <leveldb/db.h>

#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../../modules/scheme/eval.hpp"

namespace tuscany {
namespace leveldb {

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
 * Convert a database name to an absolute path.
 */
const string absdbname(const string& name) {
    if (length(name) == 0 || c_str(name)[0] == '/')
        return name;
    char cwd[512];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return name;
    return string(cwd) + "/" + name;
}

/**
 * Represents a LevelDB connection.
 */
class LevelDB {
public:
    LevelDB() : owner(false), fd(-1) {
        debug("leveldb::leveldb");
        st.st_ino = 0;
    }

    LevelDB(const string& name) : owner(true), name(absdbname(name)), fd(-1) {
        debug(name, "leveldb::leveldb::name");
        st.st_ino = 0;
    }

    LevelDB(const LevelDB& c) : owner(false), name(c.name), fd(c.fd) {
        debug("leveldb::leveldb::copy");
        st.st_ino = c.st.st_ino;
    }

    ~LevelDB() {
        debug("leveldb::~leveldb");
        if (!owner)
            return;
        if (fd == -1)
            return;
        close(fd);
    }

private:
    bool owner;
    string name;
    leveldb::DB* db;
    struct stat st;

    friend const string dbname(const LevelDB& db);
    friend const failable<int> dbopen(LevelDB& db);
    friend const failable<bool> dbclose(LevelDB& db);
};

/**
 * Return the name of the database.
 */
const string dbname(const LevelDB& db) {
    return db.name;
}

/**
 * Open a database.
 */
const failable<int> dbopen(LevelDB& db) {

	// Get database file serial number
	struct stat st;
	const int s = stat(c_str(db.name), &st);
	if (s == -1)
		return mkfailure<int>(string("Couldn't leveldb read database stat: ") + db.name);

	leveldb::DB* ldb;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, s, &ldb);
	db.db = ldb;
}

/**
 * Close a database.
 */
const failable<bool> dbclose(LevelDB& db) {
	delete db.db;
	db.db = NULL;
    return true;
}


const failable<bool> post(const value& key, const value& val, LevelDB& db) {
    debug(key, "leveldb::post::key");
    debug(val, "leveldb::post::value");
    debug(dbname(db), "leveldb::post::dbname");

    const string ks(scheme::writeValue(key));
    const string vs(scheme::writeValue(val));

    put(ks, vs, db);

    debug(r, "leveldb::post::result");
    return r;
}


const failable<bool> put(const value& key, const value& val, LevelDB& db) {
    debug(key, "leveldb::put::key");
    debug(val, "leveldb::put::value");
    debug(dbname(db), "leveldb::put::dbname");

    const string ks(scheme::writeValue(key));
    const string vs(scheme::writeValue(val));

    debug(r, "leveldb::put::result");
    return r;
}

/**
 * Get an item from the database.
 */
const failable<value> get(const value& key, LevelDB& db) {
    debug(key, "leveldb::get::key");
    debug(dbname(db), "leveldb::get::dbname");

    const string ks(scheme::writeValue(key));

    std::string data;
    db.db->Get(leveldb::ReadOptions(), key, &data);
    const value val(scheme::readValue(string(data)));

    debug(val, "leveldb::get::result");
    return val;
}

/**
 * Delete an item from the database
 */
struct delUpdate {
    const string ks;
    delUpdate(const string& ks) : ks(ks) {
    }
    const failable<bool> operator()(buffer& buf, const unsigned int klen, unused const unsigned int vlen) const {
        if (ks == string((char*)buf, klen))
            return false;
        return true;
    }
};

struct delFinish {
    delFinish() {
    }
    const failable<bool> operator()(unused struct db_make& dbm) const {
        return true;
    }
};

const failable<bool> del(const value& key, LevelDB& db) {
    debug(key, "leveldb::delete::key");
    debug(dbname(db), "leveldb::delete::dbname");

    const string ks(scheme::writeValue(key));

    leveldb::Status s = db.db->Delete(leveldb::WriteOptions(), key);

    debug(r, "leveldb::delete::result");
    return s.ok();
}

}
}

#endif /* tuscany_leveldb_hpp */
