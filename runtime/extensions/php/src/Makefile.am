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

libdir=$(prefix)/extensions/php/lib
lib_LTLIBRARIES = libtuscany_sca_php.la

noinst_HEADERS = \
tuscany/sca/php/*.h \
tuscany/sca/php/model/*.h

libtuscany_sca_php_la_SOURCES = \
tuscany/sca/php/PHPExtension.cpp \
tuscany/sca/php/PHPImplementationExtension.cpp \
tuscany/sca/php/PHPServiceWrapper.cpp \
tuscany/sca/php/PHPServiceProxy.cpp \
tuscany/sca/php/sca.cpp \
tuscany/sca/php/model/PHPImplementation.cpp \
tuscany/sca/php/model/PHPReferenceBinding.cpp \
tuscany/sca/php/model/PHPServiceBinding.cpp

libtuscany_sca_php_la_LIBADD = -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo \
	-L$(top_builddir)/runtime/core/src -ltuscany_sca \
	-L${PHP_LIB} -lphp5 \
	-L${PHP_SCA_SDO_LIB} -lsdo

INCLUDES = -Imodel -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I${PHP_INCLUDE} \
           -I${PHP_INCLUDE}/main \
           -I${PHP_INCLUDE}/Zend \
           -I${PHP_INCLUDE}/TSRM \
           -I${PHP_INCLUDE}/sapi/embed \
           -I${PHP_SCA_SDO_INCLUDE}
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG 
                   
