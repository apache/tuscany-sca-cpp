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

# Test ATOM data conversion functions

import unittest
from elemutil import *
from atomutil import *

itemEntry = \
    "<?xml version=\'1.0\' encoding=\'UTF-8\'?>\n" \
    "<entry xmlns=\"http://www.w3.org/2005/Atom\">" \
    "<title type=\"text\">item</title>" \
    "<id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</id>" \
    "<content type=\"application/xml\">" \
    "<item>" \
    "<name>Apple</name><price>$2.99</price>" \
    "</item>" \
    "</content>" \
    "<link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b\" />" \
    "</entry>\n"

incompleteEntry = \
    "<entry xmlns=\"http://www.w3.org/2005/Atom\">" \
    "<title>item</title><content type=\"text/xml\">" \
    "<Item xmlns=\"http://services/\">" \
    "<name xmlns=\"\">Orange</name>" \
    "<price xmlns=\"\">3.55</price>" \
    "</Item>" \
    "</content>" \
    "</entry>"

completedEntry = \
    "<?xml version=\'1.0\' encoding=\'UTF-8\'?>\n" \
    "<entry xmlns=\"http://www.w3.org/2005/Atom\">" \
    "<title type=\"text\">item</title>" \
    "<id />" \
    "<content type=\"application/xml\">" \
    "<Item xmlns=\"http://services/\">" \
    "<name xmlns=\"\">Orange</name>" \
    "<price xmlns=\"\">3.55</price>" \
    "</Item>" \
    "</content><link href=\"\" />" \
    "</entry>\n"

def testEntry():
    i = (element, "'item", (element, "'name", "Apple"), (element, "'price", "$2.99"))
    a = ("item", "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b", i)
    s = writeATOMEntry(a);
    assert car(s) == itemEntry

    a2 = readATOMEntry((itemEntry,))
    s2 = writeATOMEntry(a2)
    assert car(s2) == itemEntry

    a3 = readATOMEntry((incompleteEntry,))
    s3 = writeATOMEntry(a3)
    assert car(s3) == completedEntry
    return True

emptyFeed = \
    "<?xml version=\'1.0\' encoding=\'UTF-8\'?>\n" \
    "<feed xmlns=\"http://www.w3.org/2005/Atom\">" \
    "<title type=\"text\">Feed</title>" \
    "<id>1234</id>" \
    "</feed>\n"

itemFeed = \
    "<?xml version=\'1.0\' encoding=\'UTF-8\'?>\n" \
    "<feed xmlns=\"http://www.w3.org/2005/Atom\">" \
    "<title type=\"text\">Feed</title>" \
    "<id>1234</id>" \
    "<entry xmlns=\"http://www.w3.org/2005/Atom\">" \
    "<title type=\"text\">item</title>" \
    "<id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</id>" \
    "<content type=\"application/xml\">" \
    "<item>" \
    "<name>Apple</name><price>$2.99</price>" \
    "</item>" \
    "</content>" \
    "<link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b\" />" \
    "</entry>" \
    "<entry xmlns=\"http://www.w3.org/2005/Atom\">" \
    "<title type=\"text\">item</title>" \
    "<id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c</id>" \
    "<content type=\"application/xml\">" \
    "<item>" \
    "<name>Orange</name><price>$3.55</price>" \
    "</item>" \
    "</content>" \
    "<link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c\" />" \
    "</entry>" \
    "</feed>\n"

def testFeed():
    s = writeATOMFeed(("Feed", "1234"))
    assert car(s) == emptyFeed

    a2 = readATOMFeed((emptyFeed,))
    s2 = writeATOMFeed(a2)
    assert car(s2) == emptyFeed

    i3 = (("item", "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b",
            (element, "'item", (element, "'name", "Apple"), (element, "'price", "$2.99"))),
          ("item", "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c",
            (element, "'item", (element, "'name", "Orange"), (element, "'price", "$3.55"))))
    a3 = cons("Feed", cons("1234", i3))
    s3 = writeATOMFeed(a3)
    assert car(s3) == itemFeed

    i4 = (("item", "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b",
            valueToElement(("'item", ("'name", "Apple"), ("'price", "$2.99")))),
          ("item", "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c",
            valueToElement(("'item", ("'name", "Orange"), ("'price", "$3.55")))))
    a4 = cons("Feed", cons("1234", i4))
    s4 = writeATOMFeed(a4)
    assert car(s4) == itemFeed

    a5 = readATOMFeed((itemFeed,));
    s5 = writeATOMFeed(a5);
    assert car(s5) == itemFeed

    i6 = (("item", "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b",
            (("'name", "Apple"), ("'price", "$2.99"))),
          ("item", "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c",
            (("'name", "Orange"), ("'price", "$3.55"))))
    a6 = cons("Feed", cons("1234", i6))
    s6 = writeATOMFeed(feedValuesToElements(a6))
    assert car(s6) == itemFeed

    return True

if __name__ == "__main__":
    print "Testing..."
    testEntry()
    testFeed()
    print "OK"

