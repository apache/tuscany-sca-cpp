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

incl_HEADERS = *.hpp
incldir = $(prefix)/include/modules/js

BUILT_SOURCES = htdocs/all.js htdocs/all-min.js htdocs/ui-min.css
htdocs/all.js: htdocs/util.js htdocs/elemutil.js htdocs/xmlutil.js htdocs/atomutil.js htdocs/jsonutil.js htdocs/scdl.js htdocs/ui.js htdocs/component.js
	cat htdocs/util.js htdocs/elemutil.js htdocs/xmlutil.js htdocs/atomutil.js htdocs/jsonutil.js htdocs/scdl.js htdocs/ui.js htdocs/component.js >htdocs/all.js

htdocs/all-min.js: htdocs/all.js
	../http/minify-js htdocs/all.js htdocs/all-min.js

htdocs/ui-min.css: htdocs/ui.css
	../http/minify-css htdocs/ui.css htdocs/ui-min.css

CLEANFILES = htdocs/all.js htdocs/all-min.js htdocs/ui-min.css

dist_mod_SCRIPTS = js-conf
moddir = $(prefix)/modules/js
nobase_dist_mod_DATA = htdocs/*.js htdocs/*.css
EXTRA_DIST = htdocs/*.js htdocs/*.css htdocs/all.js

js_test_SOURCES = js-test.cpp
js_test_LDFLAGS = -lmozjs 

js_shell_SOURCES = js-shell.cpp
js_shell_LDFLAGS = -lmozjs 

noinst_PROGRAMS = js-test
mod_PROGRAMS = js-shell
dist_noinst_SCRIPTS = util-test
TESTS = js-test util-test
