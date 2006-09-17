libdir=$(prefix)/extensions/ws/service/lib
lib_LTLIBRARIES = \
libtuscany_sca_ws_service.la \
libtuscany_sca_ws_dispatcher.la

noinst_HEADERS = \
tuscany/sca/ws/*.h \
tuscany/sca/ws/model/*.h

libtuscany_sca_ws_service_la_SOURCES = \
tuscany/sca/ws/WSReferenceBindingExtension.cpp \
tuscany/sca/ws/model/WSReferenceBinding.cpp \
tuscany/sca/ws/WSServiceProxy.cpp \
tuscany/sca/ws/Axis2Service.cpp \
tuscany/sca/ws/Axis2Utils.cpp

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

libtuscany_sca_ws_dispatcher_la_SOURCES = \
tuscany/sca/ws/Axis2Dispatcher.cpp

libtuscany_sca_ws_dispatcher_la_LIBADD = \
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

INCLUDES = -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I${AXIS2C_HOME}/include
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
                   