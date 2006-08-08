SUBDIRS = CustomerInfo MyValue src

EXTRA_DIST = testSCASystem/subsystems \
testSCASystem/composites/MyValueServiceComposite/*.composite \
testSCASystem/composites/MyValueServiceComposite/*.componentType \
testSCASystem/composites/MyValueServiceComposite/*.wsdl \
testSCASystem/composites/MyValueServiceComposite/*.xsd \
testSCASystem/composites/MyValueServiceComposite/*.config

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


${BUILT_SOURCES}: ./testSCASystem/composites/MyValueServiceComposite/sca.composite
	cp ./MyValue/MyValue.h ./testSCASystem/composites/MyValueServiceComposite
	cp ./MyValue/MyValueImpl.h ./testSCASystem/composites/MyValueServiceComposite
	cp ./MyValue/StockQuoteService.h ./testSCASystem/composites/MyValueServiceComposite
	cp ./CustomerInfo/CustomerInfo.h ./testSCASystem/composites/MyValueServiceComposite
	cp ./CustomerInfo/CustomerInfoImpl.h ./testSCASystem/composites/MyValueServiceComposite
	java -jar $(top_builddir)/tools/scagen/bld/scagen.jar -dir testSCASystem/composites/MyValueServiceComposite -output tmp
	cp tmp/CustomerInfoImpl*.* CustomerInfo
	cp tmp/MyValueImpl*.* MyValue
	
                   
install-exec-local:
	cp CustomerInfo/.libs/libCustomerInfo.so testSCASystem/composites/MyValueServiceComposite
	cp MyValue/.libs/libMyValue.so testSCASystem/composites/MyValueServiceComposite
	
