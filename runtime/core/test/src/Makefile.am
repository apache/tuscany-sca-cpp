prgbindir=$(prefix)/bin/test
prgbin_PROGRAMS = tuscany_sca_test
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS)
tuscany_sca_test_SOURCES = TestSCA.cpp

tuscany_sca_test_LDADD   =  -L$(top_builddir)/runtime/core/src -ltuscany_sca \
	-L${TUSCANY_SDOCPP}/lib -ltuscany_sdo -ltuscany_sdo_axiom \
	-L$(top_builddir)/runtime/extensions/ws_reference/axis2c/src -ltuscany_sca_ws_reference
	                -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_axiom \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -laxis2_minizip \
                    -lpthread \
                    -laxis2_http_sender \
                    -laxis2_http_receiver
	

INCLUDES = -I$(top_builddir)/runtime/core/test/MyValue \
            -I$(top_builddir)/runtime/core/src \
            -I$(top_builddir)/runtime/extensions \
            -I${TUSCANY_SDOCPP}/include
