libdir=$(prefix)/extensions/python/lib
lib_LTLIBRARIES = libtuscany_sca_python.la
includedir=$(prefix)/extensions/python/include
nobase_include_HEADERS = \
tuscany/sca/python/*.h \
tuscany/sca/python/model/*.h

libtuscany_sca_python_la_SOURCES = \
tuscany/sca/python/PythonExtension.cpp \
tuscany/sca/python/PythonImplementationExtension.cpp \
tuscany/sca/python/PythonInterfaceExtension.cpp \
tuscany/sca/python/PythonServiceWrapper.cpp \
tuscany/sca/python/PythonServiceProxy.cpp \
tuscany/sca/python/model/PythonImplementation.cpp \
tuscany/sca/python/model/PythonInterface.cpp \
tuscany/sca/python/model/PythonReferenceBinding.cpp \
tuscany/sca/python/model/PythonServiceBinding.cpp


libtuscany_sca_python_la_LIBADD = -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo \
	-L$(top_builddir)/runtime/core/src -ltuscany_sca \
	-L${PYTHON_LIB} -lpython2.3

INCLUDES = -Imodel -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I${PYTHON_INCLUDE}
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
                   
