SUBDIRS = runtime samples
datadir=$(prefix)

data_DATA = INSTALL README LICENSE COPYING
EXTRA_DIST = LICENSE COPYING INSTALL README scatest.sh
dist-hook:
	rm -rf `find $(distdir)/ -type d -name .svn`
	rm -rf `find $(distdir)/ -type d -name .deps`
