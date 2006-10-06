deploydir=$(prefix)/samples/BigBank/deploy
wsclientdir=$(deploydir)/bigbank.wsclient

wsclient_PROGRAMS = account_wsclient
wsclient_SCRIPTS = runwsclient.sh
EXTRA_DIST = runwsclient.sh

AM_CPPFLAGS = $(CPPFLAGS)
account_wsclient_SOURCES = axis2_Account_stub.cpp AccountWSClient.cpp
noinst_HEADERS = *.h

account_wsclient_LDADD   =  -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_axiom \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -lwoden \
                    -lwoden_wsdl \
                    -laxis2_xml_schema \
                    -laxis2_parser \
                    -laxis2_minizip \
                    -lpthread \
                    -laxis2_http_sender \
                    -laxis2_http_receiver
	

INCLUDES =  -I${AXIS2C_HOME}/include
