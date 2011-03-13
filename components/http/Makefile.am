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

compdir=$(prefix)/components/http

EXTRA_DIST = http.composite httpget.componentType *.scm

comp_LTLIBRARIES = libhttpget.la
noinst_DATA = libhttpget.so

libhttpget_la_SOURCES = httpget.cpp
libhttpget_la_LDFLAGS = -lxml2 -lmozjs -curl
libhttpget.so:
	ln -s .libs/libhttpget.so

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs

dist_noinst_SCRIPTS = server-test
noinst_PROGRAMS = client-test
TESTS = server-test

