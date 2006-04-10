lib_LTLIBRARIES = libMyValue.la

libMyValue_la_SOURCES = \
MyValueImpl.cpp \
MyValueImpl_customerInfo_Proxy.cpp \
MyValueImpl_MyValueService_Proxy.cpp \
MyValueImpl_MyValueService_Wrapper.cpp \
MyValueImpl_stockQuote_Proxy.cpp

libMyValue_la_LIBADD = -L$(top_builddir)/runtime/core/src -ltuscany_sca \
	-L${TUSCANY_SDOCPP}/lib -ltuscany_sdo

INCLUDES = -I.. \
           -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include
                   
install: