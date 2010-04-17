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



includedir = $(prefix)/include/kernel
include_HEADERS = *.hpp

string_test_SOURCES = string-test.cpp

test_LTLIBRARIES = libdynlib-test.la
testdir = $(prefix)/test
libdynlib_test_la_SOURCES = dynlib-test.cpp

kernel_test_SOURCES = kernel-test.cpp

lambda_test_SOURCES = lambda-test.cpp

mem_test_SOURCES = mem-test.cpp

parallel_test_SOURCES = parallel-test.cpp

xml_test_SOURCES = xml-test.cpp
xml_test_LDFLAGS = -lxml2

xsd_test_SOURCES = xsd-test.cpp
xsd_test_LDFLAGS = -lxml2

hash_test_SOURCES = hash-test.cpp

noinst_PROGRAMS = string-test kernel-test lambda-test hash-test mem-test parallel-test xml-test xsd-test
TESTS = string-test kernel-test lambda-test hash-test mem-test parallel-test xml-test

