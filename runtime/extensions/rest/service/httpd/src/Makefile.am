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

libdir=$(prefix)/extensions/rest/service/lib
lib_LTLIBRARIES = \
libtuscany_sca_rest_service.la \
libtuscany_sca_mod_rest.la

noinst_HEADERS = \
tuscany/sca/rest/*.h \
tuscany/sca/rest/model/*.h

libtuscany_sca_rest_service_la_SOURCES = \
tuscany/sca/rest/RESTReferenceBindingExtension.cpp \
tuscany/sca/rest/model/RESTReferenceBinding.cpp \
tuscany/sca/rest/RESTServiceProxy.cpp

libtuscany_sca_rest_service_la_LIBADD = \
  -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo \
  -L$(top_builddir)/runtime/core/src -ltuscany_sca \
  -L$(top_builddir)/runtime/extensions/rest/interface/src -ltuscany_sca_rest_interface

rootdir=$(prefix)/extensions/rest/service

libtuscany_sca_mod_rest_la_SOURCES = \
tuscany/sca/rest/ModREST.cpp

libtuscany_sca_mod_rest_la_LIBADD = \
  -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo \
  -L$(top_builddir)/runtime/core/src -ltuscany_sca \
  -L$(top_builddir)/runtime/extensions/rest/service/httpd/src -ltuscany_sca_rest_service \
  -L$(top_builddir)/runtime/extensions/rest/interface/src -ltuscany_sca_rest_interface

INCLUDES = -I$(top_builddir)/runtime/core/src \
  -I$(top_builddir)/runtime/extensions/rest/interface/src \
  -I${TUSCANY_SDOCPP}/include \
  -I${HTTPD_INCLUDE} -I${APR_INCLUDE}
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG

moduledir=$(prefix)/extensions/rest/service/module
extension = libtuscany_sca_rest_service$(libsuffix)
	                    
install-exec-hook:
	test -z $(moduledir) || $(mkdir_p) $(moduledir);
	-rm -f  $(moduledir)/$(extension)
	$(LN_S) $(libdir)/$(extension) $(moduledir)/$(extension)   