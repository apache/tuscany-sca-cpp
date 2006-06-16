lib_LTLIBRARIES = libCustomerInfo.la

libCustomerInfo_la_SOURCES = \
CustomerInfoImpl.cpp \
CustomerInfoImpl_CustomerInfoService_Proxy.cpp \
CustomerInfoImpl_CustomerInfoService_Wrapper.cpp

noinst_HEADERS = *.h

libCustomerInfo_la_LIBADD =

INCLUDES = -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include
                      
install: