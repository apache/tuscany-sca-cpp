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

minified = htdocs/public/iframe-min.html htdocs/create/index-min.html htdocs/page/index-min.html htdocs/login/index-min.html htdocs/notfound/index-min.html htdocs/oops/index-min.html htdocs/graph/index-min.html htdocs/notauth/index-min.html htdocs/account/index-min.html htdocs/home/index-min.html htdocs/index-min.html htdocs/notyet/index-min.html htdocs/clone/index-min.html htdocs/stats/index-min.html htdocs/app/index-min.html htdocs/logout/index-min.html htdocs/store/index-min.html htdocs/headconfig-min.js htdocs/footconfig-min.js

resources = edit.composite *.py htdocs/*.html htdocs/*.js htdocs/*.cmf htdocs/*.ico htdocs/home/*.png htdocs/home/*.b64 htdocs/*.txt htdocs/account/*.html htdocs/create/*.html htdocs/clone/*.html htdocs/app/*.html htdocs/store/*.html htdocs/stats/*.html htdocs/graph/*.html htdocs/home/*.html htdocs/page/*.html htdocs/login/*.html htdocs/logout/*.html htdocs/notauth/*.html htdocs/notfound/*.html htdocs/oops/*.html htdocs/public/*.html htdocs/public/*.png htdocs/public/*.b64 palettes/*/palette.composite apps/*/app.composite apps/*/app.stats apps/*/htdocs/app.html dashboards/*/user.apps store/*/store.apps ${minified}

nobase_dist_mod_DATA = ${resources}
EXTRA_DIST = ${resources}

SUFFIXES = -min.html -min.js
.html-min.html:
	../http/minify-html $< $@

.js-min.js:
	../http/minify-js $< $@

CLEANFILES = ${minified}

endif
