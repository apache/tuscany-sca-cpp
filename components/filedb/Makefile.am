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
incldir = $(prefix)/include/components/filedb

compdir=$(prefix)/components/filedb

EXTRA_DIST = filedb.composite filedb.componentType

comp_LTLIBRARIES = libfiledb.la
noinst_DATA = libfiledb.so

libfiledb_la_SOURCES = filedb.cpp
libfiledb_la_LDFLAGS = 
libfiledb.so:
	ln -s .libs/libfiledb.so

file_test_SOURCES = file-test.cpp
file_test_LDFLAGS = 

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs

dist_noinst_SCRIPTS = filedb-test server-test
noinst_PROGRAMS = file-test client-test
TESTS = filedb-test server-test

