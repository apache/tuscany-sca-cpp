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
#include "../../modules/scheme/eval.hpp"
#include "../../modules/xml/xml.hpp"
#include "../../modules/json/json.hpp"

namespace tuscany {
namespace filedb {

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
 * Represents a FileDB connection.
 */
class FileDB {
public:
    FileDB() : owner(false) {
    }

    FileDB(const string& name, const string& format) : owner(true), name(absdbname(name)), format(format) {
        debug(name, "filedb::filedb::name");
        debug(format, "filedb::filedb::format");
    }

    FileDB(const FileDB& c) : owner(false), name(c.name), format(c.format) {
        debug("filedb::filedb::copy");
    }

    FileDB& operator=(const FileDB& c) = delete;

    ~FileDB() {
    }

private:
    unused const bool owner;
    const string name;
    const string format;

    friend const failable<bool> write(const value& v, ostream& os, const string& format);
    friend const failable<value> read(istream& is, const string& format);
    friend const failable<bool> post(const value& key, const value& val, const FileDB& db);
    friend const failable<bool> put(const value& key, const value& val, const FileDB& db);
    friend const failable<bool> patch(const value& key, const value& val, const FileDB& db);
    friend const failable<value> get(const value& key, const FileDB& db);
    friend const failable<bool> del(const value& key, const FileDB& db);
};

/**
 * Convert a key to a file name.
 */
const string filename(const list<value>& path, const string& root) {
    if (isNull(path))
        return root;
    const string name = root + "/" + (isString(car(path))? (string)car(path) : write(content(scheme::writeValue(car(path)))));
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
    if (isNull(cdr(path)))
        return true;
    const string dir = root + "/" + (isString(car(path))? (string)car(path) : write(content(scheme::writeValue(car(path)))));
    mkdir(c_str(dir), S_IRWXU);
    return mkdirs(cdr(path), dir);
}

/**
 * Write a value to a database file.
 */
const failable<bool> write(const value& v, ostream& os, const string& format) {
    if (format == "scheme") {
        const string vs(write(content(scheme::writeValue(v))));
        os << vs;
        return true;
    }
    if (format == "xml") {
        failable<list<string> > s = xml::writeElements(valuesToElements(v));
        if (!hasContent(s))
            return mkfailure<bool>(s);
        write(content(s), os);
        return true;
    }
    if (format == "json") {
        failable<list<string> > s = json::writeValue(v);
        if (!hasContent(s))
            return mkfailure<bool>(s);
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
        return scheme::readValue(streamList(is));
    }
    if (format == "xml") {
        const failable<list<value> > fv = xml::readElements(streamList(is));
        if (!hasContent(fv))
            return mkfailure<value>(fv);
        const value v = elementsToValues(content(fv));
        return v;
    }
    if (format == "json") {
        return json::readValue(streamList(is));
    }
    return mkfailure<value>(string("Unsupported database format: ") + format);
}

/**
 * Post a new item to the database.
 */
const failable<bool> post(const value& key, const value& val, const FileDB& db) {
    debug(key, "filedb::post::key");
    debug(val, "filedb::post::value");
    debug(db.name, "filedb::post::dbname");

    if (isList(key))
        mkdirs(key, db.name);
    const string fn = filename(key, db.name);
    debug(fn, "filedb::post::filename");
    ofstream os(fn);
    if (os.fail()) {
        ostringstream os;
        os << "Couldn't post file database entry: " << key;
        return mkfailure<bool>(str(os));
    }
    const failable<bool> r = write(val, os, db.format);

    debug(r, "filedb::post::result");
    return r;
}

/**
 * Update an item in the database. If the item doesn't exist it is added.
 */
const failable<bool> put(const value& key, const value& val, const FileDB& db) {
    debug(key, "filedb::put::key");
    debug(val, "filedb::put::value");
    debug(db.name, "filedb::put::dbname");

    if (isList(key))
        mkdirs(key, db.name);
    const string fn = filename(key, db.name);
    debug(fn, "filedb::put::filename");
    ofstream os(fn);
    if (os.fail()) {
        ostringstream os;
        os << "Couldn't put file database entry: " << key;
        return mkfailure<bool>(str(os));
    }
    const failable<bool> r = write(val, os, db.format);

    debug(r, "filedb::put::result");
    return r;
}

/**
 * Patch an item in the database. If the item doesn't exist it is added.
 */
const failable<bool> patch(const value& key, const value& val, const FileDB& db) {
    debug(key, "filedb::patch::key");
    debug(val, "filedb::patch::value");
    debug(db.name, "filedb::patch::dbname");

    if (isList(key))
        mkdirs(key, db.name);
    const string fn = filename(key, db.name);
    debug(fn, "filedb::patch::filename");
    ofstream os(fn);
    if (os.fail()) {
        ostringstream os;
        os << "Couldn't patch file database entry: " << key;
        return mkfailure<bool>(str(os));
    }
    const failable<bool> r = write(val, os, db.format);

    debug(r, "filedb::patch::result");
    return r;
}

/**
 * Get an item from the database.
 */
const failable<value> get(const value& key, const FileDB& db) {
    debug(key, "filedb::get::key");
    debug(db.name, "filedb::get::dbname");

    const string fn = filename(key, db.name);
    debug(fn, "filedb::get::filename");
    ifstream is(fn);
    if (is.fail()) {
        ostringstream os;
        os << "Couldn't get file database entry: " << key;
        return mkfailure<value>(str(os), 404, false);
    }
    const failable<value> val = read(is, db.format);

    debug(val, "filedb::get::result");
    return val;
}

/**
 * Delete an item from the database
 */
const failable<bool> del(const value& key, const FileDB& db) {
    debug(key, "filedb::delete::key");
    debug(db.name, "filedb::delete::dbname");

    const string fn = filename(key, db.name);
    debug(fn, "filedb::del::filename");
    const int rc = unlink(c_str(fn));
    if (rc == -1) {
        ostringstream os;
        os << "Couldn't delete file database entry: " << key;
        return errno == ENOENT? mkfailure<bool>(str(os), 404, false) : mkfailure<bool>(str(os));
    }

    debug(true, "filedb::delete::result");
    return true;
}

}
}

#endif /* tuscany_filedb_hpp */
