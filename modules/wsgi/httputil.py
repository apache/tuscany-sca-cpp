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
from atomutil import *
from jsonutil import *

# JSON request id
id = 1

# Make a callable HTTP client
class client:
    def __init__(self, url):
        self.url = urlparse(url)

    def __call__(self, func, *args):

        # Connect to the configured URL
        print >> stderr, "Client POST", self.url.geturl()
        c, headers = connect(self.url)

        # POST a JSON-RPC request
        global id
        req = StringIO()
        writeStrings(jsonRequest(id, func, args), req)
        id = id + 1
        headers["Content-type"] = "application/json-rpc"
        c.request("POST", self.url.path, req.getvalue(), headers)
        res = c.getresponse()
        print >> stderr, "Client status", res.status
        if res.status != 200:
            return None
        return jsonResultValue((res.read(),))

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

            # For HTTP basic authentication the user and password are
            # provided by htpasswd.py
            import htpasswd
            auth =  'Basic ' + b64encode(htpasswd.user + ':' + htpasswd.passwd)
            return c, {"Authorization": auth}
    else:
        c = HTTPConnection(url.hostname, 80 if url.port == None else url.port)
        return c, {}

