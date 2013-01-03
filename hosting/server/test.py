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
from sys import debug
import time
time.strftime = lambda f, t: '2012-01-01T00:00:00+00:00'
try:
    import PIL
except:
    PIL = None

import unittest
from test.property import *
from test.reference import *
from test.cache import *

from util import *
from atomutil import *
import user
import accounts
import pictures
import pages
import icons
import composites
import apps
import store
import dashboards
import reviews
import ratings
import selector
import search

def testUser():
    # Return current user
    assert user.get((), mkprop('user', lambda: 'johndoe'), mkprop('email', lambda: 'jdoe@example.com'), mkprop('nick', lambda: 'jdoe'), mkprop('full', lambda: 'john doe'), mkprop('first', lambda: 'john'), mkprop('last', lambda: 'doe'), mkprop('realm', lambda: 'example.com'), mkprop('host', lambda: 'localhost')) == 'jdoe@example.com'
    return True

def testAccounts():
    # Get default account
    defaccount = (("'entry", ("'title", 'jdoe@example.com'), ("'id", 'jdoe@example.com'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content",)),)
    assert accounts.get((), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {})) == defaccount

    # Get user's account
    jdoe = (("'entry", ("'title", 'John Doe'), ("'id", 'jdoe@example.com'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'key", 'value'))),)
    assert accounts.get((), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('accounts', 'jdoe@example.com', 'user.account') : jdoe})) == jdoe

    # Put and get account
    cache1 = mkcache('cache', {})
    assert accounts.put((), jdoe, mkref('user', lambda id: 'jdoe@example.com'), cache1) == True
    assert accounts.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1) == jdoe
    return True

def testPictures():
    if PIL is None:
        return True
    img16 = 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAOUlEQVQ4y2N0b3/7nwEH2FkpzGg7hQGn/OEcBkYmBgrBqAFUMIBiQDCeL3qY4ZTX33FqNB0Mj3QAAFC7Dhs7i7zzAAAAAElFTkSuQmCC'
    img50 = 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADIAAAAyCAYAAAAeP4ixAAAAg0lEQVR4nO3YsRVFUBAG4Xu1oBCZTEwxGlDBa0AxxDKZQtRARAfrzWG+aLP/TLq5/e1HCjAPZb7uZkwhG0uf7o0iYuAfDKExhMYQGkNoDKExhMYQGkNoDKExRPqI/MTzbOvqkI1qWn3QYRlCYwiNITSG0BhCYwiNITSG0BhC85oQKcgJzukOV+8REuQAAAAASUVORK5CYII='

    # Put and get picture
    pic16 = (("'entry", ("'title", 'jdoe@example.com'), ("'id", 'jdoe@example.com'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'picture", ("'image", img16)))),)
    pic50 = (("'entry", ("'title", 'jdoe@example.com'), ("'id", 'jdoe@example.com'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'picture", ("'image", img50)))),)
    cache1 = mkcache('cache', {})
    assert pictures.put((), pic16, mkref('user', lambda id: 'jdoe@example.com'), cache1) == True
    assert pictures.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1) == pic50
    return True

def testPages():
    # Get default page
    defpage = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content",)),)
    app1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-03T00:00:00+00:00'), ("'content",)),)
    assert pages.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', lambda id: None)) == defpage
    defpagefromapp = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content",)),)
    assert pages.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', lambda id: app1)) == defpagefromapp

    # Get a page
    page1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content",)),)
    assert pages.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('apps', 'app1', 'htdocs', 'app.html') : page1}), mkref('apps', lambda id: app1)) == page1

    # Put and get a page
    cache1 = mkcache('cache', {})
    page1updated = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content",)),)
    assert pages.put(('app1',), page1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == True
    assert pages.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == page1updated
    
    # Reject put from user other than the author
    app1otherauthor = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jane@example.com'), ("'updated", '2012-01-03T00:00:00+00:00'), ("'content",)),)
    page1otherauthor = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jane@example.com'), ("'updated", '2012-01-02T00:00:00+00:00')),)
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

def testIcons():
    if PIL is None:
        return True
    img16 = 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAOUlEQVQ4y2N0b3/7nwEH2FkpzGg7hQGn/OEcBkYmBgrBqAFUMIBiQDCeL3qY4ZTX33FqNB0Mj3QAAFC7Dhs7i7zzAAAAAElFTkSuQmCC'
    img50 = 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADIAAAAyCAYAAAAeP4ixAAAAg0lEQVR4nO3YsRVFUBAG4Xu1oBCZTEwxGlDBa0AxxDKZQtRARAfrzWG+aLP/TLq5/e1HCjAPZb7uZkwhG0uf7o0iYuAfDKExhMYQGkNoDKExhMYQGkNoDKExRPqI/MTzbOvqkI1qWn3QYRlCYwiNITSG0BhCYwiNITSG0BhC85oQKcgJzukOV+8REuQAAAAASUVORK5CYII='

    # Get default icon
    deficon = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content",)),)
    app1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-03T00:00:00+00:00'), ()),)
    assert icons.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', lambda id: None)) == deficon
    deficonfromapp = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content",)),)
    assert icons.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', lambda id: app1)) == deficonfromapp

    # Get a icon
    icon1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'icon", ("'image", img16)))),)
    assert icons.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('apps', 'app1', 'app.icon') : icon1}), mkref('apps', lambda id: app1)) == icon1

    # Put and get a icon
    cache1 = mkcache('cache', {})
    icon1updated = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'icon", ("'image", img50)))),)
    assert icons.put(('app1',), icon1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == True
    assert icons.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == icon1updated
    
    # Reject put from user other than the author
    app1otherauthor = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jane@example.com'), ("'updated", '2012-01-03T00:00:00+00:00'), ("'content",)),)
    icon1otherauthor = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jane@example.com'), ("'updated", '2012-01-02T00:00:00+00:00')),)
    assert icons.put(('app1',), icon1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1otherauthor)) == False
    assert icons.put(('app1',), icon1otherauthor, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1otherauthor)) == False
    assert icons.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == icon1updated

    # Reject delete from user other than the author
    assert icons.delete(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1otherauthor)) == False
    assert icons.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == icon1updated

    # Put an upload token in an icon 
    icon1token = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'icon", ("'token", '1234')))),)
    assert icons.put(('app1',), icon1token, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == True
    assert icons.get(('app1',), mkref('user', lambda id: 'another@example.com'), cache1, mkref('apps', lambda id: app1)) == icon1updated
    icon1updatedwithtoken = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'icon", ("'image", img50), ("'token", '1234')))),)
    assert icons.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == icon1updatedwithtoken

    # Reject upload with invalid token
    icon1badtoken = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'icon", ("'image", img50), ("'token", '4567')))),)
    assert icons.put(('app1',), icon1badtoken, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == False

    # Upload with valid token
    icon1oktoken = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'icon", ("'image", img50), ("'token", '1234')))),)
    assert icons.put(('app1',), icon1oktoken, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == True
    assert icons.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == icon1updated

    # Delete a icon
    assert icons.delete(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == True
    assert icons.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == deficonfromapp
    return True

def testComposites():
    # Get default composite
    defcomposite = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content",)),)
    app1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-03T00:00:00+00:00'), ("'content",)),)
    assert composites.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', lambda id: None)) == defcomposite
    defcompositefromapp = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content",)),)
    assert composites.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', lambda id: app1)) == defcompositefromapp

    # Get a composite
    composite1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content",)),)
    assert composites.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('apps', 'app1', 'app.composite') : composite1}), mkref('apps', lambda id: app1)) == composite1

    # Put and get a composite
    cache1 = mkcache('cache', {})
    composite1updated = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content",)),)
    assert composites.put(('app1',), composite1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == True
    assert composites.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1)) == composite1updated
    
    # Reject put from user other than the author
    app1otherauthor = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jane@example.com'), ("'updated", '2012-01-03T00:00:00+00:00'), ("'content",)),)
    composite1otherauthor = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jane@example.com'), ("'updated", '2012-01-02T00:00:00+00:00')),)
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
    assert apps.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None), mkref('icons', lambda id: None)) is None

    # Get an app
    app1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", '')))),)
    assert apps.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('apps', 'app1', 'app.info') : app1}), mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None), mkref('icons', lambda id: None)) == app1

    # Put and get an app
    cache1 = mkcache('cache', {})
    assert apps.put(('app1',), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('dashboard', lambda id, app: True), mkref('store', lambda id, app: True), mkref('composites', lambda id, app: True), mkref('pages', lambda id, app: True), mkref('icons', lambda id, app: True)) == True
    assert apps.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None), mkref('icons', lambda id: None)) == app1
    return True
    
    # Reject put from user other than the author
    app1otherauthor = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jane@example.com'), ("'updated", '2012-01-03T00:00:00+00:00'), ("'content",)),)
    assert apps.put(('app1',), app1, mkref('user', lambda id: 'jane@example.com'), cache1, mkref('dashboard', lambda id, app: True), mkref('store', lambda id, app: True), mkref('composites', lambda id, app: True), mkref('pages', lambda id, app: True), mkref('icons', lambda id, app: True)) == false
    assert apps.get(('app1',), mkref('user', lambda id: 'jane@example.com'), cache1, mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None), mkref('icons', lambda id: None)) == app1

    # Reject delete from user other than the author
    assert apps.delete(('app1',), mkref('user', lambda id: 'jane@example.com'), cache1, mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None), mkref('icons', lambda id: None)) == False
    assert apps.get(('app1',), mkref('user', lambda id: 'jane@example.com'), cache1, mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None), mkref('icons', lambda id: None)) == app1

    # Delete an app
    assert apps.delete(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None), mkref('icons', lambda id: None)) == True
    assert apps.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('dashboard', lambda id: None), mkref('store', lambda id: None), mkref('composites', lambda id: None), mkref('pages', lambda id: None), mkref('icons', lambda id: None)) is None
    return True

