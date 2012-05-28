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
from time import strftime
from util import *
from sys import debug

# Convert an id to a composite id
def compid(id):
    return ("apps", car(id), "app.composite")

# Put a composite into the composite db
def put(id, comp, user, cache, apps):
    debug('composites.py::put::id', id)
    debug('composites.py::put::comp', comp)

    # Get the requested app
    app = apps.get(id);
    if isNil(app) or app is None:
        debug('composites.py::put', 'app not found', id)
        return False

    # Check app author
    author = cadr(assoc("'author", car(app)))
    if author != user.get(()):
        debug('composites.py::put', 'different author', author)
        return False

    # Update the composite in the composite db
    compentry = (("'entry", assoc("'title", car(app)), ("'id", car(id)), ("'author", user.get(())), ("'updated", strftime('%b %d, %Y')), assoc("'content", car(comp))),)
    debug('composites.py::put::compentry', compentry)
    return cache.put(compid(id), compentry)

# Get a composite from the composite db
def get(id, user, cache, apps):
    debug('composites.py::get::id', id)
    if isNil(id):
        return (("'feed", ("'title", "Composites"), ("'id", "composites")),)

    # Get the requested app
    app = apps.get(id)
    if isNil(app) or app is None:
        debug('composites.py::get', 'app not found', id)

        # Return a default new composite
        return (("'entry", ("'title", car(id)), ("'id", car(id)), ("'author", user.get(())), ("'updated", strftime('%b %d, %Y'))),)

    # Get the requested composite
    comp = cache.get(compid(id));
    if isNil(comp) or comp is None:
        debug('composites.py::get', 'composite not found', id)

        # Return a default new composite
        return (("'entry", ("'title", car(id)), ("'id", car(id)), assoc("'author", car(app)), assoc("'updated", car(app))),)

    # Return the composite
    def updated(u):
        return assoc("'updated", car(app)) if isNil(u) or u is None else u

    compentry = (("'entry", assoc("'title", car(app)), ("'id", car(id)), assoc("'author", car(app)), updated(assoc("'updated", car(comp))), assoc("'content", car(comp))),)
    debug('composites.py::get::compentry', compentry)
    return compentry

# Delete a composite from the composite db
def delete(id, user, cache, apps):
    debug('composites.py::delete::id', id)

    # Get the requested app
    app = apps.get(id);
    if isNil(app) or app is None:
        debug('composites.py::delete', 'app not found', id)
        return False

    # Check app author
    author = cadr(assoc("'author", car(app)))
    if author != user.get(()):
        debug('composites.py::delete', 'different author', author)
        return False

    # Delete the composite
    return cache.delete(compid(id))

