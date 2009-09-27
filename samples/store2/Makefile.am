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

store2_PROGRAMS = store2-test
store2dir=$(prefix)/store2/deploy

store2_test_SOURCES = store2-test.cpp

INCLUDES = -I. -I${TUSCANY_SCACPP}/include
store2_test_LDADD = -L${TUSCANY_SCACPP}/lib -ltuscany_sca -L${LIBXML2_LIB} -lxml2
	
EXTRA_DIST = *.composite
store2_DATA = *.composite
