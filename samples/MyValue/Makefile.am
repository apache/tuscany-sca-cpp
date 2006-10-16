#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#    
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.

SUBDIRS = MyValueComposite Client

deploy_dir = $(top_builddir)/samples/runtime
deploy_composite = $(deploy_dir)/packages/MyValueComposite
                   
install-exec-local:
	cp MyValueSubsystem/sca.subsystem $(deploy_dir)/configuration/MyValueSubsystem
	cp MyValueComposite/sca.composite $(deploy_composite)
	cp MyValueComposite/CustomerInfo.fragment $(deploy_composite)
	cp MyValueComposite/CustomerInfo/CustomerInfoImpl.componentType $(deploy_composite)/CustomerInfo
	cp MyValueComposite/CustomerInfo/.libs/libCustomerInfo$libsuffix) $(deploy_composite)/CustomerInfo
	cp MyValueComposite/CustomerInfo/CustomerInfo.h $(deploy_composite)/CustomerInfo
	cp MyValueComposite/CustomerInfo/CustomerInfoImpl.h $(deploy_composite)/CustomerInfo
	cp MyValueComposite/MyValue/MyValueImpl.componentType $(deploy_composite)/MyValue
	cp MyValueComposite/MyValue/MyValue.h $(deploy_composite)/MyValue
	cp MyValueComposite/MyValue/MyValueImpl.h $(deploy_composite)/MyValue
	cp MyValueComposite/MyValue/MyValueService.wsdl $(deploy_composite)/MyValue
	cp MyValueComposite/MyValue/.libs/libMyValue$libsuffix) $(deploy_composite)/MyValue
	cp MyValueComposite/StockQuoteService/StockQuoteService.h $(deploy_composite)/StockQuoteService
	cp MyValueComposite/StockQuoteService/StockQuoteService.wsdl $(deploy_composite)/StockQuoteService
	

