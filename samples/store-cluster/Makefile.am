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
if WANT_SQLDB
if WANT_OPENID
if WANT_LOG
if WANT_QUEUE

dist_sample_SCRIPTS = start stop ssl-start ssl-stop proxy-conf proxy-ssl-conf server-conf server-ssl-conf tunnel-ssl-conf sqldb-master-conf sqldb-standby-conf
sampledir = $(prefix)/samples/store-cluster

nobase_dist_sample_DATA = htdocs/*.html htdocs/domains/*/*.html domains/*/*.py domains/*/*.composite shared/*.composite

dist_noinst_SCRIPTS = server-test
#TESTS = server-test

endif
endif
endif
endif
endif
