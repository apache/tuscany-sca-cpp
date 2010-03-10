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

# Memcached based cache implementation
import uuid
from google.appengine.api import memcache

# Post a new item to the cache
def post(collection, item):
    id = collection + (str(uuid.uuid1()),)
    r = memcache.add(repr(id), item, 600)
    if r == False:
        return None
    return id

# Get items from the cache
def get(id):
    item = memcache.get(repr(id))
    return item

# Update an item in the cache
def put(id, item):
    return memcache.set(repr(id), item, 600)

# Delete items from the cache
def delete(id):
    r = memcache.delete(repr(id))
    if r != 2:
        return False
    return True

