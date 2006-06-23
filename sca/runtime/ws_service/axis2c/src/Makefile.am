lib_LTLIBRARIES = libtuscany_sca_ws_service.la

libtuscany_sca_ws_service_la_SOURCES = Axis2EntryPointService_skeleton.cpp Axis2Utils.cpp

noinst_HEADERS = *.h

AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG

libtuscany_sca_ws_service_la_LIBADD = \
	-L${TUSCANY_SDOCPP}/lib -ltuscany_sdo -ltuscany_sdo_axiom  \
	-L$(top_builddir)/runtime/core/src -ltuscany_sca  \
	-L$(AXIS2C_HOME)/lib \
		    -laxis2_util \
                    -laxis2_axiom \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -lpthread \
                    -laxis2_minizip \
                    -laxis2_http_sender \
                    -laxis2_http_receiver
	
	
INCLUDES = -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I${AXIS2C_HOME}/include
                   
