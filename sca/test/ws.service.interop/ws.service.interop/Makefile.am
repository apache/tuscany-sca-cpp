deploydir=$(prefix)/test/ws.service.interop/deploy
compositedir=$(deploydir)/packages/ws.service.interop
prgbindir=$(deploydir)/bin

BUILT_SOURCES = TestImpl_TestService_Proxy.cpp \
TestImpl_TestService_Wrapper.cpp

noinst_HEADERS = *.h

${BUILT_SOURCES}: ws.service.interop.composite
	java -jar $(TUSCANY_SCACPP)/bin/scagen.jar -dir . -output . -verbose

prgbin_SCRIPTS = runwsserver.sh

composite_LTLIBRARIES = libws_service_interop.la
composite_DATA = ws.service.interop.composite *.componentType
EXTRA_DIST = ws.service.interop.composite *.componentType runwsserver.sh

libws_service_interop_la_SOURCES = \
TestImpl.cpp \
TestImpl_TestService_Proxy.cpp \
TestImpl_TestService_Wrapper.cpp

libws_service_interop_la_LIBADD = \
-L${TUSCANY_SCACPP}/lib \
  -ltuscany_sca \
-L${TUSCANY_SCACPP}/extensions/cpp/lib \
  -ltuscany_sca_cpp \
-L${TUSCANY_SDOCPP}/lib \
  -ltuscany_sdo

INCLUDES = \
-I$(TUSCANY_SCACPP)/extensions/cpp/include \
-I$(TUSCANY_SCACPP)/include \
-I${TUSCANY_SDOCPP}/include

