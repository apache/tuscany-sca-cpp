#!/usr/bin/python
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

# Test the hosting server components

import sys
sys.debug = lambda *l: sys.stderr.write('python::debug ' + repr(l) + '\n')
import time
time.strftime = lambda f: 'Jan 01, 2012'

import unittest
from test.property import *
from test.reference import *
from test.cache import *

import user
import accounts
import pages
import composites
import apps
import store
import dashboards

def testUser():
    # Return current user
    assert user.get((), mkprop('user', lambda: 'johndoe'), mkprop('email', lambda: 'jdoe@example.com'), mkprop('nick', lambda: 'jdoe'), mkprop('full', lambda: 'john doe'), mkprop('first', lambda: 'john'), mkprop('last', lambda: 'doe'), mkprop('realm', lambda: 'example.com'), mkprop('host', lambda: 'localhost')) == 'jdoe@example.com'
    return True

def testAccounts():
    # Get default account
    defaccount = (("'entry", ("'title", 'jdoe@example.com'), ("'id", 'jdoe@example.com'), ("'updated", 'Jan 01, 2012')),)
    assert accounts.get((), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {})) == defaccount

    # Get user's account
    jdoe = (("'entry", ("'title", 'John Doe'), ("'id", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'key", 'value'))),)
    assert accounts.get((), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('accounts', 'jdoe@example.com', 'user.account') : jdoe})) == jdoe

    # Put and get account
    cache1 = mkcache('cache', {})
    assert accounts.put((), jdoe, mkref('user', lambda id: 'jdoe@example.com'), cache1) == True
    assert accounts.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1) == jdoe
    return True

def testPages():
    # Get default page
    defpage = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 01, 2012')),)
    app1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 03, 2012'), ("'content", ())),)
    assert pages.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', lambda id: None)) == defpage
    defpagefromapp = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 03, 2012')),)
    assert pages.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', lambda id: app1)) == defpagefromapp

    # Get a page
    page1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ())),)
    assert pages.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('apps', 'app1', 'htdocs', 'app.html') : page1}), mkref('apps', lambda id: app1)) == page1

    # Put and get a page
    cache1 = mkcache('cache', {})
    page1updated = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 01, 2012'), ("'content", ())),)
    assert pages.put(('app1',), page1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == True
    assert pages.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == page1updated
    
    # Reject put from user other than the author
    app1otherauthor = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jane@example.com'), ("'updated", 'Jan 03, 2012'), ("'content", ())),)
    page1otherauthor = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jane@example.com'), ("'updated", 'Jan 02, 2012')),)
    assert pages.put(('app1',), page1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1otherauthor)) == False
    assert pages.put(('app1',), page1otherauthor, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1otherauthor)) == False
    assert pages.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == page1updated

    # Reject delete from user other than the author
    assert pages.delete(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1otherauthor)) == False
    assert pages.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == page1updated

    # Delete a page
    assert pages.delete(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == True
    assert pages.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == defpagefromapp
    return True

def testComposites():
    # Get default composite
    defcomposite = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 01, 2012')),)
    app1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 03, 2012'), ("'content", ())),)
    assert composites.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', lambda id: None)) == defcomposite
    defcompositefromapp = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 03, 2012')),)
    assert composites.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', lambda id: app1)) == defcompositefromapp

    # Get a composite
    composite1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ())),)
    assert composites.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('apps', 'app1', 'app.composite') : composite1}), mkref('apps', lambda id: app1)) == composite1

    # Put and get a composite
    cache1 = mkcache('cache', {})
    composite1updated = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 01, 2012'), ("'content", ())),)
    assert composites.put(('app1',), composite1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == True
    assert composites.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == composite1updated
    
    # Reject put from user other than the author
    app1otherauthor = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jane@example.com'), ("'updated", 'Jan 03, 2012'), ("'content", ())),)
    composite1otherauthor = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jane@example.com'), ("'updated", 'Jan 02, 2012')),)
    assert composites.put(('app1',), composite1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1otherauthor)) == False
    assert composites.put(('app1',), composite1otherauthor, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1otherauthor)) == False
    assert composites.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == composite1updated

    # Reject delete from user other than the author
    assert composites.delete(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1otherauthor)) == False
    assert composites.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == composite1updated

    # Delete a composite
    assert composites.delete(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == True
    assert composites.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == defcompositefromapp
    return True

def testApps():
    # Get default app
    defapp = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 01, 2012'), ("'content", ("'stats", ("'description", '')))),)
    assert apps.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None)) == defapp

    # Get an app
    app1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 01, 2012'), ("'content", ("'stats", ("'description", '')))),)
    assert apps.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('apps', 'app1', 'app.stats') : app1}), mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None)) == app1

    # Put and get an app
    cache1 = mkcache('cache', {})
    assert apps.put(('app1',), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('dashboard', lambda id, app: True), mkref('store', lambda id, app: True), mkref('composites', lambda id, app: True), mkref('pages', lambda id, app: True)) == True
    assert apps.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None)) == app1
    return True
    
    # Reject put from user other than the author
    app1otherauthor = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jane@example.com'), ("'updated", 'Jan 03, 2012'), ("'content", ())),)
    assert apps.put(('app1',), app1, mkref('user', lambda id: 'jane@example.com'), cache1, mkref('dashboard', lambda id, app: True), mkref('store', lambda id, app: True), mkref('composites', lambda id, app: True), mkref('pages', lambda id, app: True)) == false
    assert apps.get(('app1',), mkref('user', lambda id: 'jane@example.com'), cache1, mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None)) == app1

    # Reject delete from user other than the author
    assert apps.delete(('app1',), mkref('user', lambda id: 'jane@example.com'), cache1, mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None)) == False
    assert apps.get(('app1',), mkref('user', lambda id: 'jane@example.com'), cache1, mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None)) == app1

    # Delete an app
    assert apps.delete(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None)) == True
    assert apps.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None)) == defapp
    return True

