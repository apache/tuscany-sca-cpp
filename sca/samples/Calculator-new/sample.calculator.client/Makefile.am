deploydir=$(prefix)/samples/Calculator/deploy
prgbindir=$(deploydir)/bin

prgbin_PROGRAMS = calculator_client
prgbin_SCRIPTS = runclient.sh
EXTRA_DIST = runclient.sh

AM_CPPFLAGS = $(CPPFLAGS)
calculator_client_SOURCES = \
CalculatorClient.cpp

calculator_client_LDADD = \
-L${TUSCANY_SCACPP}/lib \
  -ltuscany_sca \
-L${TUSCANY_SCACPP}/extensions/cpp/lib \
  -ltuscany_sca_cpp \
-L${TUSCANY_SDOCPP}/lib \
  -ltuscany_sdo \
  -ltuscany_sdo_axiom \
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
	
INCLUDES = \
-I$(TUSCANY_SCACPP)/extensions/cpp/include \
-I${TUSCANY_SCACPP}/include \
-I${TUSCANY_SDOCPP}/include
