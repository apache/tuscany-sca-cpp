lib_LTLIBRARIES = libCustomerInfo.la

libCustomerInfo_la_SOURCES = \
CustomerInfoImpl.cpp \
CustomerInfoImpl_CustomerInfoService_Proxy.cpp \
CustomerInfoImpl_CustomerInfoService_Wrapper.cpp

libCustomerInfo_la_LIBADD = -L$(top_builddir)/runtime/core/src -ltuscany_sca \
	-L${TUSCANY_SDOCPP}/lib -ltuscany_sdo

INCLUDES = -I.. \
           -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include
                      
install: