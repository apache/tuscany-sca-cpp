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
from util import *
from atomutil import *
from sys import debug

# Convert an id to an app id
def appid(id):
    return ("apps", car(id), "app.info")

# Put an app into the apps db
def put(id, app, user, cache, dashboard, store, composites, pages, icons):
    debug('apps.py::put::id', id)
    debug('apps.py::put::app', app)

    # Update an app
    eid = entryid(app)
    if car(id) == eid:
        # Check app author
        eapp = cache.get(appid(id))
        if (not isNull(eapp)) and (author(eapp) != user.get(())):
            debug('apps.py::put', 'different author', author(eapp))
            return False

        # Update the app in the apps db
        appentry = mkentry(title(app), car(id), user.get(()), now(), content(app))
        debug('apps.py::put::appentry', appentry)
        cache.put(appid(id), appentry)
        dashboard.put(id, appentry)

        # Create new page and composite if necessary
        if isNull(eapp):
            comp = mkentry(car(id), car(id), user.get(()), now(), ())
            composites.put(id, comp)
            page = mkentry(car(id), car(id), user.get(()), now(), ())
            pages.put(id, page)
            icon = mkentry(car(id), car(id), user.get(()), now(), ())
            icons.put(id, icon)
            return True
        return True

    # Check app author
    eapp = cache.get(appid(id))
    if (not isNull(eapp)) and (author(eapp) != user.get(())):
        debug('apps.py::put', 'different author', author(eapp))
        return False

    # Get app to clone
    capp = cache.get(appid((eid,)))
    if isNull(capp):
        debug('apps.py::put', 'cloned app not found', (eid,))
        return False

    # Clone app
    appentry = mkentry(title(app), car(id), user.get(()), now(), content(app))
    debug('apps.py::put::appentry', appentry)
    cache.put(appid(id), appentry)
    composites.put(id, composites.get((eid,)))
    pages.put(id, pages.get((eid,)))
    icons.put(id, icons.get((eid,)))
    dashboard.put(id, appentry)
    return True

# Get an app from the apps db
def get(id, user, cache, dashboard, store, composites, pages, icons):
    debug('apps.py::get::id', id)
    if isNull(id):
        return (("'feed", ("'title", "Apps"), ("'id", "apps")),)

    # Get the requested app
    app = cache.get(appid(id))
    if isNull(app):
        debug('apps.py::get', 'app not found', id)
        return None

    # Return the app
    debug('apps.py::get::app', app)
    return app

# Delete an app from the apps db
def delete(id, user, cache, dashboard, store, composites, pages, icons):
    debug('apps.py::delete::id', id)

    # Get the requested app
    app = cache.get(appid(id))
    if isNull(app):
        debug('apps.py::delete', 'app not found', id)
        return False

    # Check app author
    if author(app) != user.get(()):
        debug('apps.py::delete', 'different author', author(app))
        return False

    # Delete the app, its composite and page
    dashboard.delete(id)
    composites.delete(id)
    pages.delete(id)
    icons.delete(id)
    cache.delete(appid(id))
    return True

