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

deploydir=$(prefix)/CppCalculator/deploy
compositedir=$(deploydir)/sample.calculator

BUILT_SOURCES = scagen

noinst_HEADERS = *.h

scagen:
	java -jar $(TUSCANY_SCACPP)/bin/scagen.jar -dir . -output .

composite_LTLIBRARIES = libCalculator.la
composite_DATA = *.composite *.componentType
EXTRA_DIST = *.composite *.componentType

dist_libCalculator_la_SOURCES = \
CalculatorImpl.cpp \
DivideImpl.cpp

nodist_libCalculator_la_SOURCES = \
CalculatorImpl_CalculatorService_Proxy.cpp \
CalculatorImpl_CalculatorService_Wrapper.cpp \
CalculatorImpl_divideService_Proxy.cpp \
DivideImpl_DivideService_Proxy.cpp \
DivideImpl_DivideService_Wrapper.cpp

libCalculator_la_LIBADD = \
-L${TUSCANY_SCACPP}/lib \
  -ltuscany_sca \
-L${TUSCANY_SCACPP}/extensions/cpp/lib \
  -ltuscany_sca_cpp

INCLUDES = \
-I$(TUSCANY_SCACPP)/extensions/cpp/include \
-I$(TUSCANY_SCACPP)/include \
-I${TUSCANY_SDOCPP}/include

