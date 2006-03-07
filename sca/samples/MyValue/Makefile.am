SUBDIRS = MyValueModule Client

deploy_dir = $(top_builddir)/samples/runtime
deploy_module = $(deploy_dir)/modules/MyValueModule
                   
install-exec-local:
	cp MyValueSubsystem/sca.subsystem $(deploy_dir)/subsystems/MyValueSubsystem
	cp MyValueModule/sca.module $(deploy_module)
	cp MyValueModule/CustomerInfo.fragment $(deploy_module)
	cp MyValueModule/Tuscany-model.config $(deploy_module)
	cp MyValueModule/CustomerInfo/CustomerInfoImpl.componentType $(deploy_module)/CustomerInfo
	cp MyValueModule/CustomerInfo/.libs/libCustomerInfo.so $(deploy_module)/CustomerInfo
	cp MyValueModule/CustomerInfo/CustomerInfo.h $(deploy_module)/CustomerInfo
	cp MyValueModule/CustomerInfo/CustomerInfoImpl.h $(deploy_module)/CustomerInfo
	cp MyValueModule/MyValue/MyValueImpl.componentType $(deploy_module)/MyValue
	cp MyValueModule/MyValue/MyValue.h $(deploy_module)/MyValue
	cp MyValueModule/MyValue/MyValueImpl.h $(deploy_module)/MyValue
	cp MyValueModule/MyValue/MyValueService.wsdl $(deploy_module)/MyValue
	cp MyValueModule/MyValue/.libs/libMyValue.so $(deploy_module)/MyValue
	cp MyValueModule/StockQuoteService/StockQuoteService.h $(deploy_module)/StockQuoteService
	cp MyValueModule/StockQuoteService/StockQuoteService.wsdl $(deploy_module)/StockQuoteService
	

