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

# Stores collection implementation
from util import *
from sys import debug

# Convert a particular store tag to a store id
def storeid(tag):
    return ("store", tag, "store.apps")

# Get a store from the cache
def getstore(id, cache):
    debug('store.py::getstore::id', id)
    val = cache.get(id)
    if isNil(val) or val is None:
        return ()
    store = cdddr(car(val))
    if not isNil(store) and isList(car(cadr(car(store)))):
        # Expand list of entries
        estore = tuple(map(lambda e: cons("'entry", e), cadr(car(store))))
        debug('store.py::getstore::estore', estore)
        return estore

    debug('store.py::getstore::store', store)
    return store

# Put a store into the cache
def putstore(id, store, cache):
    debug('store.py::putstore::id', id)
    debug('store.py::putstore::store', store)
    val = ((("'feed", ("'title", "App Store"), ("'id", cadr(id))) + store),)
    return cache.put(id, val)

# Put an app into a store
def put(id, app, user, cache, apps):
    debug('store.py::put::id', id)
    debug('store.py::put::app', app)
    tag = car(id)
    appid = cdr(id)

    def putapp(appid, app, store):
        if isNil(store):
            return app
        if car(appid) == cadr(assoc("'id", car(store))):
            return cons(car(app), cdr(store))
        return cons(car(store), putapp(appid, app, cdr(store)))

    appentry = (("'entry", assoc("'title", car(app)), ("'id", car(appid)), ("'author", user.get(())), assoc("'updated", car(app)), assoc("'content", car(app))),)
    debug('store.py::put::appentry', appentry)

    store = putapp(appid, appentry, getstore(storeid(tag), cache))
    return putstore(storeid(tag), store, cache)

# Get apps from a store
def get(id, user, cache, apps):
    debug('store.py::get::id', id)
    tag = car(id)
    appid = cdr(id)

    def findapp(appid, store):
        if isNil(store):
            return None
        if car(appid) == cadr(assoc("'id", car(store))):
            return (car(store),)
        return findapp(appid, cdr(store))

    if isNil(appid):
        store = ((("'feed", ("'title", "App Store"), ("'id", tag)) + getstore(storeid(tag), cache)),)
        debug('store.py::get::store', store)
        return store

    app = findapp(appid, getstore(storeid(tag), cache))
    debug('store.py::get::app', app)
    return app

# Delete apps from a store
def delete(id, user, cache, apps):
    debug('store.py::delete::id', id)
    tag = car(id)
    appid = cdr(id)

    if isNil(appid):
        return cache.delete(storeid(tag))

    def deleteapp(appid, store):
        if isNil(store):
            return ()
        if car(appid) == cadr(assoc("'id", car(store))):
            return cdr(store)
        return cons(car(store), deleteapp(appid, cdr(store)))

    store = getstore(storeid(tag), cache)
    deleted = deleteapp(appid, store)
    if deleted == store:
        return False
    return putstore(storeid(tag), deleted, cache)

