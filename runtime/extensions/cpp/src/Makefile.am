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

libdir=$(prefix)/extensions/cpp/lib
lib_LTLIBRARIES = libtuscany_sca_cpp.la
includedir=$(prefix)/extensions/cpp/include
nobase_include_HEADERS = \
osoa/sca/*.h \
tuscany/sca/cpp/*.h \
tuscany/sca/cpp/model/*.h

libtuscany_sca_cpp_la_SOURCES = \
osoa/sca/ComponentContext.cpp \
osoa/sca/CompositeContext.cpp \
tuscany/sca/cpp/ComponentContextImpl.cpp \
tuscany/sca/cpp/CompositeContextImpl.cpp \
tuscany/sca/cpp/CPPExtension.cpp \
tuscany/sca/cpp/CPPImplementationExtension.cpp \
tuscany/sca/cpp/CPPInterfaceExtension.cpp \
tuscany/sca/cpp/CPPServiceProxy.cpp \
tuscany/sca/cpp/CPPServiceWrapper.cpp \
tuscany/sca/cpp/TuscanyRuntime.cpp \
tuscany/sca/cpp/model/CPPImplementation.cpp \
tuscany/sca/cpp/model/CPPInterface.cpp \
tuscany/sca/cpp/model/CPPReferenceBinding.cpp \
tuscany/sca/cpp/model/CPPServiceBinding.cpp

libtuscany_sca_cpp_la_LIBADD = -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo \
	-L$(top_builddir)/runtime/core/src -ltuscany_sca

INCLUDES = -Imodel -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
                   