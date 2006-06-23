SUBDIRS = tools runtime
DIST_SUBDIRS = tools runtime samples
datadir=$(prefix)

data_DATA = INSTALL README LICENSE COPYING 
nobase_data_DATA = xsd/*.*
EXTRA_DIST = LICENSE COPYING INSTALL README scatest.sh xsd
dist-hook:
	rm -rf `find $(distdir)/ -type d -name .svn`
	rm -rf `find $(distdir)/ -type d -name .deps`

install-data-hook:
	rm -rf $(prefix)/samples
	cp -r samples $(prefix)
	rm -rf `find $(prefix)/samples -type d -name .svn`
	rm -rf `find $(prefix)/samples -type d -name .deps`
	
	
bindist: dist
	rm -rf ${PACKAGE}-${PACKAGE_VERSION}
	gunzip -f ${PACKAGE}-${PACKAGE_VERSION}.tar.gz
	tar -xf ${PACKAGE}-${PACKAGE_VERSION}.tar
	gzip ${PACKAGE}-${PACKAGE_VERSION}.tar
	cd ${PACKAGE}-${PACKAGE_VERSION} \
	&&	sh ../makebindist.sh
	mkdir ${PACKAGE}-bin-${PACKAGE_VERSION}
	cp -r ${PACKAGE}-${PACKAGE_VERSION}/deploy/* ${PACKAGE}-bin-${PACKAGE_VERSION}
	tar -cf - ${PACKAGE}-bin-${PACKAGE_VERSION} | gzip -c > ${PACKAGE}-bin-${PACKAGE_VERSION}.tar.gz
	rm -rf ${PACKAGE}-${PACKAGE_VERSION}
	rm -rf ${PACKAGE}-bin-${PACKAGE_VERSION}
	