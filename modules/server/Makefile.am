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

noinst_PROGRAMS = client-test

libdir=$(prefix)/lib
lib_LTLIBRARIES = libmod_tuscany_eval.la libmod_tuscany_wiring.la

INCLUDES = -I. -I$(top_builddir)/kernel -I${LIBXML2_INCLUDE} -I${HTTPD_INCLUDE} -I${APR_INCLUDE} -I${JS_INCLUDE} -I${CURL_INCLUDE}

libmod_tuscany_eval_la_SOURCES = mod-eval.cpp
libmod_tuscany_eval_la_CXXFLAGS = $(CXXFLAGS) -D_REENTRANT
libmod_tuscany_eval_la_LIBADD = -lpthread -L${LIBXML2_LIB} -lxml2 -L${APR_LIB} -lapr-1 -laprutil-1 -L${CURL_LIB} -lcurl -L${JS_LIB} -lmozjs

libmod_tuscany_wiring_la_SOURCES = mod-wiring.cpp
libmod_tuscany_wiring_la_CXXFLAGS = $(CXXFLAGS) -D_REENTRANT
libmod_tuscany_wiring_la_LIBADD = -lpthread -L${LIBXML2_LIB} -lxml2 -L${APR_LIB} -lapr-1 -laprutil-1 -L${CURL_LIB} -lcurl -L${JS_LIB} -lmozjs

testdir=$(prefix)/test
test_LTLIBRARIES = libimpl-test.la

libimpl_test_la_SOURCES = impl-test.cpp
libimpl_test_la_CXXFLAGS = $(CXXFLAGS) -D_REENTRANT
libimpl_test_la_LIBADD = -lpthread -L${APR_LIB} -lapr-1 -laprutil-1

client_test_SOURCES = client-test.cpp
client_test_LDADD = -L${LIBXML2_LIB} -lxml2 -L${APR_LIB} -lapr-1 -laprutil-1 -L${CURL_LIB} -lcurl -L${JS_LIB} -lmozjs

TESTS = httpd-test server-test wiring-test
