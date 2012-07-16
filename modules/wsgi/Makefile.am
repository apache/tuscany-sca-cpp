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

INCLUDES = -I${PYTHON_INCLUDE}

dist_mod_SCRIPTS = composite.py wsgi-start wsgi-stop gae-start gae-stop
moddir = $(prefix)/modules/wsgi

dist_mod_DATA = app.yaml scdl.py util.py elemutil.py xmlutil.py atomutil.py jsonutil.py httputil.py rssutil.py

noinst_DATA = target.stamp

target.stamp: app.yaml *.py *.composite htdocs/*
	mkdir -p target
	cp app.yaml *.py *.composite target
	cp -R htdocs target/htdocs
	touch target.stamp

clean-local:
	rm -rf target.stamp target

prefix_DATA = gae.prefix
prefixdir=$(prefix)/modules/wsgi
gae.prefix: $(top_builddir)/config.status
	echo ${GAE_PREFIX} >gae.prefix

EXTRA_DIST = domain-test.composite *.py htdocs/test/*.xml htdocs/test/*.txt htdocs/*.html

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs

noinst_PROGRAMS = client-test

dist_noinst_SCRIPTS = util-test wsgi-test wiring-test http-test server-test
TESTS = util-test

if WANT_GAE
dist_noinst_SCRIPTS += gae-test
TESTS += wsgi-test wiring-test http-test server-test gae-test
endif

endif
