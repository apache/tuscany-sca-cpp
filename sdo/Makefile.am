SUBDIRS = runtime
datadir=$(prefix)

data_DATA = INSTALL README LICENSE COPYING
EXTRA_DIST = LICENSE COPYING INSTALL README sdotest.sh
dist-hook:
	rm -rf `find $(distdir)/ -type d -name .svn`
	rm -rf `find $(distdir)/ -type d -name .deps`
	
bindist: dist
	rm -rf ${PACKAGE}-${PACKAGE_VERSION}
	gunzip -f ${PACKAGE}-${PACKAGE_VERSION}.tar.gz
	tar -xf ${PACKAGE}-${PACKAGE_VERSION}.tar
	gzip ${PACKAGE}-${PACKAGE_VERSION}.tar
	cd ${PACKAGE}-${PACKAGE_VERSION} \
	&&	sh ../makebindist.sh
	rm -rf ${PACKAGE}-bin-${PACKAGE_VERSION}
	mkdir ${PACKAGE}-bin-${PACKAGE_VERSION}
	cp -r ${PACKAGE}-${PACKAGE_VERSION}/deploy/* ${PACKAGE}-bin-${PACKAGE_VERSION}
	tar -cf - ${PACKAGE}-bin-${PACKAGE_VERSION} | gzip -c > ${PACKAGE}-bin-${PACKAGE_VERSION}.tar.gz
