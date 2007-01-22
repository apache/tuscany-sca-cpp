libdir=$(prefix)/extensions/rest/reference/lib
lib_LTLIBRARIES = libtuscany_sca_rest_reference.la

noinst_HEADERS = tuscany/sca/rest/*.h tuscany/sca/rest/model/*.h

libtuscany_sca_rest_reference_la_SOURCES = \
tuscany/sca/rest/RESTServiceBindingExtension.cpp \
tuscany/sca/rest/model/RESTServiceBinding.cpp \
tuscany/sca/rest/RESTServiceWrapper.cpp

libtuscany_sca_rest_reference_la_LIBADD = \
  -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo \
  -L$(top_builddir)/runtime/core/src -ltuscany_sca \
  -L${CURL_LIB} -lcurl

INCLUDES = -I$(top_builddir)/runtime/core/src \
  -I${TUSCANY_SDOCPP}/include \
  -I${CURL_INCLUDE}
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG


