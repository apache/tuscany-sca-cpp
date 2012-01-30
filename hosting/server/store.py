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

# stores collection implementation
from util import *

# Convert a particular store tag to a store id
def storeid(tag):
    return ("'store", "'" + tag, "'store.apps")

# Get a store from the cache
def getstore(id, cache):
    store = cache.get(id)
    if isNil(store) or store is None:
        return ()
    return store

# Put an app into a store
def put(key, app, cache):
    def putapp(app, store):
        if isNil(store):
            return app
        if cadr(caddr(car(app))) == cadr(caddr(car(store))):
            return cons(car(app), cdr(store))
        return cons(car(store), putapp(app, cdr(store)))

    tag = car(key)
    store = putapp(app, getstore(storeid(tag), cache))
    cache.put(storeid(tag), store)
    return True

# Get apps from a store
def get(key, cache):
    tag = car(key)
    id = cdr(key)

    def findapp(id, store):
        if isNil(store):
            return None
        if car(id) == cadr(caddr(car(store))):
            return (car(store),)
        return findapp(id, cdr(store))

    if isNil(id):
        return ((("'feed", ("'title", "App Store"), ("'id", tag)) + getstore(storeid(tag), cache)),)
    return findapp(id, getstore(storeid(tag), cache))

# Delete apps from a store
def delete(key, cache):
    tag = car(key)
    id = cdr(key)

    if isNil(id):
        return cache.delete(storeid(tag))

    def deleteapp(id, store):
        if isNil(store):
            return ()
        if car(id) == cadr(caddr(car(store))):
            return cdr(store)
        return cons(car(store), deleteapp(id, cdr(store)))

    store = deleteapp(id, getstore(storeid(tag), cache))
    cache.put(storeid(tag), store)
    return True

