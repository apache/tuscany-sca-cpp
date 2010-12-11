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

eval_test_SOURCES = eval-test.cpp

eval_shell_SOURCES = eval-shell.cpp

xml_value_SOURCES = xml-value.cpp
xml_value_LDFLAGS =  -lxml2

value_xml_SOURCES = value-xml.cpp
value_xml_LDFLAGS =  -lxml2

json_value_SOURCES = json-value.cpp
json_value_LDFLAGS = -lmozjs 

value_json_SOURCES = value-json.cpp
value_json_LDFLAGS = -lmozjs 

noinst_PROGRAMS = eval-test eval-shell xml-value value-xml json-value value-json
TESTS = eval-test
