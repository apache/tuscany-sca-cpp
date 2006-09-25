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
