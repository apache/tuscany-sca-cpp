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
import uuid
import sys
from util import *

# Convert a particular user email to a dashboard id
def dashboardid(user):
    return ("'" + user.id(),)

# Get a dashboard from the cache
def getdashboard(id, cache):
    dashboard = cache.get(id)
    if isNil(dashboard):
        return ()
    return dashboard

# Post a new app to the user's dashboard
def post(collection, app, user, cache):
    id = (str(uuid.uuid1()),)
    newapp = list("'entry", cadr(car(app)), list("'id", id), cadddr(car(app)))
    dashboard = cons(newapp, getdashboard(dashboardid(user), cache))
    cache.put(dashboardid(user), dashboard)
    return id

# Put an app into the user's dashboard
def put(id, app, user, cache):
    def putapp(app, dashboard):
        if isNil(dashboard):
            return app
        if cadr(caddr(car(app))) == cadr(caddr(car(dashboard))):
            return cons(car(app), cdr(dashboard))
        return cons(car(dashboard), putapp(app, cdr(dashboard)))

    dashboard = putapp(app, getdashboard(dashboardid(user), cache))
    cache.put(dashboardid(user), dashboard)
    return True

# Get apps from the user's dashboard
def get(id, user, cache):
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
def delete(id, user, cache):
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
    return True

