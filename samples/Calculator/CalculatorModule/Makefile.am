deploydir=$(prefix)/samples/Calculator/deploy
moduledir=$(deploydir)/modules/CalculatorModule

BUILT_SOURCES = CalculatorImpl_CalculatorService_Proxy.cpp CalculatorImpl_CalculatorService_Wrapper.cpp
noinst_HEADERS = *.h

CalculatorImpl_CalculatorService_Proxy.cpp:
	java -jar $(TUSCANY_SCACPP)/bin/scagen.jar -dir . -output .

module_LTLIBRARIES = libCalculatorModule.la
module_DATA = sca.module.lin Tuscany-model.config *.componentType *.wsdl
EXTRA_DIST = sca.module.lin Tuscany-model.config *.componentType *.wsdl services.xml

libCalculatorModule_la_SOURCES = \
CalculatorImpl.cpp \
CalculatorImpl_CalculatorService_Proxy.cpp \
CalculatorImpl_CalculatorService_Wrapper.cpp

libCalculatorModule_la_LIBADD = -L${TUSCANY_SCACPP}/lib -ltuscany_sca

INCLUDES = \
           -I$(TUSCANY_SCACPP)/include \
           -I${TUSCANY_SDOCPP}/include

install-data-hook:
	mv $(moduledir)/sca.module.lin $(moduledir)/sca.module