deploydir=$(prefix)/samples/BigBank/deploy
moduledir=$(deploydir)/modules/Accounts

BUILT_SOURCES = AccountDataServiceImpl_AccountDataService_Proxy.cpp \
AccountDataServiceImpl_AccountDataService_Wrapper.cpp \
AccountServiceImpl_AccountDataService_Proxy.cpp \
AccountServiceImpl_AccountService_Proxy.cpp \
AccountServiceImpl_AccountService_Wrapper.cpp \
AccountServiceImpl_stockQuote_Proxy.cpp \
StockQuoteServiceImpl_StockQuoteExternal_Proxy.cpp \
StockQuoteServiceImpl_StockQuoteService_Proxy.cpp \
StockQuoteServiceImpl_StockQuoteService_Wrapper.cpp

noinst_HEADERS = *.h

AccountDataServiceImpl_AccountDataService_Proxy.cpp:
	java -jar $(TUSCANY_SCACPP)/bin/scagen.jar -dir . -output .

module_LTLIBRARIES = libAccounts.la
module_DATA = sca.module.lin Tuscany-model.config *.componentType *.wsdl services.xml
EXTRA_DIST = sca.module.lin Tuscany-model.config *.componentType *.wsdl services.xml

libAccounts_la_SOURCES = \
AccountDataServiceImpl.cpp \
AccountServiceImpl.cpp \
StockQuoteServiceImpl.cpp \
AccountDataServiceImpl_AccountDataService_Proxy.cpp \
AccountDataServiceImpl_AccountDataService_Wrapper.cpp \
AccountServiceImpl_AccountDataService_Proxy.cpp \
AccountServiceImpl_AccountService_Proxy.cpp \
AccountServiceImpl_AccountService_Wrapper.cpp \
AccountServiceImpl_stockQuote_Proxy.cpp \
StockQuoteServiceImpl_StockQuoteExternal_Proxy.cpp \
StockQuoteServiceImpl_StockQuoteService_Proxy.cpp \
StockQuoteServiceImpl_StockQuoteService_Wrapper.cpp

libAccounts_la_LIBADD = -L${TUSCANY_SCACPP}/lib -ltuscany_sca

INCLUDES = \
           -I$(TUSCANY_SCACPP)/include \
           -I${TUSCANY_SDOCPP}/include

install-data-hook:
	mv $(moduledir)/sca.module.lin $(moduledir)/sca.module
