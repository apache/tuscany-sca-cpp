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

if WANT_OAUTH

INCLUDES = -I${HTTPD_INCLUDE} -I${LIBOAUTH_INCLUDE}

dist_mod_SCRIPTS = oauth-conf oauth-memcached-conf oauth1-appkey-conf oauth2-appkey-conf
moddir=$(prefix)/modules/oauth

mod_LTLIBRARIES = libmod_tuscany_oauth1.la libmod_tuscany_oauth2.la
noinst_DATA = libmod_tuscany_oauth1${libsuffix} libmod_tuscany_oauth2${libsuffix}

libmod_tuscany_oauth1_la_SOURCES = mod-oauth1.cpp
libmod_tuscany_oauth1_la_LDFLAGS = -L${LIBOAUTH_LIB} -R${LIBOAUTH_LIB} -loauth -lxml2 -lcurl -lmozjs
libmod_tuscany_oauth1${libsuffix}:
	ln -s .libs/libmod_tuscany_oauth1${libsuffix}

libmod_tuscany_oauth2_la_SOURCES = mod-oauth2.cpp
libmod_tuscany_oauth2_la_LDFLAGS = -lxml2 -lcurl -lmozjs
libmod_tuscany_oauth2${libsuffix}:
	ln -s .libs/libmod_tuscany_oauth2${libsuffix}

EXTRA_DIST = oauth.composite user-info.scm htdocs/index.html htdocs/login/index.html htdocs/logout/index.html htdocs/public/index.html

dist_noinst_SCRIPTS = start-test stop-test

endif
