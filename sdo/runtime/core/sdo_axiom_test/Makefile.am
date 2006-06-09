prgbindir=$(prefix)/bin/test
prgbin_PROGRAMS = tuscany_sdo_axiom_test
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
tuscany_sdo_axiom_test_SOURCES = sdo_axiom_test.cpp

noinst_HEADERS=*.h

tuscany_sdo_axiom_test_LDADD   =  -ltuscany_sdo \
	-L$(top_builddir)/runtime/core/sdo_axiom -ltuscany_sdo_axiom \
	-L$(top_builddir)/runtime/core/src/commonj/sdo -lxml2 -lstdc++ \
	-L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_om \
                    -laxis2_parser \
                    -laxis2_unix

INCLUDES = -I$(top_builddir)/runtime/core/sdo_axiom_test \
           -I$(top_builddir)/runtime/core/src \
           -I$(top_builddir)/runtime/core/sdo_axiom  \
           -I${AXIS2C_HOME}/include
            