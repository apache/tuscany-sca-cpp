SUBDIRS = CustomerInfo MyValue src

EXTRA_DIST = testSCASystem/configuration \
testSCASystem/packages/MyValueServiceComposite/*.composite \
testSCASystem/packages/MyValueServiceComposite/*.componentType \
testSCASystem/packages/MyValueServiceComposite/*.wsdl \
testSCASystem/packages/MyValueServiceComposite/*.xsd \
testSCASystem/packages/MyValueServiceComposite/*.config

BUILT_SOURCES = CustomerInfo/CustomerInfoImpl_CustomerInfoService_Proxy.cpp \
CustomerInfo/CustomerInfoImpl_CustomerInfoService_Proxy.h \
CustomerInfo/CustomerInfoImpl_CustomerInfoService_Wrapper.cpp \
CustomerInfo/CustomerInfoImpl_CustomerInfoService_Wrapper.h \
MyValue/MyValueImpl_customerInfo_Proxy.cpp \
MyValue/MyValueImpl_customerInfo_Proxy.h \
MyValue/MyValueImpl_MyValueService_Proxy.cpp \
MyValue/MyValueImpl_MyValueService_Proxy.h \
MyValue/MyValueImpl_MyValueService_Wrapper.cpp \
MyValue/MyValueImpl_MyValueService_Wrapper.h \
MyValue/MyValueImpl_stockQuote_Proxy.cpp \
MyValue/MyValueImpl_stockQuote_Proxy.h


${BUILT_SOURCES}: ./testSCASystem/packages/MyValueServiceComposite/sca.composite
	cp ./MyValue/MyValue.h ./testSCASystem/packages/MyValueServiceComposite
	cp ./MyValue/MyValueImpl.h ./testSCASystem/packages/MyValueServiceComposite
	cp ./MyValue/StockQuoteService.h ./testSCASystem/packages/MyValueServiceComposite
	cp ./CustomerInfo/CustomerInfo.h ./testSCASystem/packages/MyValueServiceComposite
	cp ./CustomerInfo/CustomerInfoImpl.h ./testSCASystem/packages/MyValueServiceComposite
	java -jar $(top_builddir)/tools/scagen/bld/scagen.jar -dir testSCASystem/packages/MyValueServiceComposite -output tmp
	cp tmp/CustomerInfoImpl*.* CustomerInfo
	cp tmp/MyValueImpl*.* MyValue
	
                   
install-exec-local:
	cp CustomerInfo/.libs/libCustomerInfo$(libsuffix) testSCASystem/packages/MyValueServiceComposite
	cp MyValue/.libs/libMyValue$(libsuffix) testSCASystem/packages/MyValueServiceComposite
	
