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

libdir=$(prefix)/extensions/sca/service/lib
lib_LTLIBRARIES = libtuscany_sca_binding_service.la

noinst_HEADERS = \
tuscany/sca/binding/*.h \
tuscany/sca/binding/model/*.h

libtuscany_sca_binding_service_la_SOURCES = \
tuscany/sca/binding/SCAReferenceBindingExtension.cpp \
tuscany/sca/binding/model/SCAReferenceBinding.cpp

libtuscany_sca_binding_service_la_LIBADD = \
  -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo -ltuscany_sdo_axiom \
  -L$(top_builddir)/runtime/core/src -ltuscany_sca \
  -L$(top_builddir)/runtime/extensions/ws/service/axis2c/src -ltuscany_sca_ws_service

INCLUDES = -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I$(top_builddir)/runtime/extensions/ws/service/axis2c/src
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
