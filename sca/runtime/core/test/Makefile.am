SUBDIRS = CustomerInfo MyValue src

BUILT_SOURCES = scagen

scagen:
	cp ./MyValue/MyValue.h ./testSCASystem/modules/MyValueServiceModule
	cp ./MyValue/MyValueImpl.h ./testSCASystem/modules/MyValueServiceModule
	cp ./MyValue/StockQuoteService.h ./testSCASystem/modules/MyValueServiceModule
	cp ./CustomerInfo/CustomerInfo.h ./testSCASystem/modules/MyValueServiceModule
	cp ./CustomerInfo/CustomerInfoImpl.h ./testSCASystem/modules/MyValueServiceModule
	cd $(top_builddir)/bin && \
	./scagen.sh -dir ../runtime/core/test/testSCASystem/modules/MyValueServiceModule -output ../runtime/core/test/tmp
	cp tmp/CustomerInfoImpl*.* CustomerInfo
	cp tmp/MyValueImpl*.* MyValue
	
                   
install-exec-local:
	cp CustomerInfo/.libs/libCustomerInfo.so testSCASystem/modules/MyValueServiceModule
	cp MyValue/.libs/libMyValue.so testSCASystem/modules/MyValueServiceModule
	