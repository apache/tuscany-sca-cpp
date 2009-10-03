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

store_script_PROGRAMS = store-script-test
store_scriptdir=$(prefix)/test/store-script/deploy

INCLUDES = -I. -I$(top_builddir)/kernel -I$(top_builddir)/modules/eval -I${LIBXML2_INCLUDE}

store_script_test_SOURCES = store-script-test.cpp
store_script_test_LDADD = -L${LIBXML2_LIB} -lxml2 -lpthread
	
EXTRA_DIST = *.composite
store_script_DATA = *.composite