def testStore():
    getapp = lambda id: (("'entry", ("'title", car(id)), ("'id", car(id)), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", car(id))))),)

    # Get default store
    defstore = (("'feed", ("'title", 'App Store'), ("'id", 'featured')),)
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', getapp), mkref('ratings', lambda id: None)) == defstore

    # Get a store
    store1= (("'feed", ("'title", 'App Store'), ("'id", 'featured'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app1')))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app2'))))),)
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('store', 'featured', 'store.apps') : store1}), mkref('apps', getapp), mkref('ratings', lambda id: None)) == store1
    
    store1compact = (("'feed", ("'title", 'App Store'), ("'id", 'featured'), ("'entry", ((("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app1')))), (("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app2'))))))),)
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('store', 'featured', 'store.apps') : store1compact}), mkref('apps', getapp), mkref('ratings', lambda id: None)) == store1

    # Put an app in an empty store
    cache1 = mkcache('cache', {})
    app1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app1')))),)
    store1withapp1 = (("'feed", ("'title", 'App Store'), ("'id", 'featured'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app1'))))),)
    assert store.put(('featured', 'app1'), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == store1withapp1
    assert store.put(('featured', 'app1'), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == store1withapp1

    # Put a second app in the store
    app2 = (("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app2')))),)
    store1withapp2 = (("'feed", ("'title", 'App Store'), ("'id", 'featured'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app1')))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app2'))))),)
    assert store.put(('featured', 'app2'), app2, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == store1withapp2
    assert store.put(('featured', 'app1'), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == store1withapp2
    assert store.put(('featured', 'app2'), app2, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == store1withapp2

    # Put a third app in the store
    app3 = (("'entry", ("'title", 'app3'), ("'id", 'app3'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app3')))),)
    store1withapp3 = (("'feed", ("'title", 'App Store'), ("'id", 'featured'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app1')))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app2')))), ("'entry", ("'title", 'app3'), ("'id", 'app3'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app3'))))),)
    assert store.put(('featured', 'app3'), app3, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == store1withapp3
    assert store.put(('featured', 'app1'), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == store1withapp3
    assert store.put(('featured', 'app2'), app2, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == store1withapp3
    assert store.put(('featured', 'app3'), app3, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == store1withapp3

    # Get an app from the store
    assert store.get(('featured','app1'), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == app1
    assert store.get(('featured','app2'), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == app2
    assert store.get(('featured','app3'), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == app3

    # Put a third app in the store, starting from a compacted list
    cache2 = mkcache('cache', {('store', 'featured', 'store.apps') : store1compact})
    assert store.put(('featured', 'app3'), app3, mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', getapp), mkref('ratings', lambda id: None)) == store1withapp3

    # Delete the apps
    assert store.delete(('featured', 'app2'), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert store.delete(('featured', 'app4'), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == False
    assert store.delete(('featured', 'app1'), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert store.delete(('featured', 'app3'), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == defstore

    # Delete a store
    assert store.delete(('featured',), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert store.get(('featured',), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', getapp), mkref('ratings', lambda id: None)) == defstore
    return True

def testDashboards():
    getapp = lambda id: (("'entry", ("'title", car(id)), ("'id", car(id)), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", car(id))))),)

    # Get default dashboard
    defdashboard = (("'feed", ("'title", 'Your Apps'), ("'id", 'jdoe@example.com')),)
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', getapp), mkref('ratings', lambda id: None)) == defdashboard

    # Get the user's dashboard
    dash1= (("'feed", ("'title", 'Your Apps'), ("'id", 'jdoe@example.com'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app1')))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app2'))))),)
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('dashboards', 'jdoe@example.com', 'user.apps') : dash1}), mkref('apps', getapp), mkref('ratings', lambda id: None)) == dash1
    
    dash1compact = (("'feed", ("'title", 'Your Apps'), ("'id", 'jdoe@example.com'), ("'entry", ((("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app1')))), (("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app2'))))))),)
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('dashboards', 'jdoe@example.com', 'user.apps') : dash1compact}), mkref('apps', getapp), mkref('ratings', lambda id: None)) == dash1

    # Put an app in an empty dashboard
    cache1 = mkcache('cache', {})
    app1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app1')))),)
    dash1withapp1 = (("'feed", ("'title", 'Your Apps'), ("'id", 'jdoe@example.com'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app1'))))),)
    assert dashboards.put(('app1',), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == dash1withapp1
    assert dashboards.put(('app1',), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == dash1withapp1

    # Put a second app in the dashboard
    app2 = (("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app2')))),)
    dash1withapp2 = (("'feed", ("'title", 'Your Apps'), ("'id", 'jdoe@example.com'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app1')))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app2'))))),)
    assert dashboards.put(('app2',), app2, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == dash1withapp2
    assert dashboards.put(('app1',), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == dash1withapp2
    assert dashboards.put(('app2',), app2, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == dash1withapp2

    # Put a third app in the dashboard
    app3 = (("'entry", ("'title", 'app3'), ("'id", 'app3'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app3')))),)
    dash1withapp3 = (("'feed", ("'title", 'Your Apps'), ("'id", 'jdoe@example.com'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app1')))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app2')))), ("'entry", ("'title", 'app3'), ("'id", 'app3'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app3'))))),)
    assert dashboards.put(('app3',), app3, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == dash1withapp3
    assert dashboards.put(('app1',), app1, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == dash1withapp3
    assert dashboards.put(('app2',), app2, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == dash1withapp3
    assert dashboards.put(('app3',), app3, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == dash1withapp3

    # Get an app from the user's dashboard
    assert dashboards.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == app1
    assert dashboards.get(('app2',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == app2
    assert dashboards.get(('app3',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == app3

    # Put a third app in the dashboard, starting from a compacted list
    cache2 = mkcache('cache', {('dashboards', 'jdoe@example.com', 'user.apps') : dash1compact})
    assert dashboards.put(('app3',), app3, mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', getapp), mkref('ratings', lambda id: None)) == dash1withapp3

    # Delete the apps
    assert dashboards.delete(('app2',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert dashboards.delete(('app4',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == False
    assert dashboards.delete(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert dashboards.delete(('app3',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', getapp), mkref('ratings', lambda id: None)) == defdashboard

    # Delete the dashboard
    assert dashboards.delete((), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', getapp), mkref('ratings', lambda id: None)) == True
    assert dashboards.get((), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', getapp), mkref('ratings', lambda id: None)) == defdashboard
    return True

def testReviews():
    app1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'info", ("'description", '')))),)

    # Get default reviews
    defreviews = (("'feed", ("'title", 'Your Reviews'), ("'id", 'jdoe@example.com')),)
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == defreviews

    # Get the user's reviews
    reviews1= (("'feed", ("'title", 'Your Reviews'), ("'id", 'jdoe@example.com'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'rating", '1'))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'rating", '2')))),)
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('reviews', 'jdoe@example.com', 'user.reviews') : reviews1}), mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == reviews1
    
    reviews1compact = (("'feed", ("'title", 'Your Reviews'), ("'id", 'jdoe@example.com'), ("'entry", ((("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'rating", '1'))), (("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'rating", '2')))))),)
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('reviews', 'jdoe@example.com', 'user.reviews') : reviews1compact}), mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == reviews1

    # Put a review in an empty reviews record
    cache1 = mkcache('cache', {})
    app1review = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'rating", '1'))),)
    reviews1withapp1 = (("'feed", ("'title", 'Your Reviews'), ("'id", 'jdoe@example.com'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'rating", '1')))),)
    assert reviews.put(('app1',), app1review, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == True
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == reviews1withapp1
    assert reviews.put(('app1',), app1review, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == True
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == reviews1withapp1

    # Put a second review in the reviews record
    app2review = (("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'rating", '2'))),)
    reviews1withapp2 = (("'feed", ("'title", 'Your Reviews'), ("'id", 'jdoe@example.com'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'rating", '1'))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'rating", '2')))),)
    assert reviews.put(('app2',), app2review, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == True
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == reviews1withapp2
    assert reviews.put(('app1',), app1review, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == True
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == reviews1withapp2
    assert reviews.put(('app2',), app2review, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == True
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == reviews1withapp2

    # Put a third review in the reviews record
    app3review = (("'entry", ("'title", 'app3'), ("'id", 'app3'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'rating", '3'))),)
    reviews1withapp3 = (("'feed", ("'title", 'Your Reviews'), ("'id", 'jdoe@example.com'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'rating", '1'))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'rating", '2'))), ("'entry", ("'title", 'app3'), ("'id", 'app3'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'rating", '3')))),)
    assert reviews.put(('app3',), app3review, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == True
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == reviews1withapp3
    assert reviews.put(('app1',), app1review, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == True
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == reviews1withapp3
    assert reviews.put(('app2',), app2review, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == True
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == reviews1withapp3
    assert reviews.put(('app3',), app3review, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == True
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == reviews1withapp3

    # Get a review from the user's reviews
    assert reviews.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == app1review
    assert reviews.get(('app2',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == app2review
    assert reviews.get(('app3',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == app3review

    # Put a third review in the user's reviews record, starting from a compacted list
    cache2 = mkcache('cache', {('reviews', 'jdoe@example.com', 'user.reviews') : reviews1compact})
    assert reviews.put(('app3',), app3review, mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == True
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == reviews1withapp3

    # Delete the reviews
    assert reviews.delete(('app2',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == True
    assert reviews.delete(('app4',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == False
    assert reviews.delete(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == True
    assert reviews.delete(('app3',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == True
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == defreviews

    # Delete the reviews record
    assert reviews.delete((), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == True
    assert reviews.get((), mkref('user', lambda id: 'jdoe@example.com'), cache2, mkref('apps', lambda id: app1), mkref('ratings', lambda id, patch: True)) == defreviews
    return True

def testRatings():
    # Get default ratings
    defratings = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content",)),)
    app1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-03T00:00:00+00:00'), ("'content",)),)
    assert ratings.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('db', lambda id: None), mkref('apps', lambda id: None)) == defratings
    defratingsfromapp = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content",)),)
    assert ratings.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('db', lambda id: None), mkref('apps', lambda id: app1)) == defratingsfromapp

    # Get an app ratings
    ratings1 = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content",)),)
    assert ratings.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {('ratings', 'app1', 'app.ratings') : ratings1}), mkref('db', lambda id: None), mkref('apps', lambda id: app1)) == ratings1

    # Put and get an app ratings
    cache1 = mkcache('cache', {})
    ratings1updated = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content",)),)
    assert ratings.put(('app1',), ratings1, mkref('user', lambda id: 'admin'), cache1, mkref('db', lambda id: None), mkref('apps', lambda id: app1)) == True
    assert ratings.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('db', lambda id: None), mkref('apps', lambda id: app1)) == ratings1updated
    
    # Reject put from user other than admin
    ratings1otheruser = (("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-04T00:00:00+00:00'), ("'content",)),)
    assert ratings.put(('app1',), ratings1otheruser, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('db', lambda id: None), mkref('apps', lambda id: app1)) == False
    assert ratings.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('db', lambda id: None), mkref('apps', lambda id: app1)) == ratings1updated

    # Patch an app ratings
    app2 = (("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-03T00:00:00+00:00'), ("'content",)),)
    ratings2patch = (("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-01T00:00:00+00:00'), ("'content", ("'patch", ("'old", '3'), ("'new", '4')))),)
    assert ratings.patch(('app2',), ratings2patch, mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('db', lambda id: None), mkref('apps', lambda id: app2)) == True
    ratings2patched = cache1.get(("ratings", 'app2', "app.ratings"))
    assert(cadr(content(ratings2patched)).find('(define orating 3)') != -1)
    assert(cadr(content(ratings2patched)).find('(define nrating 4)') != -1)

    # Reject delete from user other than admin
    assert ratings.delete(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('db', lambda id: None), mkref('apps', lambda id: app1)) == False
    assert ratings.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('db', lambda id: None), mkref('apps', lambda id: app1)) == ratings1updated

    # Delete an app ratings
    assert ratings.delete(('app1',), mkref('user', lambda id: 'admin'), cache1, mkref('db', lambda id: None), mkref('apps', lambda id: app1)) == True
    assert ratings.get(('app1',), mkref('user', lambda id: 'jdoe@example.com'), cache1, mkref('db', lambda id: None), mkref('apps', lambda id: app1)) == defratingsfromapp
    return True

def testSelector():
    assert selector.get(('apps', 'abc', 'app.info'), (1, 2)) == (1,)
    assert selector.get(('apps', 'nbc', 'app.info'), (1, 2)) == (2,)
    assert selector.get(('apps', 'nbc', 'app.info', ("'xyz", 'utv')), (1, 2)) == (2,)
    assert selector.get(('apps', 'nbc', 'app.info', ("'limit", '10')), (1, 2)) == (1, 2)
    return True

def testSearch():
    assert search.get((("'q", 'abc def'),), mkref('user', lambda id: 'jdoe@example.com'), mkcache('cache', {}), mkref('db', lambda id: ((("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app1')))),), (("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app2')))),))), mkref('apps', lambda id: None), mkref('ratings', lambda id: None)) == (("'feed", ("'title", 'Search Results'), ("'id", 'search'), ("'entry", ("'title", 'app1'), ("'id", 'app1'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app1')))), ("'entry", ("'title", 'app2'), ("'id", 'app2'), ("'author", 'jdoe@example.com'), ("'updated", '2012-01-02T00:00:00+00:00'), ("'content", ("'info", ("'description", 'app2'))))),)
    return True

if __name__ == '__main__':
    print 'Testing...'
    testUser()
    testAccounts()
    testPictures()
    testPages()
    testComposites()
    testIcons()
    testApps()
    testStore()
    testDashboards()
    testReviews()
    testRatings()
    testSelector()
    testSearch()
    print 'OK'

