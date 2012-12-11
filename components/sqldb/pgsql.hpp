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

#ifndef tuscany_pgsql_hpp
#define tuscany_pgsql_hpp

/**
 * PostgreSQL access functions.
 */

#include <libpq-fe.h>

#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../../modules/scheme/eval.hpp"

namespace tuscany {
namespace pgsql {

/**
 * Return and clear a Postgres result failure.
 */
const string pgfailure(PGresult* const r, PGconn* const conn) {
    const string re = PQresultErrorMessage(r);
    PQclear(r);
    if (length(re) != 0)
        return re;
    const string ce = PQerrorMessage(conn);
    return ce;
}

/**
 * Represents a PGSql connection.
 */
class PGSql {
public:
    PGSql() : owner(false) {
        debug("pgsql::pgsql");
    }

    PGSql(const string& conninfo, const string& table) : owner(true), conn(NULL), conninfo(conninfo), table(table) {
        debug(conninfo, "pgsql::pgsql::conninfo");
        debug(table, "pgsql::pgsql::table");

        // Connect to the database
        conn = PQconnectdb(c_str(conninfo));
        if (PQstatus(conn) != CONNECTION_OK) {
            mkfailure<bool>(string("Couldn't connect to postgresql database: ") + PQerrorMessage(conn));
            return;
        }
        debug(conn, "pgsql::pgsql::conn");

        // Find the name of the first column in the target table
        // Assume that's the key we need to use
        string ks = string("select a.attname from pg_attribute a, pg_class c where a.attrelid = c.relfilenode and c.relname = '") + table + string("' and a.attnum in (1, 2) order by a.attnum;");
        PGresult* const kr = PQexec(conn, c_str(ks));
        if (PQresultStatus(kr) != PGRES_TUPLES_OK) {
            mkfailure<bool>(string("Couldn't execute postgresql column select statement: ") + pgfailure(kr, conn));
            return;
        }
        if (PQntuples(kr) != 2) {
            PQclear(kr);
            mkfailure<bool>(string("Couldn't find postgresql table key and value column names"));
            return;
        }
        kname = c_str(string(PQgetvalue(kr, 0, 0)));
        vname = c_str(string(PQgetvalue(kr, 1, 0)));
        PQclear(kr);
    }

    PGSql(const PGSql& c) : owner(false), conn(c.conn), conninfo(c.conninfo), table(c.table), kname(c.kname), vname(c.vname) {
        debug("pgsql::pgsql::copy");
    }

    PGSql& operator=(const PGSql& c) = delete;

    ~PGSql() {
        debug("pgsql::~pgsql");
        if (!owner)
            return;
        if (conn == NULL)
            return;
        debug(conn, "pgsql::~pgsql::conn");
        PQfinish(conn);
    }

private:
    const bool owner;
    PGconn *conn;
    const string conninfo;
    const string table;
    const char* kname;
    const char* vname;

