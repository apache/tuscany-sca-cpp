libdir=$(prefix)/extensions/sca/reference/lib
lib_LTLIBRARIES = libtuscany_sca_binding_reference.la

noinst_HEADERS = \
tuscany/sca/binding/*.h \
tuscany/sca/binding/model/*.h

libtuscany_sca_binding_reference_la_SOURCES = \
tuscany/sca/binding/SCAServiceBindingExtension.cpp \
tuscany/sca/binding/model/SCAServiceBinding.cpp

libtuscany_sca_binding_reference_la_LIBADD = \
  -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo -ltuscany_sdo_axiom \
  -L$(top_builddir)/runtime/core/src -ltuscany_sca \
  -L$(top_builddir)/runtime/extensions/ws/reference/axis2c/src -ltuscany_sca_ws_reference

INCLUDES = -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I$(top_builddir)/runtime/extensions/ws/reference/axis2c/src
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
