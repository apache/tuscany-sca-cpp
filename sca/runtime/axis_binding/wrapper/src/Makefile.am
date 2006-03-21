lib_LTLIBRARIES = libtuscany_sca_axis_wrapper.la

libtuscany_sca_axis_wrapper_la_SOURCES = SCAWSWrapper.cpp AxisServiceException.cpp

libtuscany_sca_axis_wrapper_la_LIBADD = -L$(top_builddir)/runtime/core/src -ltuscany_sca \
	-L${TUSCANY_SDOCPP}/lib -ltuscany_sdo -lxml2 -lstdc++ \
	-L${AXISCPP_DEPLOY}/lib -laxis_client
	
INCLUDES = -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I${AXISCPP_DEPLOY}/include
                   
