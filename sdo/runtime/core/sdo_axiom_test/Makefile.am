prgbindir=$(prefix)/bin/test
prgbin_PROGRAMS = tuscany_sdo_axiom_test
SUBDIRS =
TESTS = tuscany_sdo_axiom_test
TESTS_ENVIRONMENT = LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${LIBXML2_LIB}:$(AXIS2C_HOME)/lib
AM_CPPFLAGS = $(CPPFLAGS)
tuscany_sdo_axiom_test_SOURCES = sdo_axiom_test.cpp

noinst_HEADERS=*.h

tuscany_sdo_axiom_test_LDADD   = \
	-L$(top_builddir)/runtime/core/src/commonj/sdo -ltuscany_sdo \
	-L$(top_builddir)/runtime/core/sdo_axiom -ltuscany_sdo_axiom \
	-L${LIBXML2_LIB} -lxml2 \
	-L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_axiom \
                    -laxis2_parser \
                    -laxis2_minizip

INCLUDES = -I$(top_builddir)/runtime/core/sdo_axiom_test \
           -I$(top_builddir)/runtime/core/src \
           -I$(top_builddir)/runtime/core/sdo_axiom  \
           -I${AXIS2C_HOME}/include
            