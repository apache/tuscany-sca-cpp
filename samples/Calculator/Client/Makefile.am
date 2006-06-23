deploydir=$(prefix)/samples/Calculator/deploy
prgbindir=$(deploydir)/bin

prgbin_PROGRAMS = calculator_client
prgbin_SCRIPTS = runclient.sh

AM_CPPFLAGS = $(CPPFLAGS)
calculator_client_SOURCES = Calc.cpp

calculator_client_LDADD   =  -L${TUSCANY_SCACPP}/lib -ltuscany_sca \
	-L${TUSCANY_SDOCPP}/lib -ltuscany_sdo

INCLUDES =  -I$(top_builddir)/Calculator \
            -I${TUSCANY_SCACPP}/include \
            -I${TUSCANY_SDOCPP}/include
