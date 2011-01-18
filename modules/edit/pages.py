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
import uuid
import sys
from sys import stderr
from util import *

# Convert an id to an app id
def appid(id):
    return ("'" + car(id), "'app.html")

# Post a new app page to the apps db
def post(collection, app, cache):
    id = appid((str(uuid.uuid1()),))
    xhtml = caddr(app);
    cache.put((id,), xhtml)
    return id

# Put an app page into the apps db
def put(id, app, cache):
    xhtml = caddr(app);
    cache.put(appid(id), xhtml)
    return True

# Get an app page from the apps db
def get(id, cache):
    if isNil(id):
        return ("Pages", "pages")
    return (car(id), car(id), cache.get(appid(id)))

# Delete an app page from the apps db
def delete(id, cache):
    cache.delete(appid(id))
    return True

