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

INCLUDES = -I${HTTPD_INCLUDE}

incl_HEADERS = *.hpp
incldir = $(prefix)/include/modules/http

dist_mod_SCRIPTS = httpd-conf httpd-addr httpd-start httpd-stop httpd-restart ssl-ca-conf ssl-cert-conf ssl-cert-find httpd-ssl-conf base64-encode basic-auth-conf cert-auth-conf form-auth-conf open-auth-conf passwd-auth-conf group-auth-conf cache-conf cache-ssl-conf cache-manifest proxy-conf proxy-base-conf proxy-ssl-conf proxy-balancer-conf proxy-member-conf proxy-ssl-member-conf proxy-ssl-nossl-member-conf alt-host-conf mass-host-conf mass-host-ssl-conf httpd-tunnel-ssl-conf tunnel-ssl-conf httpd-worker-conf httpd-event-conf httpd-loglevel-conf minify-html minify-js minify-css ssinclude-html
moddir = $(prefix)/modules/http

curl_test_SOURCES = curl-test.cpp
curl_test_LDFLAGS = -lxml2 -lcurl -ljansson

curl_get_SOURCES = curl-get.cpp
curl_get_LDFLAGS = -lxml2 -lcurl -ljansson

curl_connect_SOURCES = curl-connect.cpp
curl_connect_LDFLAGS = -lxml2 -lcurl -ljansson

mod_LTLIBRARIES = libmod_tuscany_ssltunnel.la libmod_tuscany_openauth.la
noinst_DATA = libmod_tuscany_ssltunnel${libsuffix} libmod_tuscany_openauth${libsuffix}

libmod_tuscany_ssltunnel_la_SOURCES = mod-ssltunnel.cpp
libmod_tuscany_ssltunnel_la_LDFLAGS = -lxml2 -lcurl -ljansson
libmod_tuscany_ssltunnel${libsuffix}:
	ln -s .libs/libmod_tuscany_ssltunnel${libsuffix}

libmod_tuscany_openauth_la_SOURCES = mod-openauth.cpp
libmod_tuscany_openauth_la_LDFLAGS = -lxml2 -lcurl -ljansson
libmod_tuscany_openauth${libsuffix}:
	ln -s .libs/libmod_tuscany_openauth${libsuffix}

mod_DATA = httpd.prefix httpd-apachectl.prefix httpd-modules.prefix curl.prefix pagespeed.prefix
nobase_dist_mod_DATA = conf/*

EXTRA_DIST = htdocs/index.html htdocs/login/index.html htdocs/logout/index.html

httpd.prefix: $(top_builddir)/config.status
	echo ${HTTPD_PREFIX} >httpd.prefix
httpd-apachectl.prefix: $(top_builddir)/config.status
	echo ${HTTPD_APACHECTL_PREFIX} >httpd-apachectl.prefix
httpd-modules.prefix: $(top_builddir)/config.status
	echo ${HTTPD_MODULES_PREFIX} >httpd-modules.prefix
curl.prefix: $(top_builddir)/config.status
	echo ${CURL_PREFIX} >curl.prefix

if WANT_PAGESPEED

pagespeed.prefix: $(top_builddir)/config.status
	echo ${PAGESPEED_PREFIX} >pagespeed.prefix

else

pagespeed.prefix: $(top_builddir)/config.status
	echo "" >pagespeed.prefix

endif

if WANT_MODSECURITY

modsecurity.prefix: $(top_builddir)/config.status
	echo ${MODSECURITY_PREFIX} >modsecurity.prefix

dist_modsecurity_SCRIPTS = mod-security-conf mod-security-audit-conf
modsecurity_DATA = modsecurity.prefix
modsecuritydir = $(prefix)/modules/http

endif

dist_noinst_SCRIPTS = httpd-test http-test proxy-test httpd-memgrind httpd-callgrind
noinst_PROGRAMS = curl-test curl-get curl-connect
TESTS = httpd-test http-test proxy-test

