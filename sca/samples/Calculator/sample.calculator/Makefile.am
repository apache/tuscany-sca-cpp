deploydir=$(prefix)/samples/Calculator/deploy
compositedir=$(deploydir)/sample.calculator

BUILT_SOURCES = scagen

noinst_HEADERS = *.h

scagen:
	java -jar $(TUSCANY_SCACPP)/bin/scagen.jar -dir . -output .

composite_SCRIPTS = runwsserver.sh

composite_LTLIBRARIES = libCalculator.la
composite_DATA = *.composite *.componentType *.wsdl
EXTRA_DIST = *.composite *.componentType *.wsdl runwsserver.sh

dist_libCalculator_la_SOURCES = \
CalculatorImpl.cpp \
DivideImpl.cpp

nodist_libCalculator_la_SOURCES = \
CalculatorImpl_CalculatorService_Proxy.cpp \
CalculatorImpl_CalculatorService_Wrapper.cpp \
CalculatorImpl_divideService_Proxy.cpp \
DivideImpl_DivideService_Proxy.cpp \
DivideImpl_DivideService_Wrapper.cpp

libCalculator_la_LIBADD = \
-L${TUSCANY_SCACPP}/lib \
  -ltuscany_sca \
-L${TUSCANY_SCACPP}/extensions/cpp/lib \
  -ltuscany_sca_cpp

INCLUDES = \
-I$(TUSCANY_SCACPP)/extensions/cpp/include \
-I$(TUSCANY_SCACPP)/include \
-I${TUSCANY_SDOCPP}/include

