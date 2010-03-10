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
from wsgiref.util import FileWrapper
from os import environ
import os.path
from sys import stderr, argv
from util import *
from scdl import *
from atomutil import *
from jsonutil import *

# Cache the deployed components between requests
comps = None

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

# Send a file
def fileresult(f):
    return tuple(FileWrapper(open("htdocs" + f)))

# Converts the args received in a POST to a list of key value pairs
def postArgs(a):
    if isNil(a):
        return ((),)
    l = car(a);
    return cons(l, postArgs(cdr(a)))

# WSGI application function
def application(e, r):
    m = requestMethod(e)
    fpath = requestPath(e)

    # Debug hook
    if fpath == "/debug":
        return result(r, 200, (("Content-type", "text/plain"),), ("Debug",))

    # Serve static files
    if m == "GET":
        if fpath.endswith(".html"):
            return result(r, 200, (("Content-type", "text/html"),), fileresult(fpath))
        if fpath.endswith(".js"):
            return result(r, 200, (("Content-type", "application/x-javascript"),), fileresult(fpath))
        if fpath.endswith(".png"):
            return result(r, 200, (("Content-type", "image/png"),), fileresult(fpath))

    # Find the requested component
    path = tokens(fpath)
    uc = uriToComponent(path, comps)
    uri = car(uc)
    if uri == None:
        return result(r, 404)
    comp = cadr(uc)

    # Call the requested component function
    id = path[len(uri):]
    if m == "GET":
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
    
    return result(r, 500)

# Return the WSGI server type
def serverType(e):
    return e.get("SERVER_SOFTWARE", "")

def main():
    # Read the deployed composite and evaluate the configured components
    global comps
    if comps == None:
        domain = "domain.composite" if os.path.exists("domain.composite") else "domain-test.composite"
        comps = evalComponents(components(parse(domain)))

    # Handle the WSGI request with the WSGI runtime
    st = serverType(environ)
    if st.find("App Engine") != -1 or st.find("Development") != -1:
        from google.appengine.ext.webapp.util import run_wsgi_app
        run_wsgi_app(application)
    elif st != "":
        CGIHandler().run(application)
    else:
        make_server("", int(argv[1]), application).serve_forever()

# Run the WSGI application
if __name__ == "__main__":
    main()

