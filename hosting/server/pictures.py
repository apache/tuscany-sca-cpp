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

# Pictures collection implementation
from urllib import urlopen
from util import *
from atomutil import *
from imgutil import *
from sys import debug

# Convert a particular user id to a picture id
def pictureid(id):
    return ('accounts', id, 'user.picture')

# Update the user's picture
def put(id, picture, user, cache):
    debug('pictures.py::put::id', id)
    debug('pictures.py::put::picture', picture)

    picid = user.get(()) if isNull(id) else car(id)

    # Only the admin can update other user's pictures
    if picid != user.get(()) and user.get(()) != 'admin':
        debug('pictures.py::put', 'not owner or admin', user.get(()))
        return False

    # Get image and token from input picture
    def image(c):
        img = assoc("'image", c)
        return None if isNull(img) else urlto50x50jpeg(cadr(img))
    def token(c):
        tok = assoc("'token", c)
        return None if isNull(tok) else cadr(tok)
    img = image(content(picture))
    tok = token(content(picture))

    # Update the picture
    # Put with an upload token
    if not isNull(tok):
        debug('pictures.py::put::token', tok)

        # Token alone, store token with existing image, if any
        if isNull(img):
            epicture = cache.get(pictureid(picid))
            eimg = None if isNull(epicture) else image(content(epicture))
            if isNull(eimg):
                picentry = mkentry(title(picture), picid, picid, now(), ("'picture", ("'token", tok)))
                debug('pictures.py::put::picentry', picentry)
                return cache.put(pictureid(picid), picentry)

            debug('pictures.py::put::eimg', eimg)
            picentry = mkentry(title(picture), picid, picid, now(), ("'picture", ("'image", eimg), ("'token", tok)))
            debug('pictures.py::put::picentry', picentry)
            return cache.put(pictureid(picid), picentry)

        # Token plus image, put image if token is valid, removing the token
        debug('pictures.py::put::img', img)
        epicture = cache.get(pictureid(picid))
        etok = None if isNull(epicture) else token(content(epicture))
        debug('pictures.py::put::etok', etok)
        if isNull(etok) or tok != etok:
            debug('pictures.py::put', 'invalid token', tok)
            return False

        picentry = mkentry(title(picture), picid, picid, now(), ("'picture", ("'image", img)))
        debug('pictures.py::put::picentry', picentry)
        return cache.put(pictureid(picid), picentry)

    # Update picture image
    if not isNull(img):
        debug('pictures.py::put::img', img)
        picentry = mkentry(title(picture), picid, picid, now(), ("'picture", ("'image", img)))
        debug('pictures.py::put::picentry', picentry)
        return cache.put(pictureid(picid), picentry)

    # Put default empty picture
    picentry = mkentry(title(picture), picid, picid, now(), ())
    debug('pictures.py::put::picentry', picentry)
    return cache.put(pictureid(picid), picentry)

# Get a user's picture
def get(id, user, cache):
    debug('pictures.py::get::id', id)

    # Get the requested picture
    picid = user.get(()) if isNull(id) else car(id)
    picture = cache.get(pictureid(picid))
    if isNull(picture):
        return mkentry(picid, picid, picid, now(), ())

    # Get image and token from picture
    def image(c):
        img = assoc("'image", c)
        return None if isNull(img) else cadr(img)
    def token(c):
        tok = assoc("'token", c)
        return None if isNull(tok) else cadr(tok)
    img = image(content(picture))
    tok = token(content(picture))

    # Return the picture
    picc = (() if isNull(img) else (("'image", img),)) + (() if isNull(tok) or (user.get(()) != author(picture) and user.get(()) != 'admin') else (("'token", tok),))
    if isNull(picc):
        picentry = mkentry(title(picture), picid, author(picture), updated(picture), ())
        debug('pictures.py::get::picentry', picentry)
        return picentry

    picentry = mkentry(title(picture), picid, author(picture), updated(picture), ("'picture",) + picc)
    debug('pictures.py::get::picentry', picentry)
    return picentry

