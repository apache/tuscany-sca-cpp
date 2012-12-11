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

INCLUDES = -I${LEVELDB_INCLUDE}

incl_HEADERS = *.hpp
incldir = $(prefix)/include/components/kvdb

dist_comp_SCRIPTS = leveldb
compdir=$(prefix)/components/kvdb

comp_DATA = leveldb.prefix
leveldb.prefix: $(top_builddir)/config.status
	echo ${LEVELDB_PREFIX} >leveldb.prefix

EXTRA_DIST = kvdb.composite kvdb.componentType

comp_LTLIBRARIES = libkvdb.la
noinst_DATA = libkvdb${libsuffix}

libkvdb_la_SOURCES = kvdb.cpp
libkvdb_la_LDFLAGS = -L${LEVELDB_LIB} -R${LEVELDB_LIB} -lleveldb
libkvdb${libsuffix}:
	ln -s .libs/libkvdb${libsuffix}

leveldb_test_SOURCES = leveldb-test.cpp
leveldb_test_LDFLAGS = -L${LEVELDB_LIB} -R${LEVELDB_LIB} -lleveldb

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs

dist_noinst_SCRIPTS = kvdb-test server-test
noinst_PROGRAMS = leveldb-test client-test
TESTS = kvdb-test server-test

