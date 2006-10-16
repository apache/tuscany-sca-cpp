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

deploydir=$(prefix)/samples/misc/deploy/
prgbindir=$(deploydir)/bin

prgbin_PROGRAMS = sdo_misc
prgbin_SCRIPTS = 
EXTRA_DIST = *.xsd *.xml
deploy_DATA = *.xsd *.xml

AM_CPPFLAGS = $(CPPFLAGS)
sdo_misc_SOURCES = samples.cpp \
ChangeSummarySave.cpp \
ObjectCreation.cpp \
Query.cpp \
Substitutes.cpp \
XSDLoading.cpp 


noinst_HEADERS = *.h

sdo_misc_LDADD   = -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo -lxml2
	

INCLUDES =  -I$(top_builddir)/misc \
            -I${TUSCANY_SDOCPP}/include
