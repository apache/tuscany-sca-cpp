deploydir=$(prefix)/samples/misc/deploy/
prgbindir=$(deploydir)/bin

prgbin_PROGRAMS = sdo_misc
prgbin_SCRIPTS = 
EXTRA_DIST = *.xsd *.xml
deploy_DATA = *.xsd *.xml

AM_CPPFLAGS = $(CPPFLAGS)
sdo_misc_SOURCES = samples.cpp \
ChangeSummarySave.cpp \
ObjectCreation.cpp \
Query.cpp \
Substitutes.cpp \
XSDLoading.cpp 


noinst_HEADERS = *.h

sdo_misc_LDADD   = -L${TUSCANY_SDOCPP}/lib -ltuscany_sdo -lxml2
	

INCLUDES =  -I$(top_builddir)/misc \
            -I${TUSCANY_SDOCPP}/include
