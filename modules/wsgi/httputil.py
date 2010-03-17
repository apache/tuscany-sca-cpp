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

from httplib import HTTPConnection
from urlparse import urlparse
from StringIO import StringIO
from util import *
from atomutil import *
from jsonutil import *

id = 1

# Make a callable HTTP client
class client:
    def __init__(self, uri):
        self.uri = urlparse(uri)

    def __call__(self, func, *args):
        global id
        req = StringIO()
        writeStrings(jsonRequest(id, func, args), req)
        id = id + 1
        c = HTTPConnection(self.uri.hostname, 80 if self.uri.port == None else self.uri.port)
        c.request("POST", self.uri.path, req.getvalue(), {"Content-type": "application/json-rpc"})
        res = c.getresponse()
        if res.status != 200:
            return None
        return jsonResultValue((res.read(),))

    def __repr__(self):
        return repr((self.uri,))

def mkclient(uri):
    return client(uri)

