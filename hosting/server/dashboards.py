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

# Dashboards collection implementation
from util import *
from atomutil import *
from sys import debug

# Convert a particular user id to a dashboard id
def dashboardid(user):
    return ("dashboards", user.get(()), "user.apps")

# Get a dashboard from the cache
def getdashboard(id, cache):
    debug('dashboards.py::getdashboard::id', id)
    val = cache.get(id)
    if isNull(val):
        return ()
    dashboard = cdddr(car(val))
    if not isNull(dashboard) and isList(car(cadr(car(dashboard)))):
        # Expand list of entries
        edashboard = tuple(map(lambda e: cons("'entry", e), cadr(car(dashboard))))
        debug('dashboards.py::getdashboard::edashboard', edashboard)
        return edashboard

    debug('dashboards.py::getdashboard::dashboard', dashboard)
    return dashboard

# Put a dashboard into the cache
def putdashboard(id, dashboard, cache):
    debug('dashboards.py::putdashboard::id', id)
    debug('dashboards.py::putdashboard::dashboard', dashboard)
    val = ((("'feed", ("'title", "Your Apps"), ("'id", cadr(id))) + dashboard),)
    return cache.put(id, val)

# Put an app into the user's dashboard
def put(id, app, user, cache, apps, ratings):
    debug('dashboards.py::put::id', id)
    debug('dashboards.py::put::app', app)

    def putapp(id, app, dashboard):
        if isNull(dashboard):
            return app
        if car(id) == entryid(dashboard):
            return cons(car(app), cdr(dashboard))
        return cons(car(dashboard), putapp(id, app, cdr(dashboard)))

    appentry = mkentry(title(app), car(id), user.get(()), now(), content(app))
    debug('dashboards.py::put::appentry', appentry)

    dashboard = putapp(id, appentry, getdashboard(dashboardid(user), cache))
    return putdashboard(dashboardid(user), dashboard, cache)

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

# Get apps from the user's dashboard
def get(id, user, cache, apps, ratings):
    debug('dashboards.py::get::id', id)

    def findapp(id, dashboard):
        if isNull(dashboard):
            return None
        if car(id) == entryid(dashboard):
            return (car(dashboard),)
        return findapp(id, cdr(dashboard))

    if isNull(id):
        entries = mergeapps(getdashboard(dashboardid(user), cache), apps, ratings)
        def sortkey(e):
            return updated((e,))
        sortedentries = tuple(sorted(entries, key = sortkey, reverse = True))
        dashboard = ((("'feed", ("'title", "Your Apps"), ("'id", user.get(()))) + sortedentries),)
        debug('dashboards.py::get::dashboard', dashboard)
        return dashboard

    app = findapp(id, getdashboard(dashboardid(user), cache))
    debug('dashboards.py::get::app', app)
    return app

# Delete apps from the user's dashboard
def delete(id, user, cache, apps, ratings):
    debug('dashboards.py::delete::id', id)
    if isNull(id):
        return cache.delete(dashboardid(user))

    def deleteapp(id, dashboard):
        if isNull(dashboard):
            return ()
        if car(id) == entryid(dashboard):
            return cdr(dashboard)
        return cons(car(dashboard), deleteapp(id, cdr(dashboard)))

    dashboard = getdashboard(dashboardid(user), cache)
    deleted = deleteapp(id, dashboard)
    if deleted == dashboard:
        return False
    return putdashboard(dashboardid(user), deleted, cache)

