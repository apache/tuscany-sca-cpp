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

lib_LTLIBRARIES = libtuscany_sca.la

nobase_include_HEADERS = \
tuscany/sca/*.h \
tuscany/sca/core/*.h \
tuscany/sca/model/*.h \
tuscany/sca/util/*.h \
tuscany/sca/extension/*.h

## To list the source files execute the following:
## ls -1 tuscany/sca/util/*.cpp tuscany/sca/extension/*.cpp tuscany/sca/model/*.cpp tuscany/sca/core/*.cpp tuscany/sca/cpp/*.cpp tuscany/sca/ws/*.cpp | awk '{ print $1 " \\" }'
## and copy/paste the output below
libtuscany_sca_la_SOURCES = \
tuscany/sca/core/Operation.cpp \
tuscany/sca/core/SCARuntime.cpp \
tuscany/sca/core/ServiceProxy.cpp \
tuscany/sca/core/ServiceWrapper.cpp \
tuscany/sca/extension/ImplementationExtension.cpp \
tuscany/sca/extension/InterfaceExtension.cpp \
tuscany/sca/extension/ReferenceBindingExtension.cpp \
tuscany/sca/extension/ServiceBindingExtension.cpp \
tuscany/sca/model/Binding.cpp \
tuscany/sca/model/Component.cpp \
tuscany/sca/model/ComponentType.cpp \
tuscany/sca/model/Composite.cpp \
tuscany/sca/model/CompositeReference.cpp \
tuscany/sca/model/CompositeReferenceBinding.cpp \
tuscany/sca/model/CompositeService.cpp \
tuscany/sca/model/Contract.cpp \
tuscany/sca/model/Interface.cpp \
tuscany/sca/model/ModelLoader.cpp \
tuscany/sca/model/ReferenceBinding.cpp \
tuscany/sca/model/Reference.cpp \
tuscany/sca/model/ReferenceType.cpp \
tuscany/sca/model/ServiceBinding.cpp \
tuscany/sca/model/Service.cpp \
tuscany/sca/model/ServiceType.cpp \
tuscany/sca/model/Wire.cpp \
tuscany/sca/model/WSDLDefinition.cpp \
tuscany/sca/model/WSDLInterface.cpp \
tuscany/sca/model/WSDLOperation.cpp \
tuscany/sca/util/DefaultLogWriter.cpp \
tuscany/sca/util/Exceptions.cpp \
tuscany/sca/util/File.cpp \
tuscany/sca/util/FileLogWriter.cpp \
tuscany/sca/util/Library.cpp \
tuscany/sca/util/Logger.cpp \
tuscany/sca/util/LogWriter.cpp \
tuscany/sca/util/SDOUtils.cpp \
tuscany/sca/util/Utils.cpp

libtuscany_sca_la_LIBADD = -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo

INCLUDES = -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include

AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
                   