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
import hashlib
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

def requestIfNoneMatch(e):
    return e.get("HTTP_IF_NONE_MATCH", "");

# Hash a list of strings into an MD5 signature
def md5update(md, s):
    if isNil(s):
        return md.hexdigest()
    md.update(car(s))
    return md5update(md, cdr(s))

def md5(s):
    return md5update(hashlib.md5(), s)

# Return an HTTP success result
def result(e, r, st, h = (), b = None):
    if st == 201:
        r("201 Created", list(h))
        return ()

    if st == 200:
        if b == None:
            r("200 OK", list(h))
            return ()

        # Handle etags to minimize bandwidth usage
        md = md5(b)
        if (md == requestIfNoneMatch(e)):
            r("304 Not Modified", list((("Etag", md), ("Expires", "Tue, 01 Jan 1980 00:00:00 GMT"))))
            return ()
        r("200 OK", list(h + (("Etag", md), ("Expires", "Tue, 01 Jan 1980 00:00:00 GMT"))))
        return b

    if st == 301:
        r("301 Moved Permanently", list(h))

    return failure(e, r, 500)

# Return an HTTP failure result
def failure(e, r, st):
    s = "404 Not Found" if st == 404 else str(st) + " " + "Internal Server Error"
    r(s, list((("Content-type", "text/html"),)))
    return ("<html><head><title>"+ s + "</title></head><body><h1>" + s[4:] + "</h1></body></html>",)

# Return a static file
def fileresult(e, r, ct, f):

    # Read the file, return a 404 if not found
    p = "htdocs" + f
    if not os.path.exists(p):
        return failure(e, r, 404)
    c = tuple(FileWrapper(open("htdocs" + f)))

    # Handle etags to minimize bandwidth usage
    md = md5(c)
    r("200 OK", list((("Content-type", ct),("Etag", md))))
    return c

# Converts the args received in a POST to a list of key value pairs
def postArgs(a):
    if isNil(a):
        return ((),)
    l = car(a);
    return cons(l, postArgs(cdr(a)))

# Return the URL used to sign out
def signout(ruri):
    try:
        from google.appengine.api import users
        return users.create_logout_url(ruri)
    except:
        return None

# Return the URL used to sign in
def signin(ruri):
    try:
        from google.appengine.api import users
        return users.create_login_url(ruri)
    except:
        return None

# WSGI application function
def application(e, r):
    m = requestMethod(e)
    fpath = requestPath(e)

    # Serve static files
    if m == "GET":
        if fpath.endswith(".html"):
            return fileresult(e, r, "text/html", fpath)
        if fpath.endswith(".js"):
            return fileresult(e, r, "application/x-javascript", fpath)
        if fpath.endswith(".png"):
            return fileresult(e, r, "image/png", fpath)
        if fpath == "/":
            return result(e, r, 301, (("Location", "/index.html"),))

        # Debug hook
        if fpath == "/debug":
            return result(e, r, 200, (("Content-type", "text/plain"),), ("Debug",))

        # Sign in and out
        if fpath == "/login":
            redir = signin("/")
            if redir:
                return result(e, r, 301, (("Location", redir),))
        if fpath == "/logout":
            redir = signout(signin("/"))
            if redir:
                return result(e, r, 301, (("Location", redir),))

    # Find the requested component
    path = tokens(fpath)
    uc = uriToComponent(path, comps)
    uri = car(uc)
    if uri == None:
        return failure(e, r, 404)
    comp = cadr(uc)

    # Call the requested component function
    id = path[len(uri):]
    if m == "GET":
        v = comp("get", id)
        
        # Write returned content-type / content pair
        if not isinstance(cadr(v), basestring):
            return result(e, r, 200, (("Content-type", car(v)),), cadr(v))
        
        # Write an ATOM feed or entry
        if isNil(id):
            return result(e, r, 200, (("Content-type", "application/atom+xml;type=feed"),), writeATOMFeed(feedValuesToElements(v)))
        return result(e, r, 200, (("Content-type", "application/atom+xml;type=entry"),), writeATOMEntry(entryValuesToElements(v)))

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
            return result(e, r, 200, (("Content-type", "application/json-rpc"),), jsonResult(jid, v))

        # Handle an ATOM entry POST
        if ct.find("application/atom+xml") != -1:
            ae = entryValue(readATOMEntry(requestBody(e)))
            v = comp("post", id, ae)
            if isNil(v):
                return failure(e, r, 500)
            return result(e, r, 201, (("Location", request_uri(e) + "/" + "/".join(v)),))
        return failure(e, r, 500)
    
    if m == "PUT":
        # Handle an ATOM entry PUT
        ae = entryValue(readATOMEntry(requestBody(e)))
        v = comp("put", id, ae)
        if v == False:
            return failure(e, r, 404)
        return result(e, r, 200)
    
    if m == "DELETE":
        v = comp("delete", id)
        if v == False:
            return failure(e, r, 404)
        return result(e, r, 200)
    
    return failure(e, r, 500)

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

