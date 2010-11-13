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

if WANT_PYTHON
if WANT_GAE

dist_sample_SCRIPTS = start stop
sampledir = $(prefix)/samples/store-gae

BUILT_SOURCES = target.stamp 
target.stamp: app.yaml *.py *.composite $(top_builddir)/modules/wsgi/*.py htdocs/* $(top_builddir)/modules/server/htdocs/wiring/*
	mkdir -p target
	cp app.yaml *.py *.composite `ls $(top_builddir)/modules/wsgi/*.py | grep -v "\-test"` target
	mkdir -p target/htdocs
	cp -R htdocs/* target/htdocs
	mkdir -p target/htdocs/wiring
	cp -R $(top_builddir)/modules/server/htdocs/wiring/* target/htdocs/wiring
	touch target.stamp

clean-local:
	rm -rf target.stamp target

nobase_sample_DATA = target/app.yaml target/*.py target/*.composite target/htdocs/*.html target/htdocs/wiring/*.js

EXTRA_DIST = app.yaml *.composite *.py htdocs/*.html

dist_noinst_SCRIPTS = server-test
TESTS = server-test

endif
endif
