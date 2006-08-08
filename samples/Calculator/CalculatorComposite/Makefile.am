deploydir=$(prefix)/samples/Calculator/deploy
compositedir=$(deploydir)/composites/CalculatorComposite

BUILT_SOURCES = CalculatorImpl_CalculatorService_Proxy.cpp \
CalculatorImpl_CalculatorService_Wrapper.cpp \
CalculatorImpl_CalculatorDivideService_Proxy.cpp \
DivideServiceImpl_DivideService_Proxy.cpp \
DivideServiceImpl_DivideService_Wrapper.cpp

noinst_HEADERS = *.h

${BUILT_SOURCES}: sca.composite
	java -jar $(TUSCANY_SCACPP)/bin/scagen.jar -dir . -output .

composite_LTLIBRARIES = libCalculatorComposite.la
composite_DATA = sca.composite.lin Tuscany-model.config *.componentType *.wsdl services.xml
EXTRA_DIST = sca.composite.lin Tuscany-model.config *.componentType *.wsdl services.xml

libCalculatorComposite_la_SOURCES = \
CalculatorImpl.cpp \
CalculatorImpl_CalculatorService_Proxy.cpp \
CalculatorImpl_CalculatorService_Wrapper.cpp \
CalculatorImpl_CalculatorDivideService_Proxy.cpp \
DivideServiceImpl.cpp \
DivideServiceImpl_DivideService_Proxy.cpp \
DivideServiceImpl_DivideService_Wrapper.cpp

libCalculatorComposite_la_LIBADD = -L${TUSCANY_SCACPP}/lib -ltuscany_sca

INCLUDES = \
           -I$(TUSCANY_SCACPP)/include \
           -I${TUSCANY_SDOCPP}/include

install-data-hook:
	mv $(compositedir)/sca.composite.lin $(compositedir)/sca.composite