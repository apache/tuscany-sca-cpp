prgbindir=$(prefix)/bin/test
prgbin_PROGRAMS = tuscany_sdo_test
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
tuscany_sdo_test_SOURCES = sdotest.cpp sdotest2.cpp

tuscany_sdo_test_LDADD   =  -ltuscany_sdo \
	-L$(top_builddir)/runtime/core/src/commonj/sdo -lxml2 -lstdc++

INCLUDES = -I$(top_builddir)/runtime/core/test \
            -I$(top_builddir)/runtime/core/src
