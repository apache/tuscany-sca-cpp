deploydir=$(prefix)/samples/BigBank/deploy
compositedir=$(deploydir)/composites/Accounts

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

${BUILT_SOURCES}: sca.composite
	java -jar $(TUSCANY_SCACPP)/bin/scagen.jar -dir . -output .

composite_LTLIBRARIES = libAccounts.la
composite_DATA = sca.composite.lin Tuscany-model.config *.componentType *.wsdl *.xsd services.xml
EXTRA_DIST = sca.composite.lin Tuscany-model.config *.componentType *.wsdl *.xsd services.xml

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
	mv $(compositedir)/sca.composite.lin $(compositedir)/sca.composite
