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

noinst_PROGRAMS = eval-test eval-shell

datadir=$(prefix)/modules/eval
nobase_data_DATA = *.xsd

INCLUDES = -I. -I$(top_builddir)/kernel -I${LIBXML2_INCLUDE} -I${APR_INCLUDE}

eval_test_SOURCES = eval-test.cpp
eval_test_LDADD = -L${LIBXML2_LIB} -lxml2 -L${APR_LIB} -lapr-1 -laprutil-1

eval_shell_SOURCES = eval-shell.cpp
eval_shell_LDADD = -L${LIBXML2_LIB} -lxml2 -L${APR_LIB} -lapr-1 -laprutil-1

TESTS = eval-test
