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

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = kernel modules components test doc samples

datadir=$(prefix)
data_DATA = INSTALL README LICENSE COPYING NOTICE
nobase_data_DATA = xsd/*.xsd xsd/external/*.xsd xsd/external/*.dtd

install-data-hook:
	rm -rf $(prefix)/doc
	cp -r doc $(prefix)
	rm -f $(prefix)/doc/Makefile* $(prefix)/doc/Doxyfile*
	rm -rf `find $(prefix)/doc -type d -name .svn`
	rm -rf `find $(prefix)/doc -type d -name .deps`
	rm -rf $(prefix)/test

dist-hook:
	rm -rf `find $(distdir)/ -type d -name .svn`
	rm -rf `find $(distdir)/ -type d -name .deps`

bindist: install
	rm -rf ${PACKAGE}-${PACKAGE_VERSION}-bin
	mkdir ${PACKAGE}-${PACKAGE_VERSION}-bin
	cp -r $(prefix)/* ${PACKAGE}-${PACKAGE_VERSION}-bin
	rm -rf ${PACKAGE}-${PACKAGE_VERSION}-bin/test
	tar -cf - ${PACKAGE}-${PACKAGE_VERSION}-bin | gzip -c > ${PACKAGE}-${PACKAGE_VERSION}-bin.tar.gz
	rm -rf ${PACKAGE}-${PACKAGE_VERSION}-bin
	
