lib_LTLIBRARIES = libtuscany_sca_ws_reference.la

libtuscany_sca_ws_reference_la_SOURCES = WSServiceWrapper.cpp Axis2Client.cpp

AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG

libtuscany_sca_ws_reference_la_LIBADD = \
	-L${TUSCANY_SDOCPP}/lib -ltuscany_sdo -ltuscany_sdo_axiom -lxml2 -lstdc++ \
	                    -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_om \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -laxis2_unix \
                    -lpthread \
                    -laxis2_soap \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
                    -laxis2_attachments
	
	
INCLUDES = -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I${AXIS2C_HOME}/include
                   
