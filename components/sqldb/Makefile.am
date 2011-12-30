#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#    
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.

if WANT_SQLDB

INCLUDES = -I${PGSQL_INCLUDE}

incl_HEADERS = *.hpp
incldir = $(prefix)/include/components/sqldb

dist_comp_SCRIPTS = pgsql-conf pgsql-log-conf pgsql-start pgsql-stop pgsql pgsql-standby-conf pgsql-backup
compdir=$(prefix)/components/sqldb

comp_DATA = pgsql.prefix
pgsql.prefix: $(top_builddir)/config.status
	echo ${PGSQL_PREFIX} >pgsql.prefix

EXTRA_DIST = sqldb.composite sqldb.componentType

comp_LTLIBRARIES = libsqldb.la
noinst_DATA = libsqldb${libsuffix}

libsqldb_la_SOURCES = sqldb.cpp
libsqldb_la_LDFLAGS = -L${PGSQL_LIB} -R${PGSQL_LIB} -lpq
libsqldb${libsuffix}:
	ln -s .libs/libsqldb${libsuffix}

pgsql_test_SOURCES = pgsql-test.cpp
pgsql_test_LDFLAGS = -L${PGSQL_LIB} -R${PGSQL_LIB} -lpq

pgsql_standby_test_SOURCES = pgsql-standby-test.cpp
pgsql_standby_test_LDFLAGS = -L${PGSQL_LIB} -R${PGSQL_LIB} -lpq

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs

dist_noinst_SCRIPTS = sqldb-test standby-test server-test
noinst_PROGRAMS = pgsql-test pgsql-standby-test client-test
TESTS = sqldb-test standby-test server-test

endif
