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

# Apps collection implementation
import uuid
import sys
import os
from util import *

# Convert an id to an app id
def appid(id):
    return ("'" + car(id), "'app.composite")

# Link implementation resources into an app
def mkapplink(id):
    try:
        os.symlink('../../../../../nuvem/nuvem-parallel/nuvem', 'apps/' + car(id) + '/nuvem')
        os.mkdir('apps/' + car(id) + '/htdocs')
        os.symlink('../../../htdocs/login', 'apps/' + car(id) + '/htdocs/login');
        os.symlink('../../../htdocs/logout', 'apps/' + car(id) + '/htdocs/logout');
    except:
        pass
    return True

# Post a new app to the apps db
def post(collection, app, cache):
    id = appid((str(uuid.uuid1()),))
    comp = caddr(app)
    cache.put((id,), comp)
    mkapplink((id,))
    return id

# Put an app into the apps db
def put(id, app, cache):
    comp = caddr(app)
    cache.put(appid(id), comp)
    mkapplink(id)
    return True

# Get an app from the apps db
def get(id, cache):
    if isNil(id):
        return ("Apps", "apps")
    return (car(id), car(id), cache.get(appid(id)))

# Delete an app from the apps db
def delete(id, cache):
    cache.delete(appid(id))
    return True

