prgbindir=$(prefix)/samples/bin
prgbin_PROGRAMS = MyValueClient

MyValueClient_SOURCES = MyValueClient.cpp

MyValueClient_LDADD   =  -L$(top_builddir)/runtime/core/src -ltuscany_sca \
	-L${TUSCANY_SDOCPP}/lib -ltuscany_sdo \
	-L$(AXISCPP_DEPLOY)/lib -laxis_client -lxml2

INCLUDES = -I.. \
            -I$(top_builddir)/runtime/core/src \
            -I${TUSCANY_SDOCPP}/include \
            -I${AXISCPP_DEPLOY}/include
