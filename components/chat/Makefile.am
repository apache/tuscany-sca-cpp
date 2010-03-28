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

JAVAROOT = $(top_builddir)/components/chat

if WANT_CHAT

INCLUDES = -I${LIBSTROPHE_INCLUDE} -I${LIBSTROPHE_INCLUDE}/src

comp_SCRIPTS = vysper-start vysper-stop vysper-classpath
compdir=$(prefix)/components/chat

comp_DATA = vysper.prefix
vysper.prefix: $(top_builddir)/config.status
	echo ${VYSPER_PREFIX} >vysper.prefix

comp_LTLIBRARIES = libchatter.la
libchatter_la_SOURCES = chatter.cpp
libchatter_la_LDFLAGS = -L${LIBSTROPHE_LIB} -R${LIBSTROPHE_LIB} -lstrophe -lexpat -lssl -lresolv

xmpp_test_SOURCES = xmpp-test.cpp
xmpp_test_LDFLAGS = -L${LIBSTROPHE_LIB} -R${LIBSTROPHE_LIB} -lstrophe -lexpat -lssl -lresolv

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs -L${LIBSTROPHE_LIB} -R${LIBSTROPHE_LIB} -lstrophe -lexpat -lssl -lresolv

noinst_PROGRAMS = xmpp-test client-test

if WANT_VYSPER

AM_JAVACFLAGS = -cp `${top_builddir}/components/chat/vysper-classpath ${VYSPER_PREFIX}`${JAVAROOT}

noinst_JAVA = test/*.java

CLEANFILES = test/*.class

TESTS = echo-test 
endif

endif