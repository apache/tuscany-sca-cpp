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

#ifndef tuscany_filedb_hpp
#define tuscany_filedb_hpp

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "fstream.hpp"
#include "../../modules/scheme/eval.hpp"

namespace tuscany {
namespace filedb {

/**
 * Represents a FileDB connection.
 */
class FileDB {
public:
    FileDB() : owner(false) {
    }

    FileDB(const string& name) : owner(true), name(name) {
    }

    FileDB(const FileDB& c) : owner(false), name(c.name) {
    }

    ~FileDB() {
    }

private:
    bool owner;
    string name;

    friend const string dbname(const FileDB& db);
};

/**
 * Return the name of the database.
 */
const string dbname(const FileDB& db) {
    return db.name;
}

/**
 * Convert a key to a file name.
 */
const string filename(const value& key, const FileDB& db) {
    return dbname(db) + "/" + scheme::writeValue(key);
}

/**
 * Post a new item to the database.
 */
const failable<bool> post(const value& key, const value& val, FileDB& db) {
    debug(key, "filedb::post::key");
    debug(val, "filedb::post::value");
    debug(dbname(db), "filedb::post::dbname");

    ofstream os(filename(key, db));
    const string vs(scheme::writeValue(val));
    os << vs;

    debug(true, "filedb::post::result");
    return true;
}

/**
 * Update an item in the database. If the item doesn't exist it is added.
 */
const failable<bool> put(const value& key, const value& val, FileDB& db) {
    debug(key, "filedb::put::key");
    debug(val, "filedb::put::value");
    debug(dbname(db), "filedb::put::dbname");

    ofstream os(filename(key, db));
    const string vs(scheme::writeValue(val));
    os << vs;

    debug(true, "filedb::put::result");
    return true;
}

/**
 * Get an item from the database.
 */
const failable<value> get(const value& key, FileDB& db) {
    debug(key, "filedb::get::key");
    debug(dbname(db), "filedb::get::dbname");

    ifstream is(filename(key, db));
    if (is.fail())
        return mkfailure<value>("Couldn't get file database entry.");
    const value val(scheme::readValue(is));

    debug(val, "filedb::get::result");
    return val;
}

/**
 * Delete an item from the database
 */
const failable<bool> del(const value& key, FileDB& db) {
    debug(key, "filedb::delete::key");
    debug(dbname(db), "filedb::delete::dbname");

    const int rc = unlink(c_str(filename(key, db)));
    if (rc == -1)
        return mkfailure<bool>("Couldn't delete file database entry.");
    debug(true, "filedb::delete::result");
    return true;
}

}
}

#endif /* tuscany_filedb_hpp */
