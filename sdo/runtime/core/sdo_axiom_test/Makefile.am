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
prgbin_PROGRAMS = tuscany_sdo_axiom_test
SUBDIRS =
TESTS = tuscany_sdo_axiom_test
TESTS_ENVIRONMENT = LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${LIBXML2_LIB}:$(AXIS2C_HOME)/lib
AM_CPPFLAGS = $(CPPFLAGS)
tuscany_sdo_axiom_test_SOURCES = sdo_axiom_test.cpp

noinst_HEADERS=*.h

tuscany_sdo_axiom_test_LDADD   = \
	-L$(top_builddir)/runtime/core/src/commonj/sdo -ltuscany_sdo \
	-L$(top_builddir)/runtime/core/sdo_axiom -ltuscany_sdo_axiom \
	-L${LIBXML2_LIB} -lxml2 \
	-L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_axiom \
                    -laxis2_parser \
                    -laxis2_minizip

INCLUDES = -I$(top_builddir)/runtime/core/sdo_axiom_test \
           -I$(top_builddir)/runtime/core/src \
           -I$(top_builddir)/runtime/core/sdo_axiom  \
           -I${AXIS2C_HOME}/include
            