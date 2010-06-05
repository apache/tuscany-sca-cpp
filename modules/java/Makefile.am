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

JAVAROOT = $(top_builddir)/modules/java

if WANT_JAVA

INCLUDES = -I${JAVA_INCLUDE}

incl_HEADERS = *.hpp
incldir = $(prefix)/include/modules/java

dist_mod_SCRIPTS = java-conf
moddir = $(prefix)/modules/java

prefix_DATA = java.prefix
prefixdir = $(prefix)/modules/java
java.prefix: $(top_builddir)/config.status
	echo ${JAVA_PREFIX} >java.prefix

EXTRA_DIST = domain-test.composite

mod_LTLIBRARIES = libmod_tuscany_java.la
libmod_tuscany_java_la_SOURCES = mod-java.cpp
libmod_tuscany_java_la_LDFLAGS = -lxml2 -lcurl -lmozjs ${JAVA_LDFLAGS}
noinst_DATA = libmod_tuscany_java.so
libmod_tuscany_java.so:
	ln -s .libs/libmod_tuscany_java.so

jni_test_SOURCES = jni-test.cpp
jni_test_LDFLAGS = ${JAVA_LDFLAGS}

java_test_SOURCES = java-test.cpp
java_test_LDFLAGS = ${JAVA_LDFLAGS}

java_shell_SOURCES = java-shell.cpp
java_shell_LDFLAGS = ${JAVA_LDFLAGS}

dist_mod_JAVA = org/apache/tuscany/*.java test/*.java
jardir = ${prefix}/modules/java
jarfile = libmod-tuscany-java-${PACKAGE_VERSION}.jar
jar_DATA = ${jarfile}
${jarfile}: ${dist_mod_JAVA}
	${JAR} cf $@ org/apache/tuscany/*.class

CLEANFILES = *.stamp ${jarfile} org/apache/tuscany/*.class	test/*.class

client_test_SOURCES = client-test.cpp
client_test_LDFLAGS = -lxml2 -lcurl -lmozjs

dist_noinst_SCRIPTS = server-test wiring-test
noinst_PROGRAMS = jni-test java-test java-shell client-test
TESTS = jni-test java-test server-test

endif
