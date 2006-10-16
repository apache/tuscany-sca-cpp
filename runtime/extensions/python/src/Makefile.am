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

libdir=$(prefix)/extensions/python/lib
lib_LTLIBRARIES = libtuscany_sca_python.la

install-exec-hook:
	cd $(libdir); ln -s -f libtuscany_sca_python.so sca.so

pydir=$(prefix)/extensions/python/lib
py_DATA = tuscany/sca/python/sca_proxy.py
EXTRA_DIST = tuscany/sca/python/sca_proxy.py

includedir=$(prefix)/extensions/python/include
nobase_include_HEADERS = \
tuscany/sca/python/*.h \
tuscany/sca/python/model/*.h

libtuscany_sca_python_la_SOURCES = \
tuscany/sca/python/PythonExtension.cpp \
tuscany/sca/python/PythonImplementationExtension.cpp \
tuscany/sca/python/PythonInterfaceExtension.cpp \
tuscany/sca/python/PythonServiceWrapper.cpp \
tuscany/sca/python/PythonServiceProxy.cpp \
tuscany/sca/python/sca_module.cpp \
tuscany/sca/python/model/PythonImplementation.cpp \
tuscany/sca/python/model/PythonInterface.cpp \
tuscany/sca/python/model/PythonReferenceBinding.cpp \
tuscany/sca/python/model/PythonServiceBinding.cpp

# Need python env varibles set. e.g:
# PYTHON_LIB=/usr/lib
# PYTHON_INCLUDE=/usr/include/python2.4
# PYTHON_VERSION=python2.4
libtuscany_sca_python_la_LIBADD = -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo \
	-L$(top_builddir)/runtime/core/src -ltuscany_sca \
	-L${PYTHON_LIB} -l${PYTHON_VERSION}

INCLUDES = -Imodel -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I${PYTHON_INCLUDE}
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
                   
