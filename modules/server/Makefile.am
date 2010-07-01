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

INCLUDES = -I${HTTPD_INCLUDE}

incl_HEADERS = *.hpp
incldir = $(prefix)/include/modules/server

dist_mod_SCRIPTS = cpp-conf scheme-conf server-conf
moddir = $(prefix)/modules/server

EXTRA_DIST = domain-test.composite client-test.scm server-test.scm htdocs/*.xml htdocs/*.txt htdocs/*.html htdocs/js/*.js

mod_LTLIBRARIES = libmod_tuscany_eval.la libmod_tuscany_wiring.la
noinst_DATA = libmod_tuscany_eval.so libmod_tuscany_wiring.so

libmod_tuscany_eval_la_SOURCES = mod-eval.cpp
libmod_tuscany_eval_la_LDFLAGS = -lxml2 -lcurl -lmozjs
libmod_tuscany_eval.so:
	ln -s .libs/libmod_tuscany_eval.so

libmod_tuscany_wiring_la_SOURCES = mod-wiring.cpp
libmod_tuscany_wiring_la_LDFLAGS = -lxml2 -lcurl -lmozjs
libmod_tuscany_wiring.so:
	ln -s .libs/libmod_tuscany_wiring.so

noinst_test_LTLIBRARIES = libimpl-test.la
noinst_testdir = `pwd`/tmp
noinst_DATA += libimpl-test.so

libimpl_test_la_SOURCES = impl-test.cpp
libimpl-test.so:
	ln -s .libs/libimpl-test.so

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs

dist_noinst_SCRIPTS = httpd-test server-test wiring-test
noinst_PROGRAMS = client-test
TESTS = httpd-test server-test wiring-test

