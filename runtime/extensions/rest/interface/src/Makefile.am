libdir=$(prefix)/extensions/rest/interface/lib
lib_LTLIBRARIES = libtuscany_sca_rest_interface.la

noinst_HEADERS = tuscany/sca/rest/*.h tuscany/sca/rest/model/*.h

libtuscany_sca_rest_interface_la_SOURCES = \
tuscany/sca/rest/RESTInterfaceExtension.cpp \
tuscany/sca/rest/model/RESTInterface.cpp

libtuscany_sca_rest_interface_la_LIBADD = \
  -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo \
  -L$(top_builddir)/runtime/core/src -ltuscany_sca

INCLUDES = -I$(top_builddir)/runtime/core/src \
  -I${TUSCANY_SDOCPP}/include
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
                   