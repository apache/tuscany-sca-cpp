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

# Convert a particular user id to a dashboard id
def dashboardid(user):
    return ("'dashboards", "'" + user.id(), "'user.apps")

# Get a dashboard from the cache
def getdashboard(id, cache):
    dashboard = cache.get(id)
    if isNil(dashboard) or dashboard is None:
        return ()
    return dashboard

# Put an app into the user's dashboard
def put(id, app, user, cache, apps):
    def putapp(app, dashboard):
        if isNil(dashboard):
            return app
        if cadr(caddr(car(app))) == cadr(caddr(car(dashboard))):
            return cons(car(app), cdr(dashboard))
        return cons(car(dashboard), putapp(app, cdr(dashboard)))

    appentry = (("'entry", cadr(car(app)), ("'id", car(id))),)
    dashboard = putapp(appentry, getdashboard(dashboardid(user), cache))
    cache.put(dashboardid(user), dashboard)

    # Update app in app repository
    apps.put(id, app);
    return True

# Get apps from the user's dashboard
def get(id, user, cache, apps):
    def findapp(id, dashboard):
        if isNil(dashboard):
            return None
        if car(id) == cadr(caddr(car(dashboard))):
            return (car(dashboard),)
        return findapp(id, cdr(dashboard))

    if isNil(id):
        return ((("'feed", ("'title", "Your Apps"), ("'id", user.id())) + getdashboard(dashboardid(user), cache)),)
    return findapp(id, getdashboard(dashboardid(user), cache))

# Delete apps from the user's dashboard
def delete(id, user, cache, apps):
    if isNil(id):
        return cache.delete(dashboardid(user))

    def deleteapp(id, dashboard):
        if isNil(dashboard):
            return ()
        if car(id) == cadr(caddr(car(dashboard))):
            return cdr(dashboard)
        return cons(car(dashboard), deleteapp(id, cdr(dashboard)))

    dashboard = deleteapp(id, getdashboard(dashboardid(user), cache))
    cache.put(dashboardid(user), dashboard)

    # Delete app from app repository
    apps.delete(id);
    return True

