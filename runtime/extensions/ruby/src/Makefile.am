libdir=$(prefix)/extensions/ruby/lib
lib_LTLIBRARIES = libtuscany_sca_ruby.la
includedir=$(prefix)/extensions/ruby/include
nobase_include_HEADERS = \
tuscany/sca/ruby/*.h \
tuscany/sca/ruby/model/*.h

libtuscany_sca_ruby_la_SOURCES = \
tuscany/sca/ruby/RubyExtension.cpp \
tuscany/sca/ruby/RubyImplementationExtension.cpp \
tuscany/sca/ruby/RubyServiceProxy.cpp \
tuscany/sca/ruby/RubyServiceWrapper.cpp \
tuscany/sca/ruby/model/RubyImplementation.cpp \
tuscany/sca/ruby/model/RubyReferenceBinding.cpp \
tuscany/sca/ruby/model/RubyServiceBinding.cpp \
tuscany/sca/ruby/RubyCompositeContext.cpp

libtuscany_sca_ruby_la_LIBADD = -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo \
	-L$(top_builddir)/runtime/core/src -ltuscany_sca \
	-L${RUBY_LIB} -lruby

INCLUDES = -Imodel -I$(top_builddir)/runtime/core/src \
           -I${TUSCANY_SDOCPP}/include \
           -I${RUBY_INCLUDE}
                   
AM_CPPFLAGS = $(CPPFLAGS) -D_DEBUG
                   