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


if WANT_OPENCL

INCLUDES = -I${OPENCL_INCLUDE}

incl_HEADERS = *.hpp
incldir = $(prefix)/include/modules/opencl

dist_mod_SCRIPTS = opencl-conf
moddir = $(prefix)/modules/opencl

EXTRA_DIST = domain-test.composite server-test.cl

if DARWIN
OPENCL_FLAGS = -framework OpenCL
else
OPENCL_FLAGS = -L${OPENCL_LIB} -R${OPENCL_LIB} -lOpenCL
endif

#mod_LTLIBRARIES = libmod_tuscany_opencl.la
#libmod_tuscany_opencl_la_SOURCES = mod-opencl.cpp
#libmod_tuscany_opencl_la_LDFLAGS = -lxml2 -lcurl -lmozjs -framework OpenCL
#noinst_DATA = libmod_tuscany_opencl${libsuffix}
#libmod_tuscany_opencl${libsuffix}:
#	ln -s .libs/libmod_tuscany_opencl${libsuffix}

opencl_test_SOURCES = opencl-test.cpp
opencl_test_LDFLAGS = ${OPENCL_FLAGS}

opencl_shell_SOURCES = opencl-shell.cpp
opencl_shell_LDFLAGS = ${OPENCL_FLAGS}

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs

dist_noinst_SCRIPTS = server-test
noinst_PROGRAMS = opencl-test client-test
mod_PROGRAMS = opencl-shell
TESTS = opencl-test

endif
