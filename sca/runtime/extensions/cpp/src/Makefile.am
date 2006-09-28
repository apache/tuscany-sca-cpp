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
tuscany/sca/cpp/model/CPPImplementation.cpp \
tuscany/sca/cpp/model/CPPInterface.cpp \
tuscany/sca/cpp/model/CPPReferenceBinding.cpp \
tuscany/sca/cpp/model/CPPServiceBinding.cpp

libtuscany_sca_cpp_la_LIBADD = -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo \
	-L$(top_builddir)/runtime/core/src -ltuscany_sca

INCLUDES = -Imodel -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
                   