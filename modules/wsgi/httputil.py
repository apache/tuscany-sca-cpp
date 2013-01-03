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

# HTTP client proxy functions

from httplib import HTTPConnection, HTTPSConnection
from urlparse import urlparse
from StringIO import StringIO
import os.path
from string import strip
from base64 import b64encode
from sys import stderr
from util import *
from elemutil import *
import atomutil
import jsonutil
import rssutil
import xmlutil

# JSON request id
id = 1

# Make a callable HTTP client
class client:
    def __init__(self, url):
        self.url = urlparse(url)

    def __call__(self, func, *args):
        print >> stderr, "Client proxy call", func, args

        # Connect to the configured URL
        c, headers = connect(self.url)

        # handle a GET request
        if func == "get":
            u = requesturi(self.url, car(args))
            print >> stderr, "Client GET request", u
            c.request("GET", u, None, headers)
            res = c.getresponse()
            print >> stderr, "Client status", res.status
            if res.status != 200:
                return None
            ct = res.getheader("Content-type", "text/plain")
            ls = (res.read(),)

            if atomutil.isATOMEntry(ls):
                # Read an ATOM entry
                v = atomutil.entryValue(atomutil.readATOMEntry(ls))
                print >> stderr, "Client result", v
                return v

            if contains(ct, "application/atom+xml") or atomutil.isATOMFeed(ls):
                # Read an ATOM feed
                v = atomutil.feedValues(atomutil.readATOMFeed(ls))
                print >> stderr, "Client result", v
                return v

            if contains(ct, "application/rss+xml") or rssutil.isRSSFeed(ls):
                # Read an RSS feed
                v = rssutil.feedValues(rssutil.readRSSFeed(ls))
                print >> stderr, "Client result", v
                return v

            if contains(ct, "text/javascript") or contains(ct, "application/json") or jsonutil.isJSON(ls):
                # Read a JSON document
                v = elementsToValues(jsonutil.readJSON(ls))
                print >> stderr, "Client result", v
                return v

            if contains(ct, "text/xml") or contains(ct, "application/xml") or xmlutil.isXML(ls):
                # Read an XML document
                v = elementsToValues(xmlutil.readXML(ls))
                print >> stderr, "Client result", v
                return v

            # Return the content type and a content list
            v = (ct, ls)
            print >> stderr, "Client result", v
            return v

        # handle a POST request
        if func == "post":
            u = requesturi(self.url, car(args))
            print >> stderr, "Client POST request", u
            req = StringIO()
            writeStrings(atomutil.writeATOMEntry(atomutil.entryValuesToElements(cadr(args))), req)
            headers["Content-type"] = "application/atom+xml"
            c.request("POST", u, req.getvalue(), headers)
            res = c.getresponse()
            print >> stderr, "Client status", res.status
            if res.status != 200 and res.status != 201:
                return None
            loc = res.getheader("Location")
            if loc == None:
                return None
            return loc[(loc.rfind('/') + 1):]

        # handle a PUT request
        if func == "put":
            u = requesturi(self.url, car(args))
            print >> stderr, "Client PUT request", u
            req = StringIO()
            writeStrings(atomutil.writeATOMEntry(atomutil.entryValuesToElements(cadr(args))), req)
            headers["Content-type"] = "application/atom+xml"
            c.request("PUT", u, req.getvalue(), headers)
            res = c.getresponse()
            print >> stderr, "Client status", res.status
            if res.status != 200:
                return None
            return True

        # handle a PATCH request
        if func == "patch":
            u = requesturi(self.url, car(args))
            print >> stderr, "Client PATCH request", u
            req = StringIO()
            writeStrings(atomutil.writeATOMEntry(atomutil.entryValuesToElements(cadr(args))), req)
            headers["Content-type"] = "application/atom+xml"
            c.request("PATCH", u, req.getvalue(), headers)
            res = c.getresponse()
            print >> stderr, "Client status", res.status
            if res.status != 200:
                return None
            return True

        # handle a DELETE request
        if func == "delete":
            u = requesturi(self.url, car(args))
            print >> stderr, "Client DELETE request", u
            c.request("DELETE", u, None, headers)
            res = c.getresponse()
            print >> stderr, "Client status", res.status
            if res.status != 200:
                return None
            return True

        # handle a JSON-RPC request
        u = requesturi(self.url, ())
        print >> stderr, "Client JSON-RPC request", u
        global id
        req = StringIO()
        writeStrings(jsonutil.jsonRequest(id, func, args), req)
        id = id + 1
        headers["Content-type"] = "application/json-rpc"
        c.request("POST", u, req.getvalue(), headers)
        res = c.getresponse()
        print >> stderr, "Client status", res.status
        if res.status != 200:
            return None
        v = jsonutil.jsonResultValue((res.read(),))
        print >> stderr, "Client result", v
        return v

    def __getattr__(self, name):
        if name[0] == '_':
            raise AttributeError()
        if name == "eval":
            return self
        l = lambda *args: self.__call__(name, *args)
        self.__dict__[name] = l
        return l

    def __repr__(self):
        return repr((self.url,))

def mkclient(url):
    return client(url)

# Connect to a URL, return a connection and any authorization headers
def connect(url):
    if url.scheme == "https":

        # With HTTPS, use a cerficate or HTTP basic authentication
        if os.path.exists("server.key"):
            c = HTTPSConnection(url.hostname, 443 if url.port == None else url.port, "server.key", "server.crt")
            return c, {}
        else:
            c = HTTPSConnection(url.hostname, 443 if url.port == None else url.port)

            # For HTTP basic authentication the user and password may be
            # provided by htpasswd.py
            try:
                import htpasswd
                auth =  'Basic ' + b64encode(htpasswd.user + ':' + htpasswd.passwd)
                return c, {"Authorization": auth}
            except:
                return c, {}
    else:
        c = HTTPConnection(url.hostname, 80 if url.port == None else url.port)
        return c, {}

# Convert a URL and arg to a request URI
def requesturi(url, arg):
    return url.path + path(arg) + ("" if url.query == "" else "?" + url.query)

