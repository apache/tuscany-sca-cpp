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

INCLUDES = -I${TINYCDB_INCLUDE}

incl_HEADERS = *.hpp
incldir = $(prefix)/include/components/constdb

dist_comp_SCRIPTS = tinycdb
compdir=$(prefix)/components/constdb

comp_DATA = tinycdb.prefix
tinycdb.prefix: $(top_builddir)/config.status
	echo ${TINYCDB_PREFIX} >tinycdb.prefix

EXTRA_DIST = constdb.composite constdb.componentType

comp_LTLIBRARIES = libconstdb.la
noinst_DATA = libconstdb${libsuffix}

libconstdb_la_SOURCES = constdb.cpp
libconstdb_la_LDFLAGS = -L${TINYCDB_LIB} -R${TINYCDB_LIB} -lcdb
libconstdb${libsuffix}:
	ln -s .libs/libconstdb${libsuffix}

tinycdb_test_SOURCES = tinycdb-test.cpp
tinycdb_test_LDFLAGS = -L${TINYCDB_LIB} -R${TINYCDB_LIB} -lcdb

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -ljansson

dist_noinst_SCRIPTS = constdb-test server-test
noinst_PROGRAMS = tinycdb-test client-test
TESTS = constdb-test server-test

