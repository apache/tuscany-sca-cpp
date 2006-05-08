SUBDIRS =

nobase_include_HEADERS = sdo_axiom.h \
sdo_axiom_export.h

lib_LTLIBRARIES = libtuscany_sdo_axiom.la
AM_CPPFLAGS = $(CPPFLAGS)

libtuscany_sdo_axiom_la_SOURCES = \
	sdo_axiom.cpp
	
libtuscany_sdo_axiom_la_LIBADD = \
	                -L$(top_builddir)/runtime/core/src/commonj/sdo -ltuscany_sdo \
                    -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_om \
                    -laxis2_parser

INCLUDES = -I$(top_builddir)/runtime/core/src \
           -I$(top_builddir)/runtime/core/sdo_axiom  \
           -I${AXIS2C_HOME}/include
     