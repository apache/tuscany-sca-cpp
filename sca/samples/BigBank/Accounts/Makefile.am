deploydir=$(prefix)/samples/BigBank/deploy
compositedir=$(deploydir)/packages/Accounts

BUILT_SOURCES = AccountDataServiceImpl_AccountDataService_Proxy.cpp \
AccountDataServiceImpl_AccountDataService_Wrapper.cpp \
AccountServiceImpl_accountData_Proxy.cpp \
AccountServiceImpl_AccountService_Proxy.cpp \
AccountServiceImpl_AccountService_Wrapper.cpp \
AccountServiceImpl_stockQuote_Proxy.cpp \
StockQuoteServiceImpl_webService_Proxy.cpp \
StockQuoteServiceImpl_StockQuoteService_Proxy.cpp \
StockQuoteServiceImpl_StockQuoteService_Wrapper.cpp

noinst_HEADERS = *.h

${BUILT_SOURCES}: Accounts.composite
	java -jar $(TUSCANY_SCACPP)/bin/scagen.jar -dir . -output .

composite_LTLIBRARIES = libAccounts.la
composite_DATA = *.composite Tuscany-model.config *.componentType *.wsdl *.xsd services.xml
EXTRA_DIST = *.composite Tuscany-model.config *.componentType *.wsdl *.xsd services.xml

libAccounts_la_SOURCES = \
AccountDataServiceImpl.cpp \
AccountServiceImpl.cpp \
StockQuoteServiceImpl.cpp \
AccountDataServiceImpl_AccountDataService_Proxy.cpp \
AccountDataServiceImpl_AccountDataService_Wrapper.cpp \
AccountServiceImpl_accountData_Proxy.cpp \
AccountServiceImpl_AccountService_Proxy.cpp \
AccountServiceImpl_AccountService_Wrapper.cpp \
AccountServiceImpl_stockQuote_Proxy.cpp \
StockQuoteServiceImpl_webService_Proxy.cpp \
StockQuoteServiceImpl_StockQuoteService_Proxy.cpp \
StockQuoteServiceImpl_StockQuoteService_Wrapper.cpp

libAccounts_la_LIBADD = -L${TUSCANY_SCACPP}/lib -ltuscany_sca \
		-L${TUSCANY_SCACPP}/extensions/cpp -ltuscany_sca_cpp

INCLUDES = \
           -I$(TUSCANY_SCACPP)/include \
           -I${TUSCANY_SDOCPP}/include
