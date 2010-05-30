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

if WANT_WEBSERVICE

INCLUDES = -I${AXIS2C_INCLUDE}

dist_comp_SCRIPTS = axis2-conf
compdir=$(prefix)/components/webservice

comp_DATA = axis2c.prefix axis2.xml services.xml module.xml

EXTRA_DIST = webservice.composite axis2.xml services.xml module.xml *.scm

axis2c.prefix: $(top_builddir)/config.status
	echo ${AXIS2C_PREFIX} >axis2c.prefix

comp_LTLIBRARIES = libwebservice-client.la libwebservice-listener.la libaxis2-dispatcher.la libaxis2-service.la
noinst_DATA = libwebservice-client.so libwebservice-listener.so libaxis2-dispatcher.so libaxis2-service.so

libwebservice_client_la_SOURCES = webservice-client.cpp
libwebservice_client_la_LDFLAGS = -lxml2 -L${AXIS2C_LIB} -R${AXIS2C_LIB} -laxis2_engine -laxis2_axiom -laxutil
libwebservice-client.so:
	ln -s .libs/libwebservice-client.so

libwebservice_listener_la_SOURCES = webservice-listener.cpp
libwebservice_listener_la_LDFLAGS = -lxml2 -L${AXIS2C_LIB} -R${AXIS2C_LIB} -laxis2_engine -laxis2_axiom -laxutil
libwebservice-listener.so:
	ln -s .libs/libwebservice-listener.so

libaxis2_dispatcher_la_SOURCES = axis2-dispatcher.cpp
libaxis2_dispatcher_la_LDFLAGS = -lxml2 -L${AXIS2C_LIB} -R${AXIS2C_LIB} -laxis2_engine -laxis2_axiom -laxutil
libaxis2-dispatcher.so:
	ln -s .libs/libaxis2-dispatcher.so

libaxis2_service_la_SOURCES = axis2-service.cpp
libaxis2_service_la_LDFLAGS = -lxml2 -L${AXIS2C_LIB} -R${AXIS2C_LIB} -laxis2_engine -laxis2_axiom -laxutil
libaxis2-service.so:
	ln -s .libs/libaxis2-service.so

axiom_test_SOURCES = axiom-test.cpp
axiom_test_LDFLAGS = -lxml2 -L${AXIS2C_LIB} -R${AXIS2C_LIB} -laxis2_engine -laxis2_axiom -laxutil

axis2_test_SOURCES = axis2-test.cpp
axis2_test_LDFLAGS = -lxml2 -L${AXIS2C_LIB} -R${AXIS2C_LIB} -laxis2_engine -laxis2_axiom -laxutil

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs -L${AXIS2C_LIB} -R${AXIS2C_LIB} -laxis2_engine -laxis2_axiom -laxutil

dist_noinst_SCRIPTS = echo-test server-test
noinst_PROGRAMS = axiom-test axis2-test client-test
TESTS = axiom-test echo-test server-test

endif