def testStore():
    # Get default store
    defstore = (("'feed", ("'title", 'App Store'), ("'id", 'top')),)
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', lambda id: None)) == defstore

    # Get a store
    store1= (("'feed", ("'title", 'App Store'), ("'id", 'top'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app1')))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app2'))))),)
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('store', 'top', 'store.apps') : store1}), mkref('apps', lambda id: None)) == store1
    
    store1compact = (("'feed", ("'title", 'App Store'), ("'id", 'top'), ("'entry", ((("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app1')))), (("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app2'))))))),)
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('store', 'top', 'store.apps') : store1compact}), mkref('apps', lambda id: None)) == store1

    # Put an app in an empty store
    cache1 = mkcache('cache', {})
    app1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app1')))),)
    store1withapp1 = (("'feed", ("'title", 'App Store'), ("'id", 'top'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app1'))))),)
    assert store.put(('top', 'app1'), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == store1withapp1
    assert store.put(('top', 'app1'), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == store1withapp1

    # Put a second app in the store
    app2 = (("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app2')))),)
    store1withapp2 = (("'feed", ("'title", 'App Store'), ("'id", 'top'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app1')))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app2'))))),)
    assert store.put(('top', 'app2'), app2, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == store1withapp2
    assert store.put(('top', 'app1'), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == store1withapp2
    assert store.put(('top', 'app2'), app2, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == store1withapp2

    # Put a third app in the store
    app3 = (("'entry", ("'title", 'app3'), ("'id", 'app3'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app3')))),)
    store1withapp3 = (("'feed", ("'title", 'App Store'), ("'id", 'top'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app1')))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app2')))), ("'entry", ("'title", 'app3'), ("'id", 'app3'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app3'))))),)
    assert store.put(('top', 'app3'), app3, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == store1withapp3
    assert store.put(('top', 'app1'), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == store1withapp3
    assert store.put(('top', 'app2'), app2, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == store1withapp3
    assert store.put(('top', 'app3'), app3, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == store1withapp3

    # Get an app from the store
    assert store.get(('top','app1'), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == app1
    assert store.get(('top','app2'), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == app2
    assert store.get(('top','app3'), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == app3

    # Put a third app in the store, starting from a compacted list
    cache2 = mkcache('cache', {('store', 'top', 'store.apps') : store1compact})
    assert store.put(('top', 'app3'), app3, mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', lambda id: None)) == True
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', lambda id: None)) == store1withapp3

    # Delete the apps
    assert store.delete(('top', 'app2'), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert store.delete(('top', 'app4'), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == False
    assert store.delete(('top', 'app1'), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert store.delete(('top', 'app3'), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == defstore

    # Delete a store
    assert store.delete(('top',), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', lambda id: None)) == True
    assert store.get(('top',), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', lambda id: None)) == defstore
    return True

def testDashboards():
    # Get default dashboard
    defdashboard = (("'feed", ("'title", 'Your Apps'), ("'id", 'jdoe@example.com')),)
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', lambda id: None)) == defdashboard

    # Get the user's dashboard
    dash1= (("'feed", ("'title", 'Your Apps'), ("'id", 'jdoe@example.com'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app1')))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app2'))))),)
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('dashboards', 'jdoe@example.com', 'user.apps') : dash1}), mkref('apps', lambda id: None)) == dash1
    
    dash1compact = (("'feed", ("'title", 'Your Apps'), ("'id", 'jdoe@example.com'), ("'entry", ((("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app1')))), (("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app2'))))))),)
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('dashboards', 'jdoe@example.com', 'user.apps') : dash1compact}), mkref('apps', lambda id: None)) == dash1

    # Put an app in an empty dashboard
    cache1 = mkcache('cache', {})
    app1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app1')))),)
    dash1withapp1 = (("'feed", ("'title", 'Your Apps'), ("'id", 'jdoe@example.com'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app1'))))),)
    assert dashboards.put(('app1',), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == dash1withapp1
    assert dashboards.put(('app1',), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == dash1withapp1

    # Put a second app in the dashboard
    app2 = (("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app2')))),)
    dash1withapp2 = (("'feed", ("'title", 'Your Apps'), ("'id", 'jdoe@example.com'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app1')))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app2'))))),)
    assert dashboards.put(('app2',), app2, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == dash1withapp2
    assert dashboards.put(('app1',), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == dash1withapp2
    assert dashboards.put(('app2',), app2, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == dash1withapp2

    # Put a third app in the dashboard
    app3 = (("'entry", ("'title", 'app3'), ("'id", 'app3'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app3')))),)
    dash1withapp3 = (("'feed", ("'title", 'Your Apps'), ("'id", 'jdoe@example.com'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app1')))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app2')))), ("'entry", ("'title", 'app3'), ("'id", 'app3'), ("'author", 'jdoe@example.com'), ("'updated", 'Jan 02, 2012'), ("'content", ("'stats", ("'description", 'app3'))))),)
    assert dashboards.put(('app3',), app3, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == dash1withapp3
    assert dashboards.put(('app1',), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == dash1withapp3
    assert dashboards.put(('app2',), app2, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == dash1withapp3
    assert dashboards.put(('app3',), app3, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == dash1withapp3

    # Get an app from the user's dashboard
    assert dashboards.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == app1
    assert dashboards.get(('app2',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == app2
    assert dashboards.get(('app3',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == app3

    # Put a third app in the dashboard, starting from a compacted list
    cache2 = mkcache('cache', {('dashboards', 'jdoe@example.com', 'user.apps') : dash1compact})
    assert dashboards.put(('app3',), app3, mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', lambda id: None)) == dash1withapp3

    # Delete the apps
    assert dashboards.delete(('app2',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert dashboards.delete(('app4',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == False
    assert dashboards.delete(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert dashboards.delete(('app3',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: None)) == defdashboard

    # Delete the dashboard
    assert dashboards.delete((), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', lambda id: None)) == defdashboard
    return True

if __name__ == '__main__':
    print 'Testing...'
    testUser()
    testAccounts()
    testPages()
    testComposites()
    testApps()
    testStore()
    testDashboards()
    print 'OK'

