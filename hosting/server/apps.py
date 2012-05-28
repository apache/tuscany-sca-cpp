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

# App collection implementation
from time import strftime
from util import *
from sys import debug

# Convert an id to an app id
def appid(id):
    return ("apps", car(id), "app.stats")

# Put an app into the apps db
def put(id, app, user, cache, dashboard, store, composites, pages):
    debug('apps.py::put::id', id)
    debug('apps.py::put::app', app)

    # Update an app
    eid = cadr(assoc("'id", car(app)))
    if car(id) == eid:
        # Check app author
        eapp = cache.get(appid(id));
        if (not (isNil(eapp) or eapp is None)) and (cadr(assoc("'author", car(eapp))) != user.get(())):
            debug('apps.py::put', 'different author', cadr(assoc("'author", car(eapp))))
            return False

        # Update the app in the apps db
        appentry = (("'entry", assoc("'title", car(app)), ("'id", car(id)), ("'author", user.get(())), ("'updated", strftime('%b %d, %Y')), assoc("'content", car(app))),)
        debug('apps.py::put::appentry', appentry)
        cache.put(appid(id), appentry)
        dashboard.put(id, appentry)

        # Create new page and composite if necessary
        if isNil(eapp) or eapp is None:
            comp = (("'entry", ("'title", car(id)), ("'id", car(id))),)
            composites.put(id, comp);
            page = (("'entry", ("'title", car(id)), ("'id", car(id))),)
            pages.put(id, comp);
            return True
        return True

    # Check app author
    eapp = cache.get(appid(id));
    if (not (isNil(eapp) or eapp is None)) and (cadr(assoc("'author", car(eapp))) != user.get(())):
        debug('apps.py::put', 'different author', cadr(assoc("'author", car(eapp))))
        return False

    # Clone an app
    appentry = (("'entry", assoc("'title", car(app)), ("'id", car(id)), ("'author", user.get(())), ("'updated", strftime('%b %d, %Y')), assoc("'content", car(app))),)
    debug('apps.py::put::appentry', appentry)
    cache.put(appid(id), appentry)
    composites.put(id, composites.get((eid,)))
    pages.put(id, pages.get((eid,)))
    dashboard.put(id, appentry)
    return True

# Get an app from the apps db
def get(id, user, cache, dashboard, store, composites, pages):
    debug('apps.py::get::id', id)
    if isNil(id):
        return (("'feed", ("'title", "Apps"), ("'id", "apps")),)

    # Get the requested app
    app = cache.get(appid(id));
    if isNil(app) or app is None:
        debug('apps.py::get', 'app not found', id)

        # Return a default new app
        return (("'entry", ("'title", car(id)), ("'id", car(id)), ("'author", user.get(())), ("'updated", strftime('%b %d, %Y')), ("'content", ("'stats", ("'description", '')))),)

    # Return the app
    debug('apps.py::get::app', app)
    return app

# Delete an app from the apps db
def delete(id, user, cache, dashboard, store, composites, pages):
    debug('apps.py::delete::id', id)

    # Get the requested app
    app = cache.get(appid(id));
    if isNil(app) or app is None:
        debug('apps.py::delete', 'app not found', id)
        return False

    # Check app author
    author = cadr(assoc("'author", car(app)))
    if author != user.get(()):
        debug('apps.py::delete', 'different author', author)
        return False

    # Delete the app, its composite and page
    dashboard.delete(id)
    composites.delete(id)
    pages.delete(id)
    cache.delete(appid(id))
    return True

