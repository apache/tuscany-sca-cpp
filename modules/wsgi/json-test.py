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

# Test JSON data conversion functions

import unittest
from elemutil import *
from jsonutil import *

def testJSON():
    ad = ((attribute, "'city", "san francisco"), (attribute, "'state", "ca"))
    ac = ((element, "'id", "1234"), (attribute, "'balance", 1000))
    cr = ((attribute, "'name", "jdoe"), cons(element, cons("'address", ad)), cons(element, cons("'account", ac)))
    c = (cons(element, cons("'customer", cr)),)
    s = writeJSON(c);
    assert car(s) == "{\"customer\":{\"account\":{\"@balance\":1000,\"id\":\"1234\"},\"@name\":\"jdoe\",\"address\":{\"@city\":\"san francisco\",\"@state\":\"ca\"}}}"

    phones = ("408-1234", "650-1234")
    l = ((element, "'phones", phones), (element, "'lastName", "test\ttab"), (attribute, "'firstName", "test1"))
    s2 = writeJSON(l);
    assert car(s2) == "{\"phones\":[\"408-1234\",\"650-1234\"],\"@firstName\":\"test1\",\"lastName\":\"test\\ttab\"}"

    r = readJSON(s2)
    assert r == ((element, "'lastName", "test\ttab"), (attribute, "'firstName", "test1"), (element, "'phones", phones))
    w = writeJSON(r)
    assert car(w) == "{\"lastName\":\"test\\ttab\",\"@firstName\":\"test1\",\"phones\":[\"408-1234\",\"650-1234\"]}"

    l4 = (("'ns1:echoString", ("'@xmlns:ns1", "http://ws.apache.org/axis2/services/echo"), ("'text", "Hello World!")),)
    s4 = writeJSON(valuesToElements(l4))
    assert car(s4) == "{\"ns1:echoString\":{\"@xmlns:ns1\":\"http://ws.apache.org/axis2/services/echo\",\"text\":\"Hello World!\"}}"

    r4 = elementsToValues(readJSON(s4))
    assert r4 == (("'ns1:echoString", ("'text", 'Hello World!'), ("'@xmlns:ns1", 'http://ws.apache.org/axis2/services/echo')),)
    return True

def testJSONRPC():
    return True

if __name__ == "__main__":
    print "Testing..."
    testJSON()
    testJSONRPC()
    print "OK"

