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
from atomutil import *
from sys import debug

# Convert a particular store tag to a store id
def storeid(tag):
    return ("store", tag, "store.apps")

# Get a store from the cache
def getstore(id, cache):
    debug('store.py::getstore::id', id)

    # Lookup the requested store
    val = cache.get(id)
    if isNull(val):
        return ()
    store = cdddr(car(val))
    if not isNull(store) and isList(car(cadr(car(store)))):
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
def put(id, app, user, cache, apps, ratings):
    debug('store.py::put::id', id)
    debug('store.py::put::app', app)
    tag = car(id)
    appid = cdr(id)

    def putapp(appid, app, store):
        if isNull(store):
            return app
        if car(appid) == entryid(store):
            return cons(car(app), cdr(store))
        return cons(car(store), putapp(appid, app, cdr(store)))

    appentry = mkentry(title(app), car(appid), author(app), updated(app), content(app))
    debug('store.py::put::appentry', appentry)

    store = putapp(appid, appentry, getstore(storeid(tag), cache))
    return putstore(storeid(tag), store, cache)

# Merge app info and ratings into a list of apps
def mergeapps(entries, apps, ratings):
    debug('store.py::mergeapps::entries', entries)

    def mergeapp(entry):
        debug('store.py::mergeapp::entry', entry)
        id = (entryid(entry),)
        app = apps.get(id)
        if isNull(app):
            return ((),)
        info = content(app)
        rating = ratings.get(id)
        rates = content(rating)
        mergedentry = mkentry(title(app), car(id), author(app), updated(app), ("'info",) + (() if isNull(info) else cdr(info)) + (() if isNull(rates) else cdr(rates)))
        return mergedentry

    mergedentries = tuple(filter(lambda e: not isNull(e), map(lambda e: car(mergeapp((e,))), entries)))
    debug('store.py::mergeapps::mergedentries', mergedentries)
    return mergedentries

# Get apps from a store
def get(id, user, cache, apps, ratings):
    debug('store.py::get::id', id)
    tag = car(id)

    # Collect the top rated apps
    if tag == 'top':
        topratings = ratings.get(()) 
        topapps = mergeapps(cdddr(car(topratings)), apps, ratings)
        topstore = ((("'feed", ("'title", 'App Store'), ("'id", tag)) + topapps),)
        debug('store.py::get::store', topstore)
        return topstore

    # Collect the featured apps
    appid = cdr(id)
    def findapp(appid, store):
        if isNull(store):
            return None
        if car(appid) == entryid(store):
            return (car(store),)
        return findapp(appid, cdr(store))

    if isNull(appid):
        storeapps = mergeapps(getstore(storeid(tag), cache), apps, ratings)
        store = ((("'feed", ("'title", "App Store"), ("'id", tag)) + storeapps),)
        debug('store.py::get::store', store)
        return store

    app = findapp(appid, getstore(storeid(tag), cache))
    debug('store.py::get::app', app)
    return app

# Delete apps from a store
def delete(id, user, cache, apps, ratings):
    debug('store.py::delete::id', id)
    tag = car(id)
    appid = cdr(id)

    if isNull(appid):
        return cache.delete(storeid(tag))

    def deleteapp(appid, store):
        if isNull(store):
            return ()
        if car(appid) == entryid(store):
            return cdr(store)
        return cons(car(store), deleteapp(appid, cdr(store)))

    store = getstore(storeid(tag), cache)
    deleted = deleteapp(appid, store)
    if deleted == store:
        return False
    return putstore(storeid(tag), deleted, cache)

