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

datadir=$(prefix)/modules/java
JAVAROOT = $(top_builddir)/modules/java

if WANT_JAVA

noinst_PROGRAMS = java-test java-shell

INCLUDES = -I${JAVA_INCLUDE}

java_test_SOURCES = java-test.cpp
java_test_LDADD =  -L${JAVA_LIB} -ljava
java_test_LDFLAGS = -rpath ${JAVA_LIB} -rpath ${JAVA_LIB}/server

java_shell_SOURCES = java-shell.cpp
java_shell_LDADD = -L${JAVA_LIB} -ljava
java_shell_LDFLAGS = -rpath ${JAVA_LIB} -rpath ${JAVA_LIB}/server

noinst_JAVA = org/apache/tuscany/*.java test/*.java

TESTS = java-test

endif