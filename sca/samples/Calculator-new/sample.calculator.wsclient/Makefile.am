deploydir=$(prefix)/samples/Calculator/deploy
prgbindir=$(deploydir)/bin

prgbin_PROGRAMS = calculator_wsclient
prgbin_SCRIPTS = runwsclient.sh
EXTRA_DIST = runwsclient.sh

AM_CPPFLAGS = $(CPPFLAGS)

calculator_wsclient_SOURCES = \
axis2_Calculator_stub.cpp \
CalculatorWSClient.cpp

noinst_HEADERS = *.h

calculator_wsclient_LDADD  = \
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
	
INCLUDES =  -I${AXIS2C_HOME}/include
