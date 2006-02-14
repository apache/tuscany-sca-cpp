lib_LTLIBRARIES = libtuscany_sca_axis_wrapper.la

libtuscany_sca_axis_wrapper_la_SOURCES = SCAWSWrapper.cpp

libtuscany_sca_axis_wrapper_la_LIBADD = -lstdc++ -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo

INCLUDES = -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I${AXISCPP_DEPLOY}/include
                   
