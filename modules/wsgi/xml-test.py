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

# Test XML handling functions

import unittest
from elemutil import *
from xmlutil import *

customerXML = \
    "<?xml version=\'1.0\' encoding=\'UTF-8\'?>\n" \
    "<customer>" \
    "<name>jdoe</name>" \
    "<address><city>san francisco</city><state>ca</state></address>" \
    "<account><id>1234</id><balance>1000</balance></account>" \
    "<account><id>6789</id><balance>2000</balance></account>" \
    "<account><id>4567</id><balance>3000</balance></account>" \
    "</customer>\n"

def testElements():
    ad = (("'city", "san francisco"), ("'state", "ca"))
    ac1 = (("'id", "1234"), ("'balance", 1000))
    ac2 = (("'id", "6789"), ("'balance", 2000))
    ac3 = (("'id", "4567"), ("'balance", 3000))
    c = (("'customer", ("'name", "jdoe"), cons("'address", ad), ("'account", (ac1, ac2, ac3))),)
    e = valuesToElements(c)
    v = elementsToValues(e)
    assert v == c
    s = writeXML(e, True)
    assert car(s) == customerXML

    c2 = (("'customer", ("'name", "jdoe"), cons("'address", ad), cons("'account", ac1), cons("'account", ac2), cons("'account", ac3)),)
    e2 = valuesToElements(c2);
    v2 = elementsToValues(e2);
    s2 = writeXML(e2, True)
    assert car(s2) == customerXML

    c3 = readXML((customerXML,))
    v3 = elementsToValues(c3)
    e3 = valuesToElements(v3)
    s3 = writeXML(e3, True)
    assert car(s3) == customerXML
    return True

def testValues():
    l = (("'ns1:echoString", ("'@xmlns:ns1", "http://ws.apache.org/axis2/services/echo"), ("'text", "Hello World!")),)
    e = valuesToElements(l)
    lx = writeXML(e, True)
    x = readXML(lx)
    v = elementsToValues(x)
    assert v == l
    return True

if __name__ == "__main__":
    print "Testing..."
    testElements()
    testValues()
    print "OK"

