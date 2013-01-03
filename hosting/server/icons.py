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

# App icons collection implementation
from StringIO import StringIO
try:
    from PIL import Image
except:
    Image = None
from base64 import b64encode, b64decode
from urllib import urlopen
from util import *
from atomutil import *
from sys import debug

# Convert an id to an icon id
def iconid(id):
    return ("apps", car(id), "app.icon")

# Convert image to a 50x50 PNG image
def to50x50png(url):
    debug('icons.py::to50x50png::url', url)
    if Image is None:
        return url
    img = Image.open(StringIO(b64decode(url.split(',')[1])) if url.startswith('data:') else StringIO(urlopen(url).read()))
    t = img.resize((50, 50))
    obuf = StringIO()
    t.save(obuf, 'PNG')
    return 'data:image/png;base64,' + b64encode(obuf.getvalue()).replace('\n', '')

# Put an icon
def put(id, icon, user, cache, apps):
    debug('icons.py::put::id', id)
    debug('icons.py::put::icon', icon)

    # Get the requested app
    app = apps.get(id)
    if isNil(app):
        debug('icons.py::put', 'app not found', id)
        return False

    # Check app author
    if author(app) != user.get(()) and user.get(()) != 'admin':
        debug('icons.py::put', 'different author', author(app))
        return False

    # Get image and token from input icon
    def image(c):
        img = assoc("'image", c)
        return None if isNil(img) else to50x50png(cadr(img))
    def token(c):
        tok = assoc("'token", c)
        return None if isNil(tok) else cadr(tok)
    img = image(content(icon))
    tok = token(content(icon))

    # Update the icon
    # Put with an upload token
    if not isNil(tok):
        debug('icons.py::put::token', tok)

        # Token alone, store token with existing image, if any
        if isNil(img):
            eicon = cache.get(iconid(id))
            eimg = None if isNil(eicon) else image(content(eicon))
            if isNil(eimg):
                iconentry = mkentry(title(app), car(id), author(app), now(), ("'icon", ("'token", tok)))
                debug('icons.py::put::iconentry', iconentry)
                return cache.put(iconid(id), iconentry)

            debug('icons.py::put::eimg', eimg)
            iconentry = mkentry(title(app), car(id), author(app), now(), ("'icon", ("'image", eimg), ("'token", tok)))
            debug('icons.py::put::iconentry', iconentry)
            return cache.put(iconid(id), iconentry)

        # Token plus image, put image if token is valid, removing the token
        debug('icons.py::put::img', img)
        eicon = cache.get(iconid(id))
        etok = None if isNil(eicon) else token(content(eicon))
        debug('icons.py::put::etok', etok)
        if isNil(etok) or tok != etok:
            debug('icons.py::put', 'invalid token', tok)
            return False

        iconentry = mkentry(title(app), car(id), author(app), now(), ("'icon", ("'image", img)))
        debug('icons.py::put::iconentry', iconentry)
        return cache.put(iconid(id), iconentry)

    # Update icon image
    if not isNil(img):
        debug('icons.py::put::img', img)
        iconentry = mkentry(title(app), car(id), author(app), now(), ("'icon", ("'image", img)))
        debug('icons.py::put::iconentry', iconentry)
        return cache.put(iconid(id), iconentry)

    # Put default empty icon
    iconentry = mkentry(title(app), car(id), author(app), now(), ())
    debug('icons.py::put::iconentry', iconentry)
    return cache.put(iconid(id), iconentry)

# Get an icon
def get(id, user, cache, apps):
    debug('icons.py::get::id', id)
    if isNil(id):
        return (("'feed", ("'title", "Icons"), ("'id", "icons")),)

    # Get the requested app
    app = apps.get(id)
    if isNil(app):
        debug('icons.py::get', 'app not found', id)

        # Return a default new icon
        return mkentry(car(id), car(id), user.get(()), now(), ())

    # Get the requested icon
    icon = cache.get(iconid(id))
    if isNil(icon):
        debug('icons.py::get', 'icon not found', id)

        # Return a default new icon
        return mkentry(title(app), car(id), author(app), now(), ())

    # Get image, token, and updated date from icon
    def image(c):
        img = assoc("'image", c)
        return None if isNil(img) else cadr(img)
    def token(c):
        tok = assoc("'token", c)
        return None if isNil(tok) else cadr(tok)
    img = image(content(icon))
    tok = token(content(icon))

    # Return the icon
    iconc = (() if isNil(img) else (("'image", img),)) + (() if isNil(tok) or (user.get(()) != author(app) and user.get(()) != 'admin') else (("'token", tok),))
    if isNil(iconc):
        iconentry = mkentry(title(app), car(id), author(app), updated(icon), ())
        debug('icons.py::get::iconentry', iconentry)
        return iconentry

    iconentry = mkentry(title(app), car(id), author(app), updated(icon), ("'icon",) + iconc)
    debug('icons.py::get::iconentry', iconentry)
    return iconentry

# Delete an icon
def delete(id, user, cache, apps):
    debug('icons.py::delete::id', id)

    # Get the requested app
    app = apps.get(id)
    if isNil(app):
        debug('icons.py::delete', 'app not found', id)
        return False

    # Check app author
    if author(app) != user.get(()):
        debug('icons.py::delete', 'different author', author(app))
        return False

    # Delete the icon
    return cache.delete(iconid(id))

