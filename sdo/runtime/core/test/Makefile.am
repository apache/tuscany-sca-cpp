prgbindir=$(prefix)/bin/test
prgbin_PROGRAMS = tuscany_sdo_test
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
tuscany_sdo_test_SOURCES = sdotest.cpp sdotest2.cpp utils.cpp main.cpp

EXTRA_DIST = *.xsd *.xml *.txt *.wsdl test/*.* test2/*.* t2/*.* g/*.* bugs/1/*.*
noinst_HEADERS=sdotest.h

tuscany_sdo_test_LDADD  =  \
    -L$(top_builddir)/runtime/core/src/commonj/sdo -ltuscany_sdo \
    -L${LIBXML2_LIB} -lxml2
	

INCLUDES = -I$(top_builddir)/runtime/core/test \
            -I$(top_builddir)/runtime/core/src
