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

# App pages collection implementation
from util import *
from atomutil import *
from sys import debug

# Convert an id to a page id
def pageid(id):
    return ("apps", car(id), "htdocs", "app.html")

# Put a page into the page db
def put(id, page, user, cache, apps):
    debug('pages.py::put::id', id)
    debug('pages.py::put::page', page)

    # Get the requested app
    app = apps.get(id)
    if isNull(app):
        debug('pages.py::put', 'app not found', id)
        return False

    # Check app author
    if author(app) != user.get(()):
        debug('pages.py::put', 'different author', author(app))
        return False

    # Update the page in the page db
    pageentry = mkentry(title(app), car(id), user.get(()), now(), content(page))
    debug('pages.py::put::pageentry', pageentry)
    return cache.put(pageid(id), pageentry)

# Get a page from the page db
def get(id, user, cache, apps):
    debug('pages.py::get::id', id)
    if isNull(id):
        return (("'feed", ("'title", "Pages"), ("'id", "pages")),)

    # Get the requested app
    app = apps.get(id)
    if isNull(app):
        debug('pages.py::get', 'app not found', id)

        # Return a default new page
        return mkentry(car(id), car(id), user.get(()), now(), ())

    # Get the requested page
    page = cache.get(pageid(id))
    if isNull(page):
        debug('pages.py::get', 'page not found', id)

        # Return a default new page
        return mkentry(title(app), car(id), author(app), now(), ())

    # Return the page
    debug('pages.py::get::page', page)
    debug('pages.py::get::page::content', content(page))
    pageentry = mkentry(title(app), car(id), author(app), updated(page), content(page))
    debug('pages.py::get::pageentry', pageentry)
    return pageentry

# Delete a page from the page db
def delete(id, user, cache, apps):
    debug('pages.py::delete::id', id)

    # Get the requested app
    app = apps.get(id)
    if isNull(app):
        debug('pages.py::delete', 'app not found', id)
        return False

    # Check app author
    if author(app) != user.get(()):
        debug('pages.py::delete', 'different author', author(app))
        return False

    # Delete the page
    return cache.delete(pageid(id))

