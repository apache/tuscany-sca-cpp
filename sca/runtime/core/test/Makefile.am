SUBDIRS = CustomerInfo MyValue src

EXTRA_DIST = testSCASystem/subsystems \
testSCASystem/modules/MyValueServiceModule/sca.module \
testSCASystem/modules/MyValueServiceModule/*.fragment \
testSCASystem/modules/MyValueServiceModule/*.componentType \
testSCASystem/modules/MyValueServiceModule/*.wsdl \
testSCASystem/modules/MyValueServiceModule/*.xsd \
testSCASystem/modules/MyValueServiceModule/*.config


BUILT_SOURCES = scagen

scagen:
	cp ./MyValue/MyValue.h ./testSCASystem/modules/MyValueServiceModule
	cp ./MyValue/MyValueImpl.h ./testSCASystem/modules/MyValueServiceModule
	cp ./MyValue/StockQuoteService.h ./testSCASystem/modules/MyValueServiceModule
	cp ./CustomerInfo/CustomerInfo.h ./testSCASystem/modules/MyValueServiceModule
	cp ./CustomerInfo/CustomerInfoImpl.h ./testSCASystem/modules/MyValueServiceModule
	java -jar $(top_builddir)/tools/scagen/bld/scagen.jar -dir testSCASystem/modules/MyValueServiceModule -output tmp
	cp tmp/CustomerInfoImpl*.* CustomerInfo
	cp tmp/MyValueImpl*.* MyValue
	
                   
install-exec-local:
	cp CustomerInfo/.libs/libCustomerInfo.so testSCASystem/modules/MyValueServiceModule
	cp MyValue/.libs/libMyValue.so testSCASystem/modules/MyValueServiceModule
	