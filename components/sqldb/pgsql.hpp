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
            const string rs = string("Couldn't execute postgresql column select statement: ") + pgfailure(kr, conn);
            PQclear(kr);
            mkfailure<bool>(rs);
            return;
        }
        if (PQntuples(kr) != 2) {
            const string rs = "Couldn't find postgresql table key and value column names";
            PQclear(kr);
            mkfailure<bool>(rs);
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
        if (!owner)
            return;
        if (conn == NULL)
            return;
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
    friend const failable<bool> begin(const PGSql& pgsql);
    friend const failable<bool> commit(const PGSql& pgsql);
    friend const failable<bool> rollback(const PGSql& pgsql);
    friend const failable<bool> post(const value& key, const value& val, const PGSql& pgsql);
    friend const failable<bool> put(const value& key, const value& val, const PGSql& pgsql);
    friend const failable<bool> patch(const value& key, const value& val, const PGSql& pgsql);
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
 * Begin a database transaction.
 */
const failable<bool> begin(const PGSql& pgsql) {
    debug("pgsql::begin");
    debug(pgsql.conninfo, "pgsql::begin::conninfo");
    debug(pgsql.table, "pgsql::begin::table");
    setup(pgsql);

    PGresult* const r = PQexec(pgsql.conn, "begin transaction isolation level repeatable read");
    if (PQresultStatus(r) != PGRES_COMMAND_OK) {
        const string rs = string("Couldn't execute begin SQL statement: ") + pgfailure(r, pgsql.conn);
        PQclear(r);
        return mkfailure<bool>(rs);
    }
    PQclear(r);

    debug(true, "pgsql::begin::result");
    return true;
}

/**
 * Commit a database transaction.
 */
const failable<bool> commit(const PGSql& pgsql) {
    debug("pgsql::commit");
    debug(pgsql.conninfo, "pgsql::commit::conninfo");
    debug(pgsql.table, "pgsql::commit::table");
    setup(pgsql);

    PGresult* const r = PQexec(pgsql.conn, "commit");
    if (PQresultStatus(r) != PGRES_COMMAND_OK) {
        const string rs = string("Couldn't execute commit SQL statement: ") + pgfailure(r, pgsql.conn);
        PQclear(r);
        return mkfailure<bool>(rs);
    }
    PQclear(r);

    debug(true, "pgsql::commit::result");
    return true;
}

/**
 * Rollback a database transaction.
 */
const failable<bool> rollback(const PGSql& pgsql) {
    debug("pgsql::rollback");
    debug(pgsql.conninfo, "pgsql::rollback::conninfo");
    debug(pgsql.table, "pgsql::rollback::table");
    setup(pgsql);

    PGresult* const r = PQexec(pgsql.conn, "rollback");
    if (PQresultStatus(r) != PGRES_COMMAND_OK) {
        const string rs = string("Couldn't execute rollback SQL statement: ") + pgfailure(r, pgsql.conn);
        PQclear(r);
        return mkfailure<bool>(rs);
    }
    PQclear(r);

    debug(true, "pgsql::rollback::result");
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
    if (PQresultStatus(r) != PGRES_COMMAND_OK) {
        const string rs = string("Couldn't execute insert postgresql SQL statement: ") + pgfailure(r, pgsql.conn);
        PQclear(r);
        return mkfailure<bool>(rs);
    }
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
    if (PQresultStatus(r) != PGRES_COMMAND_OK) {
        const string rs = string("Couldn't execute update postgresql SQL statement: ") + pgfailure(r, pgsql.conn);
        PQclear(r);
        return mkfailure<bool>(rs);
    }
    const char* const t = PQcmdTuples(r);
    if (t != NULL && strcmp(t, "0")) {
        PQclear(r);
        debug(true, "pgsql::put::result");
        return true;
    }
    PQclear(r);

    PGresult* const pr = PQexecParams(pgsql.conn, c_str(string("insert into ") + pgsql.table + string(" values($1, $2);")), 2, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(pr) != PGRES_COMMAND_OK) {
        const string rs = string("Couldn't execute insert postgresql SQL statement: ") + pgfailure(pr, pgsql.conn);
        PQclear(pr);
        return mkfailure<bool>(rs);
    }
    PQclear(pr);

    debug(true, "pgsql::put::result");
    return true;
}

/**
 * Patch an item in the database. If the item doesn't exist it is added.
 */
const failable<bool> patch(const value& key, const value& val, const PGSql& pgsql) {
    debug(key, "pgsql::patch::key");
    debug(val, "pgsql::patch::value");
    debug(pgsql.conninfo, "pgsql::patch::conninfo");
    debug(pgsql.table, "pgsql::patch::table");
    setup(pgsql);

    const string ks(write(content(scheme::writeValue(key))));
    const string vs(write(content(scheme::writeValue(val))));
    const char* const params[2] = { c_str(ks), c_str(vs) };
    PGresult* const r = PQexecParams(pgsql.conn, c_str(string("update ") + pgsql.table + string(" set ") + pgsql.vname + string(" = $2 where ") + pgsql.kname + string(" = $1;")), 2, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(r) != PGRES_COMMAND_OK) {
        const string rs = string("Couldn't execute update postgresql SQL statement: ") + pgfailure(r, pgsql.conn);
        const char* const st = PQresultErrorField(r, PG_DIAG_SQLSTATE);
        if (st != NULL && !strncmp(st, "40", 2)) {

            // Report a transaction serialization conflict
            PQclear(r);
            return mkfailure<bool>(rs, 409);
        }
        PQclear(r);
        return mkfailure<bool>(rs);
    }
    const char* const t = PQcmdTuples(r);
    if (t != NULL && strcmp(t, "0")) {
        PQclear(r);
        debug(true, "pgsql::patch::result");
        return true;
    }
    PQclear(r);

    PGresult* const pr = PQexecParams(pgsql.conn, c_str(string("insert into ") + pgsql.table + string(" values($1, $2);")), 2, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(pr) != PGRES_COMMAND_OK) {
        const string rs = string("Couldn't execute insert postgresql SQL statement: ") + pgfailure(pr, pgsql.conn);
        const char* const st = PQresultErrorField(pr, PG_DIAG_SQLSTATE);
        if (st != NULL && !strncmp(st, "40", 2)) {
            PQclear(pr);
            return mkfailure<bool>(rs, 40);
        }
        PQclear(pr);
        return mkfailure<bool>(rs);
    }
    PQclear(pr);

    debug(true, "pgsql::patch::result");
    return true;
}

/**
 * Convert a key to an item id.
 */
const list<value> keyid(const list<value>& key) {
    if (isNull(key))
        return nilListValue;
    if (isList(car(key)))
        return keyid(cdr(key));
    return cons<value>(car(key), keyid(cdr(key)));
}

/**
 * Convert a key to a (param name, value) assoc.
 */
const list<value> keyparams(const list<value>& key) {
    if (isNull(key))
        return nilListValue;
    if (!isList(car(key)))
        return keyparams(cdr(key));
    return cons<value>(car(key), keyparams(cdr(key)));
}

/**
 * Convert an SQL result row to a result item.
 */
const value getitem(PGresult* const r, const int i, const int rk) {
    const value val(content(scheme::readValue(string(PQgetvalue(r, i, 1)))));
    if (rk > 0) {
        // Add row tsrank and rank to result item if it's an ATOM entry
        if (isList(val) && !isNull(val)) {
            const value e = car<value>(val);
            if (isList(e) && !isNull(e)) {
                if (car<value>(e) == "entry") {
                    const list<value> ae = cdr<value>(e);
                    const list<value> lt = assoc<value>("title", ae);
                    const list<value> li = assoc<value>("id", ae);
                    const list<value> la = assoc<value>("author", ae);
                    const list<value> lu = assoc<value>("updated", ae);
                    const list<value> lc = assoc<value>("content", ae);

                    ostringstream ros;
                    ros << string(PQgetvalue(r, i, 2));
                    if (rk > 1)
                        ros << " " << string(PQgetvalue(r, i, 3));
                    const list<value> lr = mklist<value>("rank", str(ros));

                    const value rval = mklist<value>("entry") +
                        (isNull(lt)? nilListValue : lt) + (isNull(li)? nilListValue : li) + (isNull(la)? nilListValue : la) +
                        (isNull(lu)? nilListValue : lu) + lr + (isNull(lc)? nilListValue : lc);
                    debug(rval, "pgsql::getitem::rval");
                    return mklist<value>(rval);
                }
            }
        }
    }

    debug(val, "pgsql::getitem::val");
    return val;
}

/**
 * Convert an SQL result to a list of result items.
 */
const list<value> getitems(PGresult* const r, const int i, const int n, const int rk) {
    if (i == n)
        return nilListValue;
    return cons<value>(getitem(r, i, rk), getitems(r, i + 1, n, rk));
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
    const list<value> kparams = lk? keyparams(key) : nilListValue;
    const list<value> regex = assoc<value>("regex", kparams);
    const list<value> like = assoc<value>("like", kparams);
    const list<value> textsearch = assoc<value>("textsearch", kparams);
    const list<value> limit = assoc<value>("limit", kparams);
    const list<value> offset = assoc<value>("offset", kparams);
    const list<value> rank = assoc<value>("rank", kparams);
    const list<value> id = lk? keyid(key) : nilListValue;
    const list<value> atable = assoc<value>("table", kparams);
    const string table = isNull(atable)? pgsql.table : (string)cadr(atable);
    const list<value> akname = assoc<value>("kcolumn", kparams);
    const string kname = isNull(akname)? pgsql.kname : (string)cadr(akname);
    const list<value> avname = assoc<value>("vcolumn", kparams);
    const string vname = isNull(avname)? pgsql.vname : (string)cadr(avname);

    // Build the SQL query
    const char* sqlparams[6];
    int p = 0;
    int w = 0;
    int rk = 0;
    ostringstream sqlos;
    sqlos << "select data." << kname << ", data." << vname;
    if (!isNull(textsearch)) {
        // Text search, setup text result ranking
        sqlos << ", ts_rank_cd(to_tsvector(data." << vname << "), tsquery, 32) as tsrank";
        rk++;
    }
    if (!isNull(rank)) {
        // Ranking, setup rank expression
        const string rs = (string)cadr(rank);
        sqlos << ", " << rs << " as rank";
        rk++;
    }
    sqlos << " from " << table << " data";
    if (!isNull(textsearch)) {
        // Text search, define the query
        const string ts = tstranslate((string)cadr(textsearch));
        debug(ts, "pgsql::get::sqlparam");
        sqlparams[p++] = c_str(ts);
        sqlos << ", plainto_tsquery($" << p << ") tsquery";
    }
    if (!lk || !isNull(id)) {
        // Query of the form key = id
        const string ks = write(content(scheme::writeValue(lk? (value)id : key)));
        debug(ks, "pgsql::get::sqlparam");
        sqlparams[p++] = c_str(ks);
        sqlos << (w == 0? " where" : " and");
        sqlos << " data." << kname << " = $" << p;
        w++;
    }
    if (!isNull(regex)) {
        // Query of the form key ~ param
        const string rs = cadr(regex);
        debug(rs, "pgsql::get::sqlparam");
        sqlparams[p++] = c_str(rs);
        sqlos << (w == 0? " where" : " and");
        sqlos << " data." << kname << " ~ $" << p;
        w++;
    }
    if (!isNull(like)) {
        // Query of the form key like param
        const string ls = cadr(like);
        debug(ls, "pgsql::get::sqlparam");
        sqlparams[p++] = c_str(ls);
        sqlos << (w == 0? " where" : " and");
        sqlos << " data." << kname << " like $" << p;
        w++;
    }
    if (!isNull(textsearch)) {
        // Text search, apply the query
        sqlos << (w == 0? " where" : " and");
        sqlos << " tsquery @@ to_tsvector(data." << vname << ")";
        w++;
    }
    if (!isNull(rank) || !isNull(textsearch)) {
        // Result ordering
        sqlos << " order by" << (isNull(rank)? "" : " rank desc") << ((isNull(rank) || isNull(textsearch))? "" : ",") << (isNull(textsearch)? "" : " tsrank desc");
    }
    if (!isNull(offset)) {
        // Result pagination offset
        sqlos << " offset " << atoi(c_str((string)cadr(offset)));
    }
    // Result limit count
    const int l = isNull(limit)? 1 : atoi(c_str((string)cadr(limit)));
    sqlos << " limit " << l << ";";
    
    // Execute the query
    const string sqls = str(sqlos);
    debug(sqls, "pgsql::get::sqls");
    PGresult* r = PQexecParams(pgsql.conn, c_str(sqls), p, NULL, sqlparams, NULL, NULL, 0);
    if (PQresultStatus(r) != PGRES_TUPLES_OK) {
        const string rs = string("Couldn't execute select postgresql SQL statement: ") + pgfailure(r, pgsql.conn);
        PQclear(r);
        return mkfailure<value>(rs);
    }
    const int n = PQntuples(r);
    if (n < 1) {
        PQclear(r);
        ostringstream os;
        os << "Couldn't get postgresql entry: " << key;
        return mkfailure<value>(str(os), 404, false);
    }

    // Return a collection of items
    if (l != 1) {
        const list<value> lval = getitems(r, 0, n, rk);
        PQclear(r);
        debug(lval, "pgsql::get::result");
        return (value)lval;
    }

    // Return a single item
    const value val = getitem(r, 0, rk);
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
    if (PQresultStatus(r) != PGRES_COMMAND_OK) {
        const string rs = string("Couldn't execute delete postgresql SQL statement: ") + pgfailure(r, pgsql.conn);
        PQclear(r);
        return mkfailure<bool>(rs);
    }
    const char* const t = PQcmdTuples(r);
    if (t != NULL && !strcmp(t, "0")) {
        PQclear(r);
        ostringstream os;
        os << "Couldn't delete postgresql entry: " << key;
        return mkfailure<bool>(str(os), 404, false);
    }
    PQclear(r);

    debug(true, "pgsql::delete::result");
    return true;
}

}
}

#endif /* tuscany_pgsql_hpp */
