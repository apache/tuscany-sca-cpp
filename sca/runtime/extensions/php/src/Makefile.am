libdir=$(prefix)/extensions/php/lib
lib_LTLIBRARIES = libtuscany_sca_php.la
includedir=$(prefix)/extensions/php/include
nobase_include_HEADERS = \
tuscany/sca/php/*.h \
tuscany/sca/php/model/*.h

libtuscany_sca_php_la_SOURCES = \
tuscany/sca/php/PHPExtension.cpp \
tuscany/sca/php/PHPImplementationExtension.cpp \
tuscany/sca/php/PHPServiceProxy.cpp \
tuscany/sca/php/PHPServiceWrapper.cpp \
tuscany/sca/php/model/PHPImplementation.cpp \
tuscany/sca/php/model/PHPReferenceBinding.cpp \
tuscany/sca/php/model/PHPServiceBinding.cpp

libtuscany_sca_php_la_LIBADD = -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo \
	-L$(top_builddir)/runtime/core/src -ltuscany_sca \
	-L${PHP_LIB} -lphp

INCLUDES = -Imodel -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I${PHP_INCLUDE}
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
                   