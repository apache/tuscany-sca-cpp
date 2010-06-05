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

if WANT_QUEUE

INCLUDES = -I${QPIDC_INCLUDE}

incl_HEADERS = *.hpp
incldir = $(prefix)/include/components/queue

dist_comp_SCRIPTS = qpidd-start qpidd-stop
compdir=$(prefix)/components/queue

comp_DATA = qpidc.prefix
qpidc.prefix: $(top_builddir)/config.status
	echo ${QPIDC_PREFIX} >qpidc.prefix

EXTRA_DIST = queue.composite *.scm

comp_LTLIBRARIES = libqueue-sender.la libqueue-listener.la
noinst_DATA = libqueue-sender.so libqueue-listener.so

libqueue_sender_la_SOURCES = queue-sender.cpp
libqueue_sender_la_LDFLAGS = -L${QPIDC_LIB} -R${QPIDC_LIB} -lqpidclient -lqpidcommon
libqueue-sender.so:
	ln -s .libs/libqueue-sender.so

libqueue_listener_la_SOURCES = queue-listener.cpp
libqueue_listener_la_LDFLAGS = -L${QPIDC_LIB} -R${QPIDC_LIB} -lqpidclient -lqpidcommon
libqueue-listener.so:
	ln -s .libs/libqueue-listener.so

qpid_test_SOURCES = qpid-test.cpp
qpid_test_LDFLAGS = -L${QPIDC_LIB} -R${QPIDC_LIB} -lqpidclient -lqpidcommon

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs -L${QPIDC_LIB} -R${QPIDC_LIB} -lqpidclient -lqpidcommon

dist_noinst_SCRIPTS = send-test server-test
noinst_PROGRAMS = qpid-test client-test
TESTS = send-test server-test

endif
