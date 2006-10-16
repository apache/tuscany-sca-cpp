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

prgbindir=$(prefix)/bin/test
prgbin_PROGRAMS = tuscany_sdo_test
SUBDIRS =
TESTS=tuscany_sdo_test
AM_CPPFLAGS = $(CPPFLAGS)
tuscany_sdo_test_SOURCES = sdotest.cpp sdotest2.cpp utils.cpp main.cpp

EXTRA_DIST = *.xsd *.xml *.txt *.wsdl test/*.* test2/*.* t2/*.* g/*.* bugs/1/*.*
noinst_HEADERS=sdotest.h

tuscany_sdo_test_LDADD  =  \
    -L$(top_builddir)/runtime/core/src/commonj/sdo -ltuscany_sdo \
    -L${LIBXML2_LIB} -lxml2
	

INCLUDES = -I$(top_builddir)/runtime/core/test \
            -I$(top_builddir)/runtime/core/src
