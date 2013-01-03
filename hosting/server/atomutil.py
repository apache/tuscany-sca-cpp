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

# Simple ATOM handling functions
from util import *
from time import strftime, gmtime

# Make an ATOM entry
def mkentry(title, id, author, updated, content):
    return (("'entry", ("'title", title), ("'id", id), ("'author", author), ("'updated", updated), ("'content",) if isNull(content) else ("'content", content)),)

# Make an ATOM feed
def mkfeed(title, id, author, updated, entries):
    return (("'entry", ("'title", title), ("'id", id), ("'author", author), ("'updated", updated), ("'content",) if isNull(content) else ("'content", content)),)

# Return ATOM attributes
def title(e):
    if isNull(e):
        return ()
    t = assoc("'title", car(e))
    return None if isNull(t) else cadr(t)

def entryid(e):
    if isNull(e):
        return ()
    id = assoc("'id", car(e))
    return None if isNull(id) else cadr(id)

def author(e):
    if isNull(e):
        return ()
    a = assoc("'author", car(e))
    return None if isNull(a) else cadr(a)

def updated(e):
    if isNull(e):
        return ()
    u = assoc("'updated", car(e))
    return None if isNull(u) else cadr(u)

def content(e):
    if isNull(e):
        return ()
    c = assoc("'content", car(e))
    return () if isNull(c) or isNull(cdr(c)) else c[len(c) - 1]

# Return the current time
def now():
    return strftime('%Y-%m-%dT%H:%M:%S+00:00', gmtime())

# Return an (updated now) assoc
def updatedNow():
    return ("'updated", strftime('%Y-%m-%dT%H:%M:%S+00:00', gmtime()))

