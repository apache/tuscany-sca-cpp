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
incldir = $(prefix)/include/modules/scheme

moddir = $(prefix)/modules/scheme

scheme_test_SOURCES = scheme-test.cpp

scheme_shell_SOURCES = scheme-shell.cpp

value_element_SOURCES = value-element.cpp
value_element_LDFLAGS = 

element_value_SOURCES = element-value.cpp
element_value_LDFLAGS = 

xml_element_SOURCES = xml-element.cpp
xml_element_LDFLAGS = -lxml2

element_xml_SOURCES = element-xml.cpp
element_xml_LDFLAGS = -lxml2

json_value_SOURCES = json-value.cpp
json_value_LDFLAGS = -ljansson 

value_json_SOURCES = value-json.cpp
value_json_LDFLAGS = -ljansson 

noinst_PROGRAMS = scheme-test
mod_PROGRAMS = scheme-shell element-value value-element xml-element element-xml json-value value-json
TESTS = scheme-test
