SUBDIRS = runtime samples
datadir=$(prefix)

data_DATA = INSTALL README LICENSE COPYING
nobase_data_DATA = xsd/*.*
EXTRA_DIST = LICENSE COPYING INSTALL README scatest.sh xsd
dist-hook:
	rm -rf `find $(distdir)/ -type d -name .svn`
	rm -rf `find $(distdir)/ -type d -name .deps`
