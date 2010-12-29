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

# Workspace collection implementation
import uuid
import sys
from util import *

# Convert a particular user email to a workspace id
def workspaceid(user):
    return ("'" + user.id(),)

# Get a workspace from the cache
def getworkspace(id, cache):
    workspace = cache.get(id)
    if isNil(workspace):
        return ()
    return workspace

# Post a new app to the user's workspace
def post(collection, app, user, cache):
    id = (str(uuid.uuid1()),)
    workspace = cons((car(app), id, caddr(app)), getworkspace(workspaceid(user), cache))
    cache.put(workspaceid(user), workspace)
    return id

# Put an app into the user's workspace
def put(id, app, user, cache):
    def putapp(app, workspace):
        if isNil(workspace):
            return (app,)
        if cadr(app) == cadr(car(workspace)):
            return cons(app, cdr(workspace))
        return cons(car(workspace), putapp(app, cdr(workspace)))

    workspace = putapp(app, getworkspace(workspaceid(user), cache))
    cache.put(workspaceid(user), workspace)
    return True

# Get apps from the user's workspace
def get(id, user, cache):
    def findapp(id, workspace):
        if isNil(workspace):
            return None
        if id == cadr(car(workspace)):
            return car(workspace)
        return findapp(id, cdr(workspace))

    if isNil(id):
        return ("Your Apps", user.id()) + getworkspace(workspaceid(user), cache)
    return findapp(car(id), getworkspace(workspaceid(user), cache))

# Delete apps from the user's workspace
def delete(id, user, cache):
    if isNil(id):
        return cache.delete(workspaceid(user))

    def deleteapp(id, workspace):
        if isNil(workspace):
            return ()
        if id == cadr(car(workspace)):
            return cdr(workspace)
        return cons(car(workspace), deleteapp(id, cdr(workspace)))

    workspace = deleteapp(id, getworkspace(workspaceid(user), cache))
    cache.put(workspaceid(user), workspace)
    return True

