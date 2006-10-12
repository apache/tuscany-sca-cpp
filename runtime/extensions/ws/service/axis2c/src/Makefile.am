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
  -L$(top_builddir)/runtime/core/src -ltuscany_sca \
  -L$(AXIS2C_HOME)/lib \
    -laxis2_util \
    -laxis2_axiom \
    -laxis2_wsdl \
    -laxis2_engine \
    -laxis2_parser \
    -laxis2_minizip \
    -lpthread \
    -lwoden \
    -laxis2_http_sender \
    -laxis2_http_receiver

rootdir=$(prefix)/extensions/ws/service
root_DATA = axis2.xml
root_SCRIPTS = deploy.sh

libtuscany_sca_ws_dispatcher_la_SOURCES = \
tuscany/sca/ws/Axis2Dispatcher.cpp \
tuscany/sca/ws/Axis2DispatcherModule.cpp

libtuscany_sca_ws_dispatcher_la_LIBADD = \
  -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo -ltuscany_sdo_axiom \
  -L$(top_builddir)/runtime/core/src -ltuscany_sca \
  -L$(AXIS2C_HOME)/lib \
    -laxis2_util \
    -laxis2_axiom \
    -laxis2_wsdl \
    -laxis2_engine \
    -laxis2_parser \
    -laxis2_minizip \
    -lpthread \
    -lwoden \
    -laxis2_http_sender \
    -laxis2_http_receiver

servicedir=$(rootdir)/services/tuscany
service_DATA = services.xml

moduledir=$(rootdir)/modules/tuscany
module_DATA = module.xml

EXTRA_DIST = axis2.xml services.xml module.xml deploy.sh

install-data-hook:
	cd $(servicedir); ln -s -f ../../lib/libtuscany_sca_ws_service.so libtuscany_sca_ws_service.so
	cd $(moduledir); ln -s -f ../../lib/libtuscany_sca_ws_dispatcher.so libtuscany_sca_ws_dispatcher.so

INCLUDES = -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I${AXIS2C_HOME}/include
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