    friend const failable<bool> setup(const PGSql& pgsql);
    friend const failable<bool> post(const value& key, const value& val, const PGSql& pgsql);
    friend const failable<bool> put(const value& key, const value& val, const PGSql& pgsql);
    friend const failable<value> get(const value& key, const PGSql& pgsql);
    friend const failable<bool> del(const value& key, const PGSql& pgsql);
};

/**
 * Setup the database connection if necessary.
 */
const failable<bool> setup(const PGSql& pgsql) {
    debug("pgsql::setup");
    if (PQstatus(pgsql.conn) == CONNECTION_OK)
        return true;
    debug(pgsql.conn, "pgsql::setup::reset::conn");
    PQreset(pgsql.conn);
    if (PQstatus(pgsql.conn) != CONNECTION_OK)
        return mkfailure<bool>(string("Couldn't reconnect to postgresql database: ") + PQerrorMessage(pgsql.conn));
    return true;
}

/**
 * Post a new item to the database.
 */
const failable<bool> post(const value& key, const value& val, const PGSql& pgsql) {
    debug(key, "pgsql::post::key");
    debug(val, "pgsql::post::value");
    debug(pgsql.conninfo, "pgsql::post::conninfo");
    debug(pgsql.table, "pgsql::post::table");
    setup(pgsql);

    const string ks(write(content(scheme::writeValue(key))));
    const string vs(write(content(scheme::writeValue(val))));
    const char* const params[2] = { c_str(ks), c_str(vs) };
    PGresult* const r = PQexecParams(pgsql.conn, c_str(string("insert into ") + pgsql.table + string(" values($1, $2);")), 2, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(r) != PGRES_COMMAND_OK)
        return mkfailure<bool>(string("Couldn't execute insert postgresql SQL statement: ") + pgfailure(r, pgsql.conn));
    PQclear(r);

    debug(true, "pgsql::post::result");
    return true;
}

/**
 * Update an item in the database. If the item doesn't exist it is added.
 */
const failable<bool> put(const value& key, const value& val, const PGSql& pgsql) {
    debug(key, "pgsql::put::key");
    debug(val, "pgsql::put::value");
    debug(pgsql.conninfo, "pgsql::put::conninfo");
    debug(pgsql.table, "pgsql::put::table");
    setup(pgsql);

    const string ks(write(content(scheme::writeValue(key))));
    const string vs(write(content(scheme::writeValue(val))));
    const char* const params[2] = { c_str(ks), c_str(vs) };
    PGresult* const r = PQexecParams(pgsql.conn, c_str(string("update ") + pgsql.table + string(" set ") + pgsql.vname + string(" = $2 where ") + pgsql.kname + string(" = $1;")), 2, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(r) != PGRES_COMMAND_OK)
        return mkfailure<bool>(string("Couldn't execute update postgresql SQL statement: ") + pgfailure(r, pgsql.conn));
    const string t = PQcmdTuples(r);
    if (t != "0") {
        PQclear(r);
        debug(true, "pgsql::put::result");
        return true;
    }
    PQclear(r);

    PGresult* const pr = PQexecParams(pgsql.conn, c_str(string("insert into ") + pgsql.table + string(" values($1, $2);")), 2, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(pr) != PGRES_COMMAND_OK)
        return mkfailure<bool>(string("Couldn't execute insert postgresql SQL statement: ") + pgfailure(pr, pgsql.conn));
    PQclear(pr);

    debug(true, "pgsql::put::result");
    return true;
}

/**
 * Convert a key to an item id.
 */
const list<value> keyid(const list<value>& key) {
    if (isNil(key))
        return nilListValue;
    if (isList(car(key)))
        return keyid(cdr(key));
    return cons<value>(car(key), keyid(cdr(key)));
}

/**
 * Convert a key to an param name / value assoc.
 */
const list<list<value> > keyparams(const list<value>& key) {
    if (isNil(key))
        return nilListValue;
    if (!isList(car(key)))
        return keyparams(cdr(key));
    return cons<list<value> >((list<value>)car(key), keyparams(cdr(key)));
}

/**
 * Convert a get result to a list of items.
 */
const list<value> getitems(PGresult* const r, const int i, const int n) {
    if (i == n)
        return nilListValue;
    const value key(content(scheme::readValue(string(PQgetvalue(r, i, 0)))));
    const value val(content(scheme::readValue(string(PQgetvalue(r, i, 1)))));
    return cons<value>(mklist<value>(key, val), getitems(r, i + 1, n));
}

/**
 * Parse a text search query and translate single quotes to spaces and double
 * quotes to single quotes.
 */
ostringstream& tsparse(ostringstream& os, const char* const c) {
    if (!*c)
        return os;
    os << (*c == '\''? ' ' : *c == '"'? '\'' : *c);
    return tsparse(os, c + 1);
}
const string tstranslate(const string& ts) {
    ostringstream os;
    tsparse(os, c_str(ts));
    return str(os);
}

/**
 * Get one item or a collection of items from the database.
 * The key is a simple value or a list of simple values plus optional name / value
 * pairs to specify regex, like, textsearch limit and offset clause
 */
const failable<value> get(const value& key, const PGSql& pgsql) {
    debug(key, "pgsql::get::key");
    debug(pgsql.conninfo, "pgsql::get::conninfo");
    debug(pgsql.table, "pgsql::get::table");
    setup(pgsql);

    // Get item and id and get parameters from the key
    const bool lk = isList(key);
    const list<list<value> > kparams = lk? keyparams(key) : list<list<value> >();
    const list<value> regex = assoc<value>("regex", kparams);
    const list<value> like = assoc<value>("like", kparams);
    const list<value> textsearch = assoc<value>("textsearch", kparams);
    const list<value> limit = assoc<value>("limit", kparams);
    const list<value> offset = assoc<value>("offset", kparams);
    const list<value> id = lk? keyid(key) : nilListValue;
    const list<value> atable = assoc<value>("table", kparams);
    const string table = isNil(atable)? pgsql.table : (string)cadr(atable);
    const list<value> akname = assoc<value>("kcolumn", kparams);
    const string kname = isNil(akname)? pgsql.kname : (string)cadr(akname);
    const list<value> avname = assoc<value>("vcolumn", kparams);
    const string vname = isNil(avname)? pgsql.vname : (string)cadr(avname);

    // Build the SQL query
    const char* sqlparams[5];
    int p = 0;
    int w = 0;
    ostringstream sqlos;
    sqlos << "select data.*";
    if (!isNil(textsearch)) {
        // Text search, setup result ranking
        sqlos << ", ts_rank_cd(to_tsvector(data." << vname << "), tsquery, 32) as rank";
    }
    sqlos << " from " << table << " data";
    if (!isNil(textsearch)) {
        // Text search, define the query
        const string ts = tstranslate((string)cadr(textsearch));
        sqlparams[p++] = c_str(ts);
        sqlos << ", plainto_tsquery($" << p << ") tsquery";
    }
    if (!lk || !isNil(id)) {
        // Query of the form key = id
        sqlparams[p++] = c_str(write(content(scheme::writeValue(lk? (value)id : key))));
        sqlos << (w == 0? " where" : " and");
        sqlos << " data." << kname << " = $" << p;
        w++;
    }
    if (!isNil(regex)) {
        // Query of the form key ~ param
        sqlparams[p++] = c_str((string)cadr(regex));
        sqlos << (w == 0? " where" : " and");
        sqlos << " data." << kname << " ~ $" << p;
        w++;
    }
    if (!isNil(like)) {
        // Query of the form key like param
        sqlparams[p++] = c_str((string)cadr(like));
        sqlos << (w == 0? " where" : " and");
        sqlos << " data." << kname << " like $" << p;
        w++;
    }
    if (!isNil(textsearch)) {
        // Text search, apply the query
        sqlos << (w == 0? " where" : " and");
        sqlos << " tsquery @@ to_tsvector(data." << vname << ") order by rank desc";
        w++;
    }
    if (!isNil(offset)) {
        // Result pagination offset
        sqlos << " offset " << atoi(c_str((string)cadr(offset)));
    }
    // Result limit count
    const int l = isNil(limit)? 1 : atoi(c_str((string)cadr(limit)));
    sqlos << " limit " << l << ";";
    
    // Execute the query
    const string sqls = str(sqlos);
    debug(sqls, "pgsql::get::sqls");
    PGresult* r = PQexecParams(pgsql.conn, c_str(sqls), p, NULL, sqlparams, NULL, NULL, 0);
    if (PQresultStatus(r) != PGRES_TUPLES_OK)
        return mkfailure<value>(string("Couldn't execute select postgresql SQL statement: ") + pgfailure(r, pgsql.conn));
    const int n = PQntuples(r);
    if (n < 1) {
        PQclear(r);
        ostringstream os;
        os << "Couldn't get postgresql entry: " << key;
        return mkfailure<value>(str(os), 404, false);
    }

    // Return a collection of key / item pairs
    if (l != 1) {
        const list<value> lval = getitems(r, 0, n);
        PQclear(r);
        debug(lval, "pgsql::get::result");
        return (value)lval;
    }

    // Return a single item
    const value val(content(scheme::readValue(string(PQgetvalue(r, 0, 1)))));
    PQclear(r);
    debug(val, "pgsql::get::result");
    return val;
}

/**
 * Delete an item from the database
 */
const failable<bool> del(const value& key, const PGSql& pgsql) {
    debug(key, "pgsql::delete::key");
    debug(pgsql.conninfo, "pgsql::delete::conninfo");
    debug(pgsql.table, "pgsql::delete::table");
    setup(pgsql);

    const string ks(write(content(scheme::writeValue(key))));
    const char* const params[1] = { c_str(ks) };
    PGresult* const r = PQexecParams(pgsql.conn, c_str(string("delete from ") + pgsql.table + string(" where ") + pgsql.kname + string(" = $1;")), 1, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(r) != PGRES_COMMAND_OK)
        return mkfailure<bool>(string("Couldn't execute delete postgresql SQL statement: ") + pgfailure(r, pgsql.conn));
    PQclear(r);

    debug(true, "pgsql::delete::result");
    return true;
}

}
}

#endif /* tuscany_pgsql_hpp */
