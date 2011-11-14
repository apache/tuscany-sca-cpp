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

moddir = $(prefix)/modules/edit
dist_mod_SCRIPTS = start stop ssl-start mkapplinks

BUILT_SOURCES = htdocs/config.js htdocs/public/config.js
htdocs/config.js:
	touch htdocs/config.js

htdocs/public/config.js:
	touch htdocs/public/config.js

not_minified = htdocs/public/iframe.html htdocs/create/index.html htdocs/page/index.html htdocs/login/index.html htdocs/public/notfound/index.html htdocs/public/oops/index.html htdocs/graph/index.html htdocs/public/notauth/index.html htdocs/account/index.html htdocs/home/index.html htdocs/index.html htdocs/public/notyet/index.html htdocs/clone/index.html htdocs/stats/index.html htdocs/app/index.html htdocs/logout/index.html htdocs/store/index.html htdocs/config.js htdocs/public/config.js

minified = htdocs/public/iframe-min.html htdocs/create/index-min.html htdocs/page/index-min.html htdocs/login/index-min.html htdocs/public/notfound/index-min.html htdocs/public/oops/index-min.html htdocs/graph/index-min.html htdocs/public/notauth/index-min.html htdocs/account/index-min.html htdocs/home/index-min.html htdocs/index-min.html htdocs/public/notyet/index-min.html htdocs/clone/index-min.html htdocs/stats/index-min.html htdocs/app/index-min.html htdocs/logout/index-min.html htdocs/store/index-min.html htdocs/config-min.js htdocs/public/config-min.js

resources = edit.composite *.py htdocs/*.cmf htdocs/*.ico htdocs/home/*.png htdocs/app/*.cmf htdocs/home/*.b64 htdocs/*.txt htdocs/public/*.png htdocs/public/*.b64 palettes/*/palette.composite accounts/*/*.account apps/*/app.composite apps/*/app.stats apps/*/htdocs/app.html dashboards/*/user.apps store/*/store.apps ${not_minified} ${minified}

nobase_dist_mod_DATA = ${resources}
EXTRA_DIST = ${resources}

SUFFIXES = -min.html -min.js
.html-min.html:
	../http/minify-html $< $@

.js-min.js:
	../http/minify-js $< $@

CLEANFILES = ${minified}

endif
