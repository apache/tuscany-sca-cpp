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

noinst_PROGRAMS = webservice-test client-test

INCLUDES = -I${AXIS2C_INCLUDE}

compdir=$(prefix)/components/webservice
comp_LTLIBRARIES = libwebservice.la

libwebservice_la_SOURCES = webservice.cpp
libwebservice_la_LIBADD = -lxml2 -L${AXIS2C_LIB} -laxis2_engine

webservice_test_SOURCES = webservice-test.cpp
webservice_test_LDADD = -lxml2 -L${AXIS2C_LIB} -laxis2_engine

client_test_SOURCES = client-test.cpp
client_test_LDADD = -lxml2 -lcurl -lmozjs

TESTS = webservice-test client-test

endif