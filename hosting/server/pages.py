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
from time import strftime
from util import *
from sys import debug

# Convert an id to a page id
def pageid(id):
    return ("apps", car(id), "htdocs", "app.html")

# Put a page into the page db
def put(id, page, user, cache, apps):
    debug('pages.py::put::id', id)
    debug('pages.py::put::page', page)

    # Get the requested app
    app = apps.get(id);
    if isNil(app) or app is None:
        debug('pages.py::put', 'app not found', id)
        return False

    # Check app author
    author = cadr(assoc("'author", car(app)))
    if author != user.get(()):
        debug('pages.py::put', 'different author', author)
        return False

    # Update the page in the page db
    pageentry = (("'entry", assoc("'title", car(app)), ("'id", car(id)), ("'author", user.get(())), ("'updated", strftime('%b %d, %Y')), assoc("'content", car(page))),)
    debug('pages.py::put::pageentry', pageentry)
    return cache.put(pageid(id), pageentry)

# Get a page from the page db
def get(id, user, cache, apps):
    debug('pages.py::get::id', id)
    if isNil(id):
        return (("'feed", ("'title", "Pages"), ("'id", "pages")),)

    # Get the requested app
    app = apps.get(id)
    if isNil(app) or app is None:
        debug('pages.py::get', 'app not found', id)

        # Return a default new page
        return (("'entry", ("'title", car(id)), ("'id", car(id)), ("'author", user.get(())), ("'updated", strftime('%b %d, %Y'))),)

    # Get the requested page
    page = cache.get(pageid(id))
    if isNil(page) or page is None:
        debug('pages.py::get', 'page not found', id)

        # Return a default new page
        return (("'entry", ("'title", car(id)), ("'id", car(id)), assoc("'author", car(app)), assoc("'updated", car(app))),)

    # Return the page
    def updated(u):
        return assoc("'updated", car(app)) if isNil(u) or u is None else u
    pageentry = (("'entry", assoc("'title", car(app)), ("'id", car(id)), assoc("'author", car(app)), updated(assoc("'updated", car(page))), assoc("'content", car(page))),)
    debug('pages.py::get::pageentry', pageentry)
    return pageentry

# Delete a page from the page db
def delete(id, user, cache, apps):
    debug('pages.py::delete::id', id)

    # Get the requested app
    app = apps.get(id);
    if isNil(app) or app is None:
        debug('pages.py::delete', 'app not found', id)
        return False

    # Check app author
    author = cadr(assoc("'author", car(app)))
    if author != user.get(()):
        debug('pages.py::delete', 'different author', author)
        return False

    # Delete the page
    return cache.delete(pageid(id))

