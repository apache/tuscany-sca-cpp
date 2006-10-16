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

deploydir=$(prefix)/test/ws.service.interop/deploy
compositedir=$(deploydir)/packages/ws.service.interop
prgbindir=$(deploydir)/bin

BUILT_SOURCES = TestImpl_TestService_Proxy.cpp \
TestImpl_TestService_Wrapper.cpp

noinst_HEADERS = *.h

${BUILT_SOURCES}: ws.service.interop.composite
	java -jar $(TUSCANY_SCACPP)/bin/scagen.jar -dir . -output . -verbose

prgbin_SCRIPTS = runwsserver.sh

composite_LTLIBRARIES = libws_service_interop.la
composite_DATA = ws.service.interop.composite *.componentType
EXTRA_DIST = ws.service.interop.composite *.componentType runwsserver.sh

libws_service_interop_la_SOURCES = \
TestImpl.cpp \
TestImpl_TestService_Proxy.cpp \
TestImpl_TestService_Wrapper.cpp

libws_service_interop_la_LIBADD = \
-L${TUSCANY_SCACPP}/lib \
  -ltuscany_sca \
-L${TUSCANY_SCACPP}/extensions/cpp/lib \
  -ltuscany_sca_cpp \
-L${TUSCANY_SDOCPP}/lib \
  -ltuscany_sdo

INCLUDES = \
-I$(TUSCANY_SCACPP)/extensions/cpp/include \
-I$(TUSCANY_SCACPP)/include \
-I${TUSCANY_SDOCPP}/include

