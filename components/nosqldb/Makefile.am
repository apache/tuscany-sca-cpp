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

if WANT_NOSQLDB

INCLUDES = -I${TINYCDB_INCLUDE}

comp_SCRIPTS = tinycdb
compdir=$(prefix)/components/nosqldb

comp_DATA = tinycdb.prefix
tinycdb.prefix: $(top_builddir)/config.status
	echo ${TINYCDB_PREFIX} >tinycdb.prefix

comp_LTLIBRARIES = libnosqldb.la
noinst_DATA = libnosqldb.so

libnosqldb_la_SOURCES = nosqldb.cpp
libnosqldb_la_LDFLAGS = -L${TINYCDB_LIB} -R${TINYCDB_LIB} -lcdb
libnosqldb.so:
	ln -s .libs/libnosqldb.so

tinycdb_test_SOURCES = tinycdb-test.cpp
tinycdb_test_LDFLAGS = -L${TINYCDB_LIB} -R${TINYCDB_LIB} -lcdb

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs

noinst_PROGRAMS = tinycdb-test client-test
TESTS = nosqldb-test server-test

endif
