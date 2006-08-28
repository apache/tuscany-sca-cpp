lib_LTLIBRARIES = libMyValue.la

noinst_HEADERS = *.h

libMyValue_la_SOURCES = \
MyValueImpl.cpp \
MyValueImpl_customerInfo_Proxy.cpp \
MyValueImpl_MyValueService_Proxy.cpp \
MyValueImpl_MyValueService_Wrapper.cpp \
MyValueImpl_stockQuote_Proxy.cpp

libMyValue_la_LIBADD =

INCLUDES = -I$(top_builddir)/runtime/core/src \
		   -I$(top_builddir)/runtime/extensions \
           -I${TUSCANY_SDOCPP}/include \
           -I../CustomerInfo
                   
install: