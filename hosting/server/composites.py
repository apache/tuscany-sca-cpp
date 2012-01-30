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

# App composites collection implementation
from util import *

# Convert an id to an app id
def appid(id):
    return ("'apps", "'" + car(id), "'app.composite")

# Put an app into the apps db
def put(id, app, cache):
    comp = cdr(cadddr(car(app)))
    cache.put(appid(id), comp)
    return True

# Get an app from the apps db
def get(id, cache):
    if isNil(id):
        return (("'feed", ("'title", "Composites"), ("'id", "composites")),)
    app = cache.get(appid(id));
    if isNil(app) or app is None:
        return (("'entry", ("'title", car(id)), ("'id", car(id))),)
    return (("'entry", ("'title", car(id)), ("'id", car(id)), ("'content", car(app))),)

# Delete an app from the apps db
def delete(id, cache):
    cache.delete(appid(id))
    return True

