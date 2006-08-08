SUBDIRS = MyValueComposite Client

deploy_dir = $(top_builddir)/samples/runtime
deploy_composite = $(deploy_dir)/composites/MyValueComposite
                   
install-exec-local:
	cp MyValueSubsystem/sca.subsystem $(deploy_dir)/subsystems/MyValueSubsystem
	cp MyValueComposite/sca.composite $(deploy_composite)
	cp MyValueComposite/CustomerInfo.fragment $(deploy_composite)
	cp MyValueComposite/Tuscany-model.config $(deploy_composite)
	cp MyValueComposite/CustomerInfo/CustomerInfoImpl.componentType $(deploy_composite)/CustomerInfo
	cp MyValueComposite/CustomerInfo/.libs/libCustomerInfo.so $(deploy_composite)/CustomerInfo
	cp MyValueComposite/CustomerInfo/CustomerInfo.h $(deploy_composite)/CustomerInfo
	cp MyValueComposite/CustomerInfo/CustomerInfoImpl.h $(deploy_composite)/CustomerInfo
	cp MyValueComposite/MyValue/MyValueImpl.componentType $(deploy_composite)/MyValue
	cp MyValueComposite/MyValue/MyValue.h $(deploy_composite)/MyValue
	cp MyValueComposite/MyValue/MyValueImpl.h $(deploy_composite)/MyValue
	cp MyValueComposite/MyValue/MyValueService.wsdl $(deploy_composite)/MyValue
	cp MyValueComposite/MyValue/.libs/libMyValue.so $(deploy_composite)/MyValue
	cp MyValueComposite/StockQuoteService/StockQuoteService.h $(deploy_composite)/StockQuoteService
	cp MyValueComposite/StockQuoteService/StockQuoteService.wsdl $(deploy_composite)/StockQuoteService
	

