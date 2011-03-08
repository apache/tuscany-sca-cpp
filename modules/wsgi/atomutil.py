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

# ATOM data conversion functions

from util import *
from elemutil import *
from xmlutil import *

# Convert a list of elements to a list of values representing an ATOM entry
def entryElementValues(e):
    lt = filter(selector((element, "'title")), e)
    t = "" if isNil(lt) else elementValue(car(lt))
    li = filter(selector((element, "'id")), e)
    i = "" if isNil(li) else elementValue(car(li))
    lc = filter(selector((element, "'content")), e)
    return append((element, "'entry", (element, "'title", t), (element, "'id", i)),
            () if isNil(lc) else ((element, "'content", elementValue(car(lc))),))

# Convert a list of elements to a list of values representing ATOM entries
def entriesElementValues(e):
    if isNil(e):
        return e
    return cons(entryElementValues(car(e)), entriesElementValues(cdr(e)))

# Convert a list of strings to a list of values representing an ATOM entry
def readATOMEntry(l):
    e = readXML(l)
    if isNil(e):
        return ()
    return (entryElementValues(car(e)),)

# Return true if a list of strings represents an ATOM feed
def isATOMFeed(l):
    if not isXML(l):
        return False
    return contains(car(l), "<feed") and contains(car(l), "=\"http://www.w3.org/2005/Atom\"")

# Return true if a list of strings represents an ATOM entry
def isATOMEntry(l):
    if not isXML(l):
        return False
    return contains(car(l), "<entry") and not contains(car(l), "<feed") and contains(car(l), "=\"http://www.w3.org/2005/Atom\"")

# Convert a list of strings to a list of values representing an ATOM feed
def readATOMFeed(l):
    f = readXML(l)
    if isNil(f):
        return ()
    t = filter(selector((element, "'title")), car(f))
    i = filter(selector((element, "'id")), car(f))
    e = filter(selector((element, "'entry")), car(f))
    return (append(
                (element, "'feed", (element, "'title", elementValue(car(t))), (element, "'id", elementValue(car(i)))),
                entriesElementValues(e)),)

# Convert a list of values representy an ATOM entry to a list of elements
def entryElement(l):
    title = elementValue(namedElementChild("'title", l))
    id = elementValue(namedElementChild("'id", l))
    content = namedElementChild("'content", l)
    text = False if isNil(content) else elementHasValue(content)
    return append(append(
            (element, "'entry", (attribute, "'xmlns", "http://www.w3.org/2005/Atom"),
             (element, "'title", (attribute, "'type", "text"), title),
             (element, "'id", id)),
            () if isNil(content) else (append(
                (element, "'content", (attribute, "'type", "text" if text else "application/xml")), (elementValue(content),) if text else elementChildren(content)),)),
            ((element, "'link", (attribute, "'href", id)),))

# Convert a list of values representing ATOM entries to a list of elements
def entriesElements(l):
    if isNil(l):
        return l
    return cons(entryElement(car(l)), entriesElements(cdr(l)))

# Convert a list of values representing an ATOM entry to an ATOM entry
def writeATOMEntry(ll):
    l = ll if isNil(ll) else car(ll)
    return writeXML((entryElement(l),), True)

# Convert a list of values representing an ATOM feed to an ATOM feed
def writeATOMFeed(ll):
    l = ll if isNil(ll) else car(ll)
    lt = filter(selector((element, "'title")), l)
    t = '' if isNil(lt) else elementValue(car(lt))
    li = filter(selector((element, "'id")), l)
    i = '' if isNil(li) else elementValue(car(li))
    f = (element, "'feed", (attribute, "'xmlns", "http://www.w3.org/2005/Atom"),
            (element, "'title", (attribute, "'type", "text"), t),
            (element, "'id", i))

    # Write ATOM entries
    le = filter(selector((element, "'entry")), l)
    if isNil(le):
        return writeXML((f,), True)

    # Write a single ATOM entry element with a list of values
    if not isNil(le) and not isNil(car(le)) and isList(car(caddr(car(le)))):
        fe = append(f, entriesElements(caddr(car(le))))
        return writeXML((fe,), True)

    # Write separate ATOM entry elements
    fe = append(f, entriesElements(le))
    return writeXML((fe,), True)

