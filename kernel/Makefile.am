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

noinst_PROGRAMS = kernel-test xml-test

nobase_include_HEADERS = *.hpp

INCLUDES = -I. -I$(top_builddir)/kernel -I${LIBXML2_INCLUDE}

kernel_test_SOURCES = kernel-test.cpp
kernel_test_LDADD = -lpthread -L${LIBXML2_LIB} -lxml2

xml_test_SOURCES = xml-test.cpp
xml_test_LDADD = -lpthread -L${LIBXML2_LIB} -lxml2

