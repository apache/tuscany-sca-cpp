prgbindir=$(prefix)/bin/test
prgbin_PROGRAMS = tuscany_sca_test
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
tuscany_sca_test_SOURCES = TestSCA.cpp

tuscany_sca_test_LDADD   =  -L$(top_builddir)/runtime/core/src -ltuscany_sca \
	-L${TUSCANY_SDOCPP}/lib -ltuscany_sdo -lxml2 -lstdc++

INCLUDES = -I$(top_builddir)/runtime/core/test/MyValue \
            -I$(top_builddir)/runtime/core/src \
            -I${TUSCANY_SDOCPP}/include
