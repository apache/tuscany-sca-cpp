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

datadir=$(prefix)/modules/python
libdir=$(prefix)/lib

if WANT_PYTHON

noinst_PROGRAMS = python-test python-shell client-test

lib_LTLIBRARIES = libmod_tuscany_python.la

nobase_data_DATA = *.xsd

INCLUDES = -I${PYTHON_INCLUDE}

libmod_tuscany_python_la_SOURCES = mod-python.cpp
libmod_tuscany_python_la_LDFLAGS = -lxml2 -lcurl -lmozjs -L${PYTHON_LIB} -R${PYTHON_LIB} -lpython2.6

python_test_SOURCES = python-test.cpp
python_test_LDFLAGS = -L${PYTHON_LIB} -R${PYTHON_LIB} -lpython2.6

python_shell_SOURCES = python-shell.cpp
python_shell_LDFLAGS = -L${PYTHON_LIB} -R${PYTHON_LIB} -lpython2.6

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs

TESTS = python-test server-test

endif
