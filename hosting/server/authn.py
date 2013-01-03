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

# User authenticator implementation
from util import *

# Convert a particular user id to an authentication id
def authnid(id):
    return append(append(('authn',), id), ('user.authn',))

# Get a user's authentication
def get(id, cache):
    authn = cache.get(authnid(id))
    if isNil(authn):
        return None
    return authn

# Update a user's authentication
def put(id, authn, cache):
    return cache.put(authnid(id), authn)

# Delete a user's authentication
def delete(id, cache):
    return cache.delete(authnid(id))

