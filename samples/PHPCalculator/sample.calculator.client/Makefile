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

INCLUDES = \
-I/usr/local/include/php \
-I/usr/local/include/php/sapi/embed \
-I/usr/local/include/php/Zend \
-I/usr/local/include/php/main \
-I/usr/local/include/php/TSRM \
-I${TUSCANY_SDOCPP}/include \
-I${TUSCANY_SCACPP}/include \
-I${TUSCANY_SCACPP}/extensions/cpp/include \
-I${TUSCANY_SCACPP}/samples/CppCalculator/sample.calculator

FLAGS = \
-g \
-O2 \
-MD \
-MP \
-fPIC \
-DPIC

LIBS = \
-L${LIBXML2_LIB} \
-L${$AXIS2C_HOME}/lib \
-L${PHP_LIB} \
-L${TUSCANY_SDOCPP}/lib \
-L${TUSCANY_SCACPP}/lib \
-L${TUSCANY_SCACPP}/extensions/cpp/lib \
-L${TUSCANY_SCACPP}/extensions/php/lib \
-L${TUSCANY_SCACPP}/extensions/ws/lib \
-lphp5 \
-ltuscany_sdo \
-ltuscany_sca \
-ltuscany_sca_cpp

phpembedtest:
	g++ $(INCLUDES) $(FLAGS) $(LIBS) CalculatorClient.cpp

