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


if WANT_PYTHON

INCLUDES = -I${PYTHON_INCLUDE}

incl_HEADERS = *.hpp
incldir = $(prefix)/include/modules/python

dist_mod_SCRIPTS = python-conf
moddir = $(prefix)/modules/python

prefix_DATA = python.prefix
prefixdir = $(prefix)/modules/python
python.prefix: $(top_builddir)/config.status
	echo ${PYTHON_PREFIX} >python.prefix

EXTRA_DIST = domain-test.composite client-test.py server-test.py

mod_LTLIBRARIES = libmod_tuscany_python.la
libmod_tuscany_python_la_SOURCES = mod-python.cpp
libmod_tuscany_python_la_LDFLAGS = -lxml2 -lcurl -ljansson -L${PYTHON_LIB} -R${PYTHON_LIB} -lpython${PYTHON_VERSION}
noinst_DATA = libmod_tuscany_python${libsuffix}
libmod_tuscany_python${libsuffix}:
	ln -s .libs/libmod_tuscany_python${libsuffix}

python_test_SOURCES = python-test.cpp
python_test_LDFLAGS = -L${PYTHON_LIB} -R${PYTHON_LIB} -lpython${PYTHON_VERSION}

python_shell_SOURCES = python-shell.cpp
python_shell_LDFLAGS = -L${PYTHON_LIB} -R${PYTHON_LIB} -lpython${PYTHON_VERSION}

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -ljansson

dist_noinst_SCRIPTS = server-test wiring-test
noinst_PROGRAMS = python-test client-test
mod_PROGRAMS = python-shell
TESTS = python-test server-test

endif
