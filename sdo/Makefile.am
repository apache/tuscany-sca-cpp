SUBDIRS = runtime
datadir=$(prefix)

data_DATA = INSTALL README LICENSE COPYING NOTICE readme.txt
EXTRA_DIST = LICENSE COPYING INSTALL README NOTICE sdotest.sh readme.txt
dist-hook:
	rm -rf `find $(distdir)/ -type d -name .svn`
	rm -rf `find $(distdir)/ -type d -name .deps`
	
bindist: dist
	rm -rf ${PACKAGE}-${PACKAGE_VERSION}
	rm -rf ${PACKAGE}-${PACKAGE_VERSION}-src
	rm -rf ${PACKAGE}-${PACKAGE_VERSION}-bin
	gunzip -f ${PACKAGE}-${PACKAGE_VERSION}.tar.gz
	tar -xf ${PACKAGE}-${PACKAGE_VERSION}.tar
	mv ${PACKAGE}-${PACKAGE_VERSION} ${PACKAGE}-${PACKAGE_VERSION}-src
	tar -cf - ${PACKAGE}-${PACKAGE_VERSION}-src | gzip -c > ${PACKAGE}-${PACKAGE_VERSION}-src.tar.gz
	cd ${PACKAGE}-${PACKAGE_VERSION}-src \
	&&	sh ../makebindist.sh
	mkdir ${PACKAGE}-${PACKAGE_VERSION}-bin
	cp -r ${PACKAGE}-${PACKAGE_VERSION}-src/deploy/* ${PACKAGE}-${PACKAGE_VERSION}-bin
	tar -cf - ${PACKAGE}-${PACKAGE_VERSION}-bin | gzip -c > ${PACKAGE}-${PACKAGE_VERSION}-bin.tar.gz
