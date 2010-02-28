#!/usr/bin/python
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

# Composite deployment and integration with WSGI

from wsgiref.simple_server import make_server
from wsgiref.handlers import CGIHandler
from os import environ
from sys import stderr, argv
from util import *
from scdl import *

def requestPath(e):
    return e.get("PATH_INFO", "")

def requestMethod(e):
    return e.get("REQUEST_METHOD", "")

def contentType(ct):
    if ct == None:
        return []
    return [("Content-type", ct)]

def status(st):
    if st == 200:
        return "200 OK"
    if st == 404:
        return "404 Not Found"
    if st == 201:
        return "201 Created"
    return "500 Internal Server Error"

def result(r, st, ct = None, b = ()):
    r(status(st), contentType(ct))
    return b

# WSGI application function
def application(e, r):

    # Read the deployed composite
    comps = components(parse("domain-test.composite"))
    print >> stderr, comps
    
    # Find the requested component
    path = tokens(requestPath(e))
    uc = uriToComponent(path, comps)
    uri = car(uc)
    if uri == None:
        return result(r, 404)
    comp =  cadr(uc)
    mod = __import__(cadr(comp))
    
    # Call the requested component function
    id = path[len(uri):]
    print >> stderr, id
    m = requestMethod(e)
    if (m == "GET"):
        v = mod.get(id)
        print >> stderr, v
        
        # write returned content-type / content pair
        if not isinstance(cadr(v), basestring):
            return result(r, 200, car(v), cadr(v))
        
        # TODO write an ATOM feed or entry
        if nil(id):
            return result(r, 200, "application/atom+xml;type=feed", ("Atom feed"))
        return result(r, 200, "application/atom+xml;type=entry", ("Atom entry"))

    if m == "DELETE":
        v = mod.delete(id)
        if v == False:
            return result(r, 404)
        return result(r, 200)
    
    # TODO implement POST and PUT methods
    return result(r, 500)

def serverType(e):
    return e.get("SERVER_SOFTWARE", "")

# Run the WSGI application
if __name__ == "__main__":
    st = serverType(environ)
    if st == "":
        make_server("", int(argv[1]), application).serve_forever()
    else:
        CGIHandler().run(application)
