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

if WANT_LOG

INCLUDES = -I${THRIFT_INCLUDE} -I${FB303_INCLUDE}

incl_HEADERS = *.hpp
incldir = $(prefix)/include/components/log

dist_comp_SCRIPTS = scribed-central-conf scribed-client-conf scribed-central-start scribed-central-stop scribed-client-start scribed-client-stop scribe-tail-start scribe-tail-stop
compdir=$(prefix)/components/log

comp_DATA = scribe.prefix thrift.prefix
scribe.prefix: $(top_builddir)/config.status
	echo ${SCRIBE_PREFIX} >scribe.prefix

thrift.prefix: $(top_builddir)/config.status
	echo ${THRIFT_PREFIX} >thrift.prefix

EXTRA_DIST = log.composite log.componentType logger.componentType *.scm *.thrift

BUILT_SOURCES=gen-cpp/fb303_constants.cpp gen-cpp/fb303_types.cpp gen-cpp/scribe_constants.cpp gen-cpp/scribe.cpp gen-cpp/scribe_types.cpp gen-cpp/FacebookService.cpp gen-cpp/scribe.h
gen-cpp/fb303_constants.cpp gen-cpp/fb303_types.cpp gen-cpp/scribe_constants.cpp gen-cpp/scribe.cpp gen-cpp/scribe_types.cpp gen-cpp/FacebookService.cpp gen-cpp/scribe.h: scribe.thrift
	${THRIFT_PREFIX}/bin/thrift -r --gen cpp scribe.thrift; (ls gen-cpp/*.cpp gen-cpp/*.h | xargs -I {} -t ./thrift-pragmas {})

CLEANFILES = gen-cpp/*

comp_LTLIBRARIES = liblog.la liblogger.la
noinst_DATA = liblog${libsuffix} liblogger${libsuffix}

nodist_liblog_la_SOURCES = gen-cpp/fb303_constants.cpp gen-cpp/fb303_types.cpp gen-cpp/scribe_constants.cpp gen-cpp/scribe.cpp gen-cpp/scribe_types.cpp gen-cpp/FacebookService.cpp gen-cpp/scribe.h
liblog_la_CXXFLAGS = -Wno-unused-parameter -Wno-conversion -Wno-return-type
liblog_la_SOURCES = log.cpp
liblog_la_LDFLAGS = -L${THRIFT_LIB} -R${THRIFT_LIB} -lthrift -L${FB303_LIB} -R${FB303_LIB} -lfb303 -L${SCRIBE_LIB} -R${SCRIBE_LIB} -lscribe
liblog${libsuffix}:
	ln -s .libs/liblog${libsuffix}

nodist_liblogger_la_SOURCES = gen-cpp/fb303_constants.cpp gen-cpp/fb303_types.cpp gen-cpp/scribe_constants.cpp gen-cpp/scribe.cpp gen-cpp/scribe_types.cpp gen-cpp/FacebookService.cpp gen-cpp/scribe.h
liblogger_la_CXXFLAGS = -Wno-unused-parameter -Wno-conversion -Wno-return-type
liblogger_la_SOURCES = logger.cpp
liblogger_la_LDFLAGS = -L${THRIFT_LIB} -R${THRIFT_LIB} -lthrift -L${FB303_LIB} -R${FB303_LIB} -lfb303 -L${SCRIBE_LIB} -R${SCRIBE_LIB} -lscribe
liblogger${libsuffix}:
	ln -s .libs/liblogger${libsuffix}

comp_PROGRAMS = scribe-cat scribe-status

nodist_scribe_cat_SOURCES = gen-cpp/fb303_constants.cpp gen-cpp/fb303_types.cpp gen-cpp/scribe_constants.cpp gen-cpp/scribe.cpp gen-cpp/scribe_types.cpp gen-cpp/FacebookService.cpp gen-cpp/scribe.h
scribe_cat_CXXFLAGS = -Wno-unused-parameter -Wno-conversion -Wno-return-type
scribe_cat_SOURCES = scribe-cat.cpp
scribe_cat_LDFLAGS = -L${THRIFT_LIB} -R${THRIFT_LIB} -lthrift -L${FB303_LIB} -R${FB303_LIB} -lfb303 -L${SCRIBE_LIB} -R${SCRIBE_LIB} -lscribe

nodist_scribe_status_SOURCES = gen-cpp/fb303_constants.cpp gen-cpp/fb303_types.cpp gen-cpp/scribe_constants.cpp gen-cpp/scribe.cpp gen-cpp/scribe_types.cpp gen-cpp/FacebookService.cpp gen-cpp/scribe.h
scribe_status_CXXFLAGS = -Wno-unused-parameter -Wno-conversion -Wno-return-type
scribe_status_SOURCES = scribe-status.cpp
scribe_status_LDFLAGS = -L${THRIFT_LIB} -R${THRIFT_LIB} -lthrift -L${FB303_LIB} -R${FB303_LIB} -lfb303 -L${SCRIBE_LIB} -R${SCRIBE_LIB} -lscribe

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs

dist_noinst_SCRIPTS = scribe-test server-test
noinst_PROGRAMS = client-test
TESTS = scribe-test server-test

endif
