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
import os
from util import *

# Convert an id to an app id
def appid(id):
    return ("'apps", "'" + car(id), "'app.stats")

# Link implementation resources into an app
def mkapplink(id):
    try:
        os.symlink('../../../../../../nuvem/nuvem-parallel/nuvem', 'data/apps/' + car(id) + '/nuvem')
        os.symlink('../../../../../components', 'data/apps/' + car(id) + '/lib')
        os.mkdir('data/apps/' + car(id) + '/htdocs')
    except:
        pass
    return True

# Put an app into the apps db
def put(id, app, cache, store, composites, pages):
    eid = cadr(caddr(car(app)))
    appentry = (("'entry", cadr(car(app)), ("'id", car(id))),)

    # Update app in apps db
    if car(id) == eid:
        cache.put(appid(id), appentry)
        mkapplink(id)
        return True

    # Clone an app's composite and page
    cache.put(appid(id), appentry)
    mkapplink(id)
    composites.put(id, composites.get((eid,)))
    pages.put(id, pages.get((eid,)))
    return True

# Get an app from the apps db
def get(id, cache, store, composites, pages):
    if isNil(id):
        return (("'feed", ("'title", "Apps"), ("'id", "apps")),)
    app = cache.get(appid(id));
    if isNil(app) or app is None:
        return (("'entry", ("'title", car(id)), ("'id", car(id))),)
    return app

# Delete an app from the apps db
def delete(id, cache, store, composites, pages):
    cache.delete(appid(id))
    composites.delete(id)
    pages.delete(id)
    return True

