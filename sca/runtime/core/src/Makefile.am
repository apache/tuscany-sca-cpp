lib_LTLIBRARIES = libtuscany_sca.la

nobase_include_HEADERS = \
osoa/sca/ComponentContext.h \
osoa/sca/ComponentContextImpl.h \
osoa/sca/export.h \
osoa/sca/ModuleContext.h \
osoa/sca/ModuleContextImpl.h \
osoa/sca/sca.h \
osoa/sca/ServiceList.h \
osoa/sca/ServiceRuntimeException.h \
tuscany/sca/core/ComponentServiceWrapper.h \
tuscany/sca/core/ExternalServiceWrapper.h \
tuscany/sca/core/Operation.h \
tuscany/sca/core/SCAEntryPoint.h \
tuscany/sca/core/SCARuntime.h \
tuscany/sca/core/ServiceProxy.h \
tuscany/sca/core/ServiceWrapper.h \
tuscany/sca/model/Binding.h \
tuscany/sca/model/Component.h \
tuscany/sca/model/CPPImplementation.h \
tuscany/sca/model/CPPInterface.h \
tuscany/sca/model/EntryPoint.h \
tuscany/sca/model/ExternalService.h \
tuscany/sca/model/Implementation.h \
tuscany/sca/model/Interface.h \
tuscany/sca/model/ModelLoader.h \
tuscany/sca/model/Module.h \
tuscany/sca/model/Service.h \
tuscany/sca/model/ServiceReference.h \
tuscany/sca/model/Subsystem.h \
tuscany/sca/model/System.h \
tuscany/sca/model/Wire.h \
tuscany/sca/model/WireTarget.h \
tuscany/sca/model/WSBinding.h \
tuscany/sca/model/Wsdl.h \
tuscany/sca/model/WsdlOperation.h \
tuscany/sca/util/DefaultLogWriter.h \
tuscany/sca/util/Exceptions.h \
tuscany/sca/util/File.h \
tuscany/sca/util/FileLogWriter.h \
tuscany/sca/util/Library.h \
tuscany/sca/util/Logger.h \
tuscany/sca/util/Logging.h \
tuscany/sca/util/LogWriter.h \
tuscany/sca/util/Utils.h \
tuscany/sca/ws/SDOStub.h \
tuscany/sca/ws/WSServiceWrapper.h


libtuscany_sca_la_SOURCES = \
osoa/sca/ComponentContext.cpp \
osoa/sca/ComponentContextImpl.cpp \
osoa/sca/ModuleContext.cpp \
osoa/sca/ModuleContextImpl.cpp \
osoa/sca/SCA.cpp \
osoa/sca/ServiceList.cpp \
osoa/sca/ServiceRuntimeException.cpp \
tuscany/sca/util/DefaultLogWriter.cpp \
tuscany/sca/util/File.cpp \
tuscany/sca/util/FileLogWriter.cpp \
tuscany/sca/util/Library.cpp \
tuscany/sca/util/Logger.cpp \
tuscany/sca/util/LogWriter.cpp \
tuscany/sca/util/Utils.cpp \
tuscany/sca/model/Binding.cpp \
tuscany/sca/model/Component.cpp \
tuscany/sca/model/CPPImplementation.cpp \
tuscany/sca/model/CPPInterface.cpp \
tuscany/sca/model/EntryPoint.cpp \
tuscany/sca/model/ExternalService.cpp \
tuscany/sca/model/Implementation.cpp \
tuscany/sca/model/Interface.cpp \
tuscany/sca/model/ModelLoader.cpp \
tuscany/sca/model/Module.cpp \
tuscany/sca/model/Service.cpp \
tuscany/sca/model/ServiceReference.cpp \
tuscany/sca/model/Subsystem.cpp \
tuscany/sca/model/System.cpp \
tuscany/sca/model/Wire.cpp \
tuscany/sca/model/WireTarget.cpp \
tuscany/sca/model/WSBinding.cpp \
tuscany/sca/model/Wsdl.cpp \
tuscany/sca/model/WsdlOperation.cpp \
tuscany/sca/core/ComponentServiceWrapper.cpp \
tuscany/sca/core/ExternalServiceWrapper.cpp \
tuscany/sca/core/Operation.cpp \
tuscany/sca/core/SCAEntryPoint.cpp \
tuscany/sca/core/SCARuntime.cpp \
tuscany/sca/core/ServiceProxy.cpp \
tuscany/sca/core/ServiceWrapper.cpp \
tuscany/sca/ws/SDOStub.cpp \
tuscany/sca/ws/WSServiceWrapper.cpp

libtuscany_sca_la_LIBADD = -lstdc++ -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo

AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG

INCLUDES = -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I${AXISCPP_DEPLOY}/include
                   
