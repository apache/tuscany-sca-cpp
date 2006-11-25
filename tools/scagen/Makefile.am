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

SUBDIRS = 

BUILT_SOURCES = scagen_build
EXTRA_DIST = build.xml scagen.sh src META-INF
bin_SCRIPTS = scagen.sh

scagen_build:
	ant 
	touch scagen_build
	
clean:
	rm -f scagen_build
	                    
install-exec-hook:
	cp bld/scagen.jar $(bindir)
	
	 
    

