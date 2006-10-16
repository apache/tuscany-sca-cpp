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

libdir=$(prefix)/extensions/ws/reference/lib
lib_LTLIBRARIES = libtuscany_sca_ws_reference.la

noinst_HEADERS = tuscany/sca/ws/*.h tuscany/sca/ws/model/*.h

libtuscany_sca_ws_reference_la_SOURCES = \
tuscany/sca/ws/WSServiceBindingExtension.cpp \
tuscany/sca/ws/model/WSServiceBinding.cpp \
tuscany/sca/ws/WSServiceWrapper.cpp \
tuscany/sca/ws/Axis2Client.cpp

libtuscany_sca_ws_reference_la_LIBADD = \
	-L${TUSCANY_SDOCPP}/lib -ltuscany_sdo -ltuscany_sdo_axiom \
					-L$(top_builddir)/runtime/core/src -ltuscany_sca \
	                -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_axiom \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -laxis2_minizip \
                    -lpthread \
                    -lwoden \
                    -laxis2_http_sender \
                    -laxis2_http_receiver

INCLUDES = -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I${AXIS2C_HOME}/include
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
                   
