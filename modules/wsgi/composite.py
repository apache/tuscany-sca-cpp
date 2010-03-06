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
from wsgiref.util import request_uri
from os import environ
from sys import stderr, argv
from util import *
from scdl import *
from atomutil import *
from jsonutil import *

# Return the path of an HTTP request
def requestPath(e):
    return e.get("PATH_INFO", "")

# Return the method of an HTTP request
def requestMethod(e):
    return e.get("REQUEST_METHOD", "")

# Return the method of an HTTP request
def requestContentType(e):
    return e.get("CONTENT_TYPE", "")

# Return the request body input stream
def requestBody(e):
    i = e.get("wsgi.input", None)
    if i == None:
        return ()
    l = int(e.get("CONTENT_LENGTH", "0"))
    return (i.read(l),)

# Return an HTTP status
def errstatus(st):
    return (st, "<html><head><title>"+ st + "</title></head><body><h1>" + st[4:] + "</h1></body></html>")

def status(st, b):
    if st == 200:
        return cons("200 OK", b)
    if st == 404:
        return errstatus("404 Not Found")
    if st == 201:
        return cons("201 Created", b)
    return errstatus("500 Internal Server Error")

# Return an HTTP result
def result(r, st, h = (), b = ()):
    s = status(st, b)
    r(car(s), list(h))
    return cdr(s)


# Converts the args received in a POST to a list of key value pairs
def postArgs(a):
    if isNil(a):
        return ((),)
    l = car(a);
    return cons(l, postArgs(cdr(a)))

# WSGI application function
def application(e, r):

    # Read the deployed composite
    compos = components(parse("domain-test.composite"))
    #print >> stderr, compos

    # Evaluate the deployed components
    comps = evalComponents(compos)
    
    # Get the request path and method
    path = tokens(requestPath(e))
    m = requestMethod(e)
    if (isNil(path) or path == ("index.html",)) and m == "GET":
        return result(r, 200, (("Content-type", "text/html"),), ("<html><body><h1>It works!</h1></body></html>",))

    # Find the requested component
    uc = uriToComponent(path, comps)
    uri = car(uc)
    if uri == None:
        return result(r, 404)
    comp = cadr(uc)

    # Call the requested component function
    id = path[len(uri):]
    if (m == "GET"):
        v = comp("get", id)
        
        # Write returned content-type / content pair
        if not isinstance(cadr(v), basestring):
            return result(r, 200, (("Content-type", car(v)),), cadr(v))
        
        # Write an ATOM feed or entry
        if isNil(id):
            return result(r, 200, (("Content-type", "application/atom+xml;type=feed"),), writeATOMFeed(feedValuesToElements(v)))
        return result(r, 200, (("Content-type", "application/atom+xml;type=entry"),), writeATOMEntry(entryValuesToElements(v)))

    if m == "POST":
        ct = requestContentType(e)

        # Handle a JSON-RPC function call
        if ct.find("application/json-rpc") != -1 or ct.find("text/plain") != -1:
            json = elementsToValues(readJSON(requestBody(e)))
            args = postArgs(json)
            jid = cadr(assoc("'id", args))
            func = funcName(cadr(assoc("'method", args)))
            params = cadr(assoc("'params", args))
            v = comp(func, *params)
            return result(r, 200, (("Content-type", "application/json-rpc"),), jsonResult(jid, v))

        # Handle an ATOM entry POST
        if ct.find("application/atom+xml") != -1:
            ae = entryValue(readATOMEntry(requestBody(e)))
            v = comp("post", id, ae)
            if isNil(v):
                return result(r, 500)
            return result(r, 201, (("Location", request_uri(e) + "/" + "/".join(v)),))
        return result(r, 500)
    
    if m == "PUT":
        # Handle an ATOM entry PUT
        ae = entryValue(readATOMEntry(requestBody(e)))
        v = comp("put", id, ae)
        if v == False:
            return result(r, 404)
        return result(r, 200)
    
    if m == "DELETE":
        v = comp("delete", id)
        if v == False:
            return result(r, 404)
        return result(r, 200)
    
    # TODO implement POST and PUT methods
    return result(r, 500)

# Return the WSGI server type
def serverType(e):
    return e.get("SERVER_SOFTWARE", "")

# Run the WSGI application
if __name__ == "__main__":
    st = serverType(environ)
    if st == "":
        make_server("", int(argv[1]), application).serve_forever()
    elif st == "Development/1.0":
        from google.appengine.ext.webapp.util import run_wsgi_app
        run_wsgi_app(application)
    else:
        CGIHandler().run(application)

