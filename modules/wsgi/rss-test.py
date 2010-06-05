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

# Test RSS data conversion functions

import unittest
from elemutil import *
from rssutil import *

itemEntry = \
    "<?xml version=\'1.0\' encoding=\'UTF-8\'?>\n" \
    "<item>" \
    "<title>fruit</title>" \
    "<link>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</link>" \
    "<description>" \
    "<item>" \
    "<name>Apple</name><price>$2.99</price>" \
    "</item>" \
    "</description>" \
    "</item>\n"

textEntry = \
    "<?xml version=\'1.0\' encoding=\'UTF-8\'?>\n" \
    "<item>" \
    "<title>fruit</title>" \
    "<link>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</link>" \
    "<description>" \
    "Apple" \
    "</description>" \
    "</item>\n"

incompleteEntry = \
    "<item>" \
    "<title>fruit</title><description>" \
    "<fruit xmlns=\"http://services/\">" \
    "<name xmlns=\"\">Orange</name>" \
    "<price xmlns=\"\">3.55</price>" \
    "</fruit>" \
    "</description>" \
    "</item>"

completedEntry = \
    "<?xml version=\'1.0\' encoding=\'UTF-8\'?>\n" \
    "<item>" \
    "<title>fruit</title>" \
    "<link />" \
    "<description>" \
    "<fruit xmlns=\"http://services/\">" \
    "<name xmlns=\"\">Orange</name>" \
    "<price xmlns=\"\">3.55</price>" \
    "</fruit>" \
    "</description>" \
    "</item>\n"

def testEntry():
    i = (element, "'item", (element, "'name", "Apple"), (element, "'price", "$2.99"))
    a = ("fruit", "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b", i)
    s = writeRSSEntry(a);
    assert car(s) == itemEntry

    a2 = readRSSEntry((itemEntry,))
    s2 = writeRSSEntry(a2)
    assert car(s2) == itemEntry

    a3 = readRSSEntry((textEntry,))
    s3 = writeRSSEntry(a3)
    assert car(s3) == textEntry

    a4 = readRSSEntry((incompleteEntry,))
    s4 = writeRSSEntry(a4)
    assert car(s4) == completedEntry
    return True

emptyFeed = \
    "<?xml version=\'1.0\' encoding=\'UTF-8\'?>\n" \
    "<rss version=\"2.0\">" \
    "<channel>" \
    "<title>Feed</title>" \
    "<link>1234</link>" \
    "<description>Feed</description>" \
    "</channel>" \
    "</rss>\n"

itemFeed = \
    "<?xml version=\'1.0\' encoding=\'UTF-8\'?>\n" \
    "<rss version=\"2.0\">" \
    "<channel>" \
    "<title>Feed</title>" \
    "<link>1234</link>" \
    "<description>Feed</description>" \
    "<item>" \
    "<title>fruit</title>" \
    "<link>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</link>" \
    "<description>" \
    "<item>" \
    "<name>Apple</name><price>$2.99</price>" \
    "</item>" \
    "</description>" \
    "</item>" \
    "<item>" \
    "<title>fruit</title>" \
    "<link>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c</link>" \
    "<description>" \
    "<item>" \
    "<name>Orange</name><price>$3.55</price>" \
    "</item>" \
    "</description>" \
    "</item>" \
    "</channel>" \
    "</rss>\n"

def testFeed():
    s = writeRSSFeed(("Feed", "1234"))
    assert car(s) == emptyFeed

    a2 = readRSSFeed((emptyFeed,))
    s2 = writeRSSFeed(a2)
    assert car(s2) == emptyFeed

    i3 = (("fruit", "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b",
            (element, "'item", (element, "'name", "Apple"), (element, "'price", "$2.99"))),
          ("fruit", "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c",
            (element, "'item", (element, "'name", "Orange"), (element, "'price", "$3.55"))))
    a3 = cons("Feed", cons("1234", i3))
    s3 = writeRSSFeed(a3)
    assert car(s3) == itemFeed

    i4 = (("fruit", "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b",
            valueToElement(("'item", ("'name", "Apple"), ("'price", "$2.99")))),
          ("fruit", "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c",
            valueToElement(("'item", ("'name", "Orange"), ("'price", "$3.55")))))
    a4 = cons("Feed", cons("1234", i4))
    s4 = writeRSSFeed(a4)
    assert car(s4) == itemFeed

    a5 = readRSSFeed((itemFeed,));
    s5 = writeRSSFeed(a5);
    assert car(s5) == itemFeed

    i6 = (("fruit", "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b",
            (("'name", "Apple"), ("'price", "$2.99"))),
          ("fruit", "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c",
            (("'name", "Orange"), ("'price", "$3.55"))))
    a6 = cons("Feed", cons("1234", i6))
    s6 = writeRSSFeed(feedValuesToElements(a6))
    assert car(s6) == itemFeed

    return True

if __name__ == "__main__":
    print "Testing..."
    testEntry()
    testFeed()
    print "OK"

