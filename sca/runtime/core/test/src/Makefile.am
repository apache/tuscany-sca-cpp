prgbindir=$(prefix)/bin/test
prgbin_PROGRAMS = tuscany_sca_test
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -DEBUG
tuscany_sca_test_SOURCES = TestSCA.cpp

tuscany_sca_test_LDADD   =  -L$(top_builddir)/runtime/core/src -ltuscany_sca \
	-L${TUSCANY_SDOCPP}/lib -ltuscany_sdo \
                    -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_axiom \
                    -laxis2_parser
	

INCLUDES = -I$(top_builddir)/runtime/core/test/MyValue \
            -I$(top_builddir)/runtime/core/src \
            -I$(top_builddir)/runtime/extensions \
            -I${TUSCANY_SDOCPP}/include
