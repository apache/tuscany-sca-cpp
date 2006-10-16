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

deploydir=$(prefix)/samples/Calculator/deploy
wsclientdir=$(deploydir)/sample.calculator.wsclient

wsclient_PROGRAMS = calculator_wsclient
wsclient_SCRIPTS = runwsclient.sh
EXTRA_DIST = runwsclient.sh

AM_CPPFLAGS = $(CPPFLAGS)

calculator_wsclient_SOURCES = \
axis2_Calculator_stub.cpp \
CalculatorWSClient.cpp

noinst_HEADERS = *.h

calculator_wsclient_LDADD  = \
-L$(AXIS2C_HOME)/lib \
  -laxis2_util \
  -laxis2_axiom \
  -laxis2_wsdl \
  -laxis2_engine \
  -lwoden \
  -lwoden_wsdl \
  -laxis2_xml_schema \
  -laxis2_parser \
  -laxis2_minizip \
  -lpthread \
  -laxis2_http_sender \
  -laxis2_http_receiver
	
INCLUDES =  -I${AXIS2C_HOME}/include
