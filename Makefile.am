#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#    
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.

SUBDIRS = tools runtime doc
DIST_SUBDIRS = tools runtime doc samples
datadir=$(prefix)

data_DATA = INSTALL README LICENSE COPYING NOTICE GettingStarted.html
nobase_data_DATA = xsd/*.*
EXTRA_DIST = LICENSE COPYING INSTALL README NOTICE GettingStarted.html xsd build_scanative.sh
dist-hook:
	rm -rf `find $(distdir)/ -type d -name .svn`
	rm -rf `find $(distdir)/ -type d -name .deps`

install-data-hook:
	rm -rf $(prefix)/doc
	cp -r doc $(prefix)
	rm -f $(prefix)/doc/Makefile* $(prefix)/doc/Doxyfile*
	rm -rf `find $(prefix)/doc -type d -name .svn`
	rm -rf `find $(prefix)/doc -type d -name .deps`

	rm -rf $(prefix)/samples
	cp -r samples $(prefix)
	rm -rf `find $(prefix)/samples -type d -name .svn`
	rm -rf `find $(prefix)/samples -type d -name .deps`
	
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
	
