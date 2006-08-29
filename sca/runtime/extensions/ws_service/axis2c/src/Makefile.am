libdir=$(prefix)/extensions/ws_service
lib_LTLIBRARIES = libtuscany_sca_ws_service.la

noinst_HEADERS = *.h model/*.h

libtuscany_sca_ws_service_la_SOURCES = \
WSReferenceBindingExtension.cpp \
model/WSReferenceBinding.cpp \
WSServiceProxy.cpp \
Axis2Service_skeleton.cpp \
Axis2Utils.cpp

libtuscany_sca_ws_service_la_LIBADD = \
	-L${TUSCANY_SDOCPP}/lib -ltuscany_sdo -ltuscany_sdo_axiom \
					-L$(top_builddir)/runtime/core/src -ltuscany_sca
	                -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_axiom \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -laxis2_minizip \
                    -lpthread \
                    -laxis2_http_sender \
                    -laxis2_http_receiver

INCLUDES = -Imodel -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I${AXIS2C_HOME}/include
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
                   