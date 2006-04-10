prgbindir=$(prefix)/samples/bin
prgbin_PROGRAMS = MyValueClient

MyValueClient_SOURCES = MyValueClient.cpp

MyValueClient_LDADD   =  -L$(top_builddir)/runtime/core/src -ltuscany_sca \
	-L${TUSCANY_SDOCPP}/lib -ltuscany_sdo -lxml2

INCLUDES = -I.. \
            -I$(top_builddir)/runtime/core/src \
            -I${TUSCANY_SDOCPP}/include
