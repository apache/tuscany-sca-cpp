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

if WANT_ALL
  CPP_EXTENSION = cpp
  PYTHON_EXTENSION = python
  RUBY_EXTENSION = ruby
  WSBINDING_EXTENSION = ws
  SCABINDING_EXTENSION = sca
  RESTBINDING_EXTENSION = rest
endif
if WANT_CPP
  CPP_EXTENSION = cpp
endif
if WANT_PYTHON
  PYTHON_EXTENSION = python
endif
if WANT_RUBY
  RUBY_EXTENSION = ruby
endif

if WANT_WSBINDING
  WSBINDING_EXTENSION = ws
endif
if WANT_SCABINDING
  SCABINDING_EXTENSION = sca
endif
if WANT_RESTBINDING
  RESTBINDING_EXTENSION = rest
endif
SUBDIRS = ${CPP_EXTENSION} ${WSBINDING_EXTENSION} ${SCABINDING_EXTENSION} ${RESTBINDING_EXTENSION} ${PYTHON_EXTENSION} ${RUBY_EXTENSION}
