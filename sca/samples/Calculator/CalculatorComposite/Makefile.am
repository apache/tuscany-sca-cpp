deploydir=$(prefix)/samples/Calculator/deploy
compositedir=$(deploydir)/packages/CalculatorComposite

BUILT_SOURCES = CalculatorImpl_CalculatorService_Proxy.cpp \
CalculatorImpl_CalculatorService_Wrapper.cpp \
CalculatorImpl_divideService_Proxy.cpp \
DivideServiceImpl_DivideService_Proxy.cpp \
DivideServiceImpl_DivideService_Wrapper.cpp

noinst_HEADERS = *.h

${BUILT_SOURCES}: CalculatorComposite.composite
	java -jar $(TUSCANY_SCACPP)/bin/scagen.jar -dir . -output .

composite_LTLIBRARIES = libCalculatorComposite.la
composite_DATA = *.composite *.componentType *.wsdl services.xml
EXTRA_DIST = *.composite *.componentType *.wsdl services.xml

libCalculatorComposite_la_SOURCES = \
CalculatorImpl.cpp \
CalculatorImpl_CalculatorService_Proxy.cpp \
CalculatorImpl_CalculatorService_Wrapper.cpp \
CalculatorImpl_divideService_Proxy.cpp \
DivideServiceImpl.cpp \
DivideServiceImpl_DivideService_Proxy.cpp \
DivideServiceImpl_DivideService_Wrapper.cpp

libCalculatorComposite_la_LIBADD = -L${TUSCANY_SCACPP}/lib \
			-ltuscany_sca \
			-L${TUSCANY_SCACPP}/extensions/cpp/lib \
			-ltuscany_sca_cpp

INCLUDES = \
           -I$(TUSCANY_SCACPP)/extensions/cpp/include \
           -I$(TUSCANY_SCACPP)/include \
           -I${TUSCANY_SDOCPP}/include
