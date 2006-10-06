deploydir=$(prefix)/samples/BigBank/deploy
compositedir=$(deploydir)/bigbank.account
prgbindir=$(deploydir)/bin

BUILT_SOURCES = AccountDataServiceImpl_AccountDataService_Proxy.cpp \
AccountDataServiceImpl_AccountDataService_Wrapper.cpp \
AccountServiceImpl_accountDataService_Proxy.cpp \
AccountServiceImpl_AccountService_Proxy.cpp \
AccountServiceImpl_AccountService_Wrapper.cpp \
AccountServiceImpl_stockQuoteService_Proxy.cpp \
StockQuoteServiceImpl_StockQuoteService_Proxy.cpp \
StockQuoteServiceImpl_StockQuoteService_Wrapper.cpp \
StockQuoteServiceImpl_webService_Proxy.cpp

noinst_HEADERS = *.h

prgbin_SCRIPTS = runwsserver.sh

${BUILT_SOURCES}: bigbank.account.composite
	java -jar $(TUSCANY_SCACPP)/bin/scagen.jar -dir . -output .

composite_LTLIBRARIES = libAccount.la
composite_DATA = *.composite *.componentType *.wsdl *.xsd
EXTRA_DIST = *.composite *.componentType *.wsdl *.xsd runwsserver.sh

libAccount_la_SOURCES = \
AccountDataServiceImpl_AccountDataService_Proxy.cpp \
AccountDataServiceImpl_AccountDataService_Wrapper.cpp \
AccountDataServiceImpl.cpp \
AccountServiceImpl_accountDataService_Proxy.cpp \
AccountServiceImpl_AccountService_Proxy.cpp \
AccountServiceImpl_AccountService_Wrapper.cpp \
AccountServiceImpl.cpp \
AccountServiceImpl_stockQuoteService_Proxy.cpp \
StockQuoteServiceImpl.cpp \
StockQuoteServiceImpl_StockQuoteService_Proxy.cpp \
StockQuoteServiceImpl_StockQuoteService_Wrapper.cpp \
StockQuoteServiceImpl_webService_Proxy.cpp

libAccount_la_LIBADD = \
-L${TUSCANY_SCACPP}/lib \
  -ltuscany_sca \
-L${TUSCANY_SCACPP}/extensions/cpp/lib \
  -ltuscany_sca_cpp

INCLUDES = \
           -I$(TUSCANY_SCACPP)/extensions/cpp/include \
           -I$(TUSCANY_SCACPP)/include \
           -I${TUSCANY_SDOCPP}/include
