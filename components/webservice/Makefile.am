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

comp_DATA = axis2c.prefix
compdir=$(prefix)/components/webservice
axis2c.prefix: $(top_builddir)/config.status
	echo ${AXIS2C_PREFIX} >axis2c.prefix

comp_LTLIBRARIES = libwebservice-client.la libwebservice-listener.la libaxis2-dispatcher.la libaxis2-service.la

libwebservice_client_la_SOURCES = webservice-client.cpp
libwebservice_client_la_LDFLAGS = -lxml2 -L${AXIS2C_LIB} -R${AXIS2C_LIB} -laxis2_engine -laxis2_axiom -laxutil

libwebservice_listener_la_SOURCES = webservice-listener.cpp
libwebservice_listener_la_LDFLAGS = -lxml2 -L${AXIS2C_LIB} -R${AXIS2C_LIB} -laxis2_engine -laxis2_axiom -laxutil

libaxis2_dispatcher_la_SOURCES = axis2-dispatcher.cpp
libaxis2_dispatcher_la_LDFLAGS = -lxml2 -L${AXIS2C_LIB} -R${AXIS2C_LIB} -laxis2_engine -laxis2_axiom -laxutil

libaxis2_service_la_SOURCES = axis2-service.cpp
libaxis2_service_la_LDFLAGS = -lxml2 -L${AXIS2C_LIB} -R${AXIS2C_LIB} -laxis2_engine -laxis2_axiom -laxutil

axiom_test_SOURCES = axiom-test.cpp
axiom_test_LDFLAGS = -lxml2 -L${AXIS2C_LIB} -R${AXIS2C_LIB} -laxis2_engine -laxis2_axiom -laxutil

axis2_test_SOURCES = axis2-test.cpp
axis2_test_LDFLAGS = -lxml2 -L${AXIS2C_LIB} -R${AXIS2C_LIB} -laxis2_engine -laxis2_axiom -laxutil

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs -L${AXIS2C_LIB} -R${AXIS2C_LIB} -laxis2_engine -laxis2_axiom -laxutil

noinst_PROGRAMS = axiom-test axis2-test client-test
TESTS = axiom-test echo-test server-test

endif
