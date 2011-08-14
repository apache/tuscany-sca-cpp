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
#include "element.hpp"
#include "xml.hpp"
#include "../../modules/scheme/eval.hpp"
#include "../../modules/json/json.hpp"

namespace tuscany {
namespace filedb {

/**
 * Represents a FileDB connection.
 */
class FileDB {
public:
    FileDB() : owner(false) {
        debug("filedb::filedb");
    }

    FileDB(const string& name, const string& format) : owner(true), name(name), format(format) {
        debug(name, "filedb::filedb::name");
        debug(format, "filedb::filedb::format");
    }

    FileDB(const FileDB& c) : owner(false), name(c.name), format(c.format) {
        debug("filedb::filedb::copy");
    }

    ~FileDB() {
        debug("filedb::~filedb");
    }

private:
    bool owner;
    string name;
    string format;

    friend const failable<bool> write(const value& v, ostream& os, const string& format);
    friend const failable<value> read(istream& is, const string& format);
    friend const failable<bool> post(const value& key, const value& val, FileDB& db);
    friend const failable<bool> put(const value& key, const value& val, FileDB& db);
    friend const failable<value> get(const value& key, FileDB& db);
    friend const failable<bool> del(const value& key, FileDB& db);
};

/**
 * Convert a key to a file name.
 */
const string filename(const list<value>& path, const string& root) {
    if (isNil(path))
        return root;
    const string name = root + "/" + (isString(car(path))? (string)car(path) : scheme::writeValue(car(path)));
    return filename(cdr(path), name);
}

const string filename(const value& key, const string& root) {
    if (!isList(key))
        return filename(mklist(key), root);
    return filename((list<value>)key, root);
}

/**
 * Make the parent directories of a keyed file.
 */
const failable<bool> mkdirs(const list<value>& path, const string& root) {
    if (isNil(cdr(path)))
        return true;
    const string dir = root + "/" + (isString(car(path))? (string)car(path) : scheme::writeValue(car(path)));
    mkdir(c_str(dir), S_IRWXU);
    return mkdirs(cdr(path), dir);
}

/**
 * Write a value to a database file.
 */
const failable<bool> write(const value& v, ostream& os, const string& format) {
    if (format == "scheme") {
        const string vs(scheme::writeValue(v));
        os << vs;
        return true;
    }
    if (format == "xml") {
        failable<list<string> > s = writeXML(valuesToElements(v));
        if (!hasContent(s))
            return mkfailure<bool>(reason(s));
        write(content(s), os);
        return true;
    }
    if (format == "json") {
        js::JSContext jscx;
        failable<list<string> > s = json::writeJSON(valuesToElements(v), jscx);
        if (!hasContent(s))
            return mkfailure<bool>(reason(s));
        write(content(s), os);
        return true;
    }
    return mkfailure<bool>(string("Unsupported database format: ") + format);
}

/**
 * Read a value from a database file.
 */
const failable<value> read(istream& is, const string& format) {
    if (format == "scheme") {
        return scheme::readValue(is);
    }
    if (format == "xml") {
        const value v = elementsToValues(readXML(streamList(is)));
        return v;
    }
    if (format == "json") {
        js::JSContext jscx;
        const failable<list<value> > fv = json::readJSON(streamList(is), jscx);
        if (!hasContent(fv))
            return mkfailure<value>(reason(fv));
        const value v = elementsToValues(content(fv));
        return v;
    }
    return mkfailure<value>(string("Unsupported database format: ") + format);
}

/**
 * Post a new item to the database.
 */
const failable<bool> post(const value& key, const value& val, FileDB& db) {
    debug(key, "filedb::post::key");
    debug(val, "filedb::post::value");
    debug(db.name, "filedb::post::dbname");

    if (isList(key))
        mkdirs(key, db.name);
    const string fn = filename(key, db.name);
    debug(fn, "filedb::post::filename");
    ofstream os(fn);
    if (os.fail())
        return mkfailure<bool>("Couldn't post file database entry.");
    const failable<bool> r = write(val, os, db.format);

    debug(r, "filedb::post::result");
    return r;
}

/**
 * Update an item in the database. If the item doesn't exist it is added.
 */
const failable<bool> put(const value& key, const value& val, FileDB& db) {
    debug(key, "filedb::put::key");
    debug(val, "filedb::put::value");
    debug(db.name, "filedb::put::dbname");

    if (isList(key))
        mkdirs(key, db.name);
    const string fn = filename(key, db.name);
    debug(fn, "filedb::put::filename");
    ofstream os(fn);
    if (os.fail())
        return mkfailure<bool>("Couldn't put file database entry.");
    const failable<bool> r = write(val, os, db.format);

    debug(r, "filedb::put::result");
    return r;
}

/**
 * Get an item from the database.
 */
const failable<value> get(const value& key, FileDB& db) {
    debug(key, "filedb::get::key");
    debug(db.name, "filedb::get::dbname");

    const string fn = filename(key, db.name);
    debug(fn, "filedb::get::filename");
    ifstream is(fn);
    if (is.fail())
        return mkfailure<value>("Couldn't get file database entry.");
    const failable<value> val = read(is, db.format);

    debug(val, "filedb::get::result");
    return val;
}

/**
 * Delete an item from the database
 */
const failable<bool> del(const value& key, FileDB& db) {
    debug(key, "filedb::delete::key");
    debug(db.name, "filedb::delete::dbname");

    const string fn = filename(key, db.name);
    debug(fn, "filedb::del::filename");
    const int rc = unlink(c_str(fn));
    if (rc == -1)
        return mkfailure<bool>("Couldn't delete file database entry.");

    debug(true, "filedb::delete::result");
    return true;
}

}
}

#endif /* tuscany_filedb_hpp */
