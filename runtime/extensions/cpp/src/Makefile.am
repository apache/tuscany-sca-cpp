libdir=$(prefix)/extensions/cpp
lib_LTLIBRARIES = libtuscany_sca_cpp.la

noinst_HEADERS = *.h model/*.h

libtuscany_sca_cpp_la_SOURCES = \
CPPExtension.cpp \
CPPImplementationExtension.cpp \
model/CPPImplementation.cpp \
CPPInterfaceExtension.cpp \
model/CPPInterface.cpp \
CPPServiceProxy.cpp \
model/CPPReferenceBinding.cpp \
CPPServiceWrapper.cpp \
model/CPPServiceBinding.cpp

libtuscany_sca_cpp_la_LIBADD = -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo \
	-L$(top_builddir)/runtime/core/src -ltuscany_sca

INCLUDES = -Imodel -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
                   