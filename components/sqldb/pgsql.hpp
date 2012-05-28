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
const string pgfailure(PGresult* r, PGconn* conn) {
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

        // Find the name of the first column in the target table
        // Assume that's the key we need to use
        string ks = string("select a.attname from pg_attribute a, pg_class c where a.attrelid = c.relfilenode and c.relname = '") + table + string("' and a.attnum in (1, 2) order by a.attnum;");
        PGresult* kr = PQexec(conn, c_str(ks));
        if (PQresultStatus(kr) != PGRES_TUPLES_OK) {
            mkfailure<bool>(string("Couldn't execute postgresql column select statement: ") + pgfailure(kr, conn));
            return;
        }
        if (PQntuples(kr) != 2) {
            PQclear(kr);
            mkfailure<bool>(string("Couldn't find postgresql table key and value column names"));
            return;
        }
        kname = PQgetvalue(kr, 0, 0);
        vname = PQgetvalue(kr, 1, 0);
        PQclear(kr);
    }

    PGSql(const PGSql& c) : owner(false), conn(c.conn), conninfo(c.conninfo), table(c.table) {
        debug("pgsql::pgsql::copy");
    }

    ~PGSql() {
        debug("pgsql::~pgsql");
        if (!owner)
            return;
        if (conn == NULL)
            return;
        PQfinish(conn);
    }

private:
    bool owner;
    PGconn *conn;
    string conninfo;
    const string table;
    string kname;
    string vname;

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
    debug("pgsql::setup::reset");
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

    const string ks(scheme::writeValue(key));
    const string vs(scheme::writeValue(val));
    const char* params[2] = { c_str(ks), c_str(vs) };
    PGresult* r = PQexecParams(pgsql.conn, c_str(string("insert into ") + pgsql.table + string(" values($1, $2);")), 2, NULL, params, NULL, NULL, 0);
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

    const string ks(scheme::writeValue(key));
    const string vs(scheme::writeValue(val));
    const char* params[2] = { c_str(ks), c_str(vs) };
    PGresult* r = PQexecParams(pgsql.conn, c_str(string("update ") + pgsql.table + string(" set ") + pgsql.vname + string(" = $2 where ") + pgsql.kname + string(" = $1;")), 2, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(r) != PGRES_COMMAND_OK)
        return mkfailure<bool>(string("Couldn't execute update postgresql SQL statement: ") + pgfailure(r, pgsql.conn));
    const string t = PQcmdTuples(r);
    if (t != "0") {
        PQclear(r);
        debug(true, "pgsql::put::result");
        return true;
    }
    PQclear(r);

    PGresult* pr = PQexecParams(pgsql.conn, c_str(string("insert into ") + pgsql.table + string(" values($1, $2);")), 2, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(pr) != PGRES_COMMAND_OK)
        return mkfailure<bool>(string("Couldn't execute insert postgresql SQL statement: ") + pgfailure(pr, pgsql.conn));
    PQclear(pr);

    debug(true, "pgsql::put::result");
    return true;
}

/**
 * Get an item from the database.
 */
const failable<value> get(const value& key, const PGSql& pgsql) {
    debug(key, "pgsql::get::key");
    debug(pgsql.conninfo, "pgsql::get::conninfo");
    debug(pgsql.table, "pgsql::get::table");
    setup(pgsql);

    const string ks(scheme::writeValue(key));
    const char* params[1] = { c_str(ks) };
    PGresult* r = PQexecParams(pgsql.conn, c_str(string("select * from ") + pgsql.table + string(" where ") + pgsql.kname + string(" = $1;")), 1, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(r) != PGRES_TUPLES_OK)
        return mkfailure<value>(string("Couldn't execute select postgresql SQL statement: ") + pgfailure(r, pgsql.conn));
    if (PQntuples(r) < 1) {
        PQclear(r);
        ostringstream os;
        os << "Couldn't get postgresql entry: " << key;
        return mkfailure<value>(str(os), 404, false);
    }
    const char* data = PQgetvalue(r, 0, 1);
    const value val(scheme::readValue(string(data)));
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

    const string ks(scheme::writeValue(key));
    const char* params[1] = { c_str(ks) };
    PGresult* r = PQexecParams(pgsql.conn, c_str(string("delete from ") + pgsql.table + string(" where ") + pgsql.kname + string(" = $1;")), 1, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(r) != PGRES_COMMAND_OK)
        return mkfailure<bool>(string("Couldn't execute delete postgresql SQL statement: ") + pgfailure(r, pgsql.conn));
    PQclear(r);

    debug(true, "pgsql::delete::result");
    return true;
}

}
}

#endif /* tuscany_pgsql_hpp */
