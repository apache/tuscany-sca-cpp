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

moddir = $(prefix)/hosting/server
dist_mod_SCRIPTS = start stop ssl-start mkapplinks config-backup data-backup put-auth get-auth delete-auth create-tables drop-tables load-tables clean-tables load-authn pgsql imapd-start imapd-stop

not_minified = htdocs/create/index.html htdocs/page/index.html htdocs/login/index.html htdocs/public/cache/index.html htdocs/public/notfound/index.html htdocs/public/oops/index.html htdocs/proxy/public/oops/index.html htdocs/proxy/public/cache/index.html htdocs/graph/index.html htdocs/public/notauth/index.html htdocs/account/index.html htdocs/home/index.html htdocs/index.html htdocs/cache/index.html htdocs/public/notyet/index.html htdocs/clone/index.html htdocs/delete/index.html htdocs/rate/index.html htdocs/search/index.html htdocs/info/index.html htdocs/app/index.html htdocs/store/index.html htdocs/config.js htdocs/public/config.js htdocs/cache/cache-template.cmf htdocs/app/cache/cache-template.cmf htdocs/public/cache/cache-template.cmf htdocs/proxy/public/cache/cache-template.cmf

minified = htdocs/create/index-min.html htdocs/page/index-min.html htdocs/login/index-min.html htdocs/public/cache/index-min.html htdocs/public/notfound/index-min.html htdocs/public/oops/index-min.html htdocs/proxy/public/oops/index-min.html htdocs/proxy/public/cache/index-min.html htdocs/graph/index-min.html htdocs/public/notauth/index-min.html htdocs/account/index-min.html htdocs/home/index-min.html htdocs/index-min.html htdocs/cache/index-min.html htdocs/public/notyet/index-min.html htdocs/clone/index-min.html htdocs/delete/index-min.html htdocs/rate/index-min.html htdocs/search/index-min.html htdocs/info/index-min.html htdocs/app/index-min.html htdocs/store/index-min.html htdocs/config-min.js htdocs/public/config-min.js

js_minified = ../../modules/js/htdocs/all-min.js ../../modules/js/htdocs/ui-min.css

b64images = htdocs/public/app.b64 htdocs/public/img.b64 htdocs/public/rate.b64 htdocs/public/ratings.b64 htdocs/public/search.b64 htdocs/public/user.b64

resources = server.composite *.py data/palettes/*/palette.composite data/accounts/*/*.account data/apps/*/app.composite data/apps/*/app.info data/apps/*/htdocs/app.html data/dashboards/*/user.apps data/store/*/store.apps htdocs/cache/cache-manifest.cmf htdocs/app/cache/cache-manifest.cmf htdocs/public/cache/cache-manifest.cmf htdocs/proxy/public/cache/cache-manifest.cmf htdocs/*.ico htdocs/*.txt htdocs/public/*.png ${b64images} ${minified}

nobase_dist_mod_DATA = ${resources}
EXTRA_DIST = ${resources} ${not_minified}

SUFFIXES = -min.html -min.js b64
.html-min.html:
	../../modules/http/minify-html $< $@

.js-min.js:
	../../modules/http/minify-js $< $@

.png.b64:
	../../modules/http/base64-encode $< $@

htdocs/cache/cache-manifest.cmf: htdocs/cache/cache-template.cmf ${minified} ${js_minified} ${b64images}
	../../modules/http/cache-manifest htdocs/cache $^

htdocs/app/cache/cache-manifest.cmf: htdocs/app/cache/cache-template.cmf ${minified} ${js_minified} ${b64images}
	../../modules/http/cache-manifest htdocs/app/cache $^

htdocs/public/cache/cache-manifest.cmf: htdocs/public/cache/cache-template.cmf ${minified} ${js_minified} ${b64images}
	../../modules/http/cache-manifest htdocs/public/cache $^

htdocs/proxy/public/cache/cache-manifest.cmf: htdocs/proxy/public/cache/cache-template.cmf ${minified} ${js_minified} ${b64images}
	../../modules/http/cache-manifest htdocs/proxy/public/cache $^

nuvem:
	ln -s "../../../nuvem/nuvem-parallel/nuvem" "nuvem"

lib:
	ln -s "../../components" "lib"

install-data-hook:
	cd $(moddir); rm -f nuvem; ln -s "../../../nuvem/nuvem-parallel/nuvem" "nuvem"
	cd $(moddir); rm -f lib; ln -s "../../components" "lib"

CLEANFILES = ${minified} nuvem lib htdocs/cache/cache-manifest.cmf htdocs/app/cache/cache-manifest.cmf htdocs/public/cache/cache-manifest.cmf htdocs/proxy/public/cache/cache-manifest.cmf

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -ljansson

patch_test_SOURCES = patch-test.cpp
patch_test_LDFLAGS = -lxml2 -lcurl -ljansson

dist_noinst_SCRIPTS = logic-test server-test test.py
noinst_PROGRAMS = client-test patch-test
TESTS = logic-test patch-test

endif
