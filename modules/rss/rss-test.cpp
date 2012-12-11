/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/* $Rev$ $Date$ */

/**
 * Test RSS data conversion functions.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "rss.hpp"

namespace tuscany {
namespace rss {

ostream* writer(const string& s, ostream* os) {
    (*os) << s;
    return os;
}

const string itemEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<item>"
        "<title>fruit</title>"
        "<link>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</link>"
        "<description>"
        "<item>"
        "<name>Apple</name>"
        "<price>$2.99</price>"
        "</item>"
        "</description>"
        "</item>\n");

const string itemTextEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<item>"
        "<title>fruit</title>"
        "<link>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</link>"
        "<description>Apple</description>"
        "</item>\n");

const string itemNoDescriptionEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<item>"
        "<title>fruit</title>"
        "<link>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</link>"
        "</item>\n");

const string incompleteEntry("<item>\n"
        "<title>fruit</title>"
        "<description>"
        "<Item xmlns=\"http://services/\">"
        "<name xmlns=\"\">Orange</name>"
        "<price xmlns=\"\">3.55</price>"
        "</Item>"
        "</description>"
        "</item>");

const string completedEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<item>"
        "<title>fruit</title>"
        "<link></link>"
        "<description>"
        "<Item xmlns=\"http://services/\">"
        "<name xmlns=\"\">Orange</name>"
        "<price xmlns=\"\">3.55</price>"
        "</Item>"
        "</description>"
        "</item>\n");

const bool testEntry() {
    {
        const list<value> i = nilListValue + element + value("item")
                + value(nilListValue + element + value("name") + value(string("Apple")))
                + value(nilListValue + element + value("price") + value(string("$2.99")));
        const list<value> a = nilListValue + (nilListValue + element + value("entry")
                + value(nilListValue + element + value("title") + value(string("fruit")))
                + value(nilListValue + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b")))
                + value(nilListValue + element + value("content") + value(i)));
        ostringstream os;
        writeRSSEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemEntry);
    }
    {
        const list<value> a = nilListValue + (nilListValue + element + value("entry")
                + value(nilListValue + element + value("title") + value(string("fruit")))
                + value(nilListValue + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b")))
                + value(nilListValue + element + value("content") + value(string("Apple"))));
        ostringstream os;
        writeRSSEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemTextEntry);
    }
    {
        const list<value> a = nilListValue + (nilListValue + element + value("entry")
                + value(nilListValue + element + value("title") + value(string("fruit")))
                + value(nilListValue + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))));
        ostringstream os;
        writeRSSEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemNoDescriptionEntry);
    }
    {
        const list<value> a = content(readRSSEntry(mklist(itemEntry)));
        ostringstream os;
        writeRSSEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemEntry);
    }
    {
        const list<value> a = content(readRSSEntry(mklist(itemTextEntry)));
        ostringstream os;
        writeRSSEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemTextEntry);
    }
    {
        const list<value> a = content(readRSSEntry(mklist(itemNoDescriptionEntry)));
        ostringstream os;
        writeRSSEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemNoDescriptionEntry);
    }
    {
        const list<value> a = content(readRSSEntry(mklist(incompleteEntry)));
        ostringstream os;
        writeRSSEntry<ostream*>(writer, &os, a);
        assert(str(os) == completedEntry);
    }
    return true;
}

const string emptyFeed("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<rss version=\"2.0\">"
        "<channel>"
        "<title>Feed</title>"
        "<link>1234</link>"
        "<description>Feed</description>"
        "</channel>"
        "</rss>\n");

const string itemFeed("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<rss version=\"2.0\">"
        "<channel>"
        "<title>Feed</title>"
        "<link>1234</link>"
        "<description>Feed</description>"
        "<item>"
        "<title>fruit</title>"
        "<link>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</link>"
        "<description>"
        "<item>"
        "<name>Apple</name>"
        "<price>$2.99</price>"
        "</item>"
        "</description>"
        "</item>"
        "<item>"
        "<title>fruit</title>"
        "<link>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c</link>"
        "<description>"
        "<item>"
        "<name>Orange</name>"
        "<price>$3.55</price>"
        "</item>"
        "</description>"
        "</item>"
        "</channel>"
        "</rss>\n");

const bool testFeed() {
    {
        const list<value> a = nilListValue + (nilListValue + element + value("feed")
                + value(nilListValue + element + value("title") + value(string("Feed")))
                + value(nilListValue + element + value("id") + value(string("1234"))));
        ostringstream os;
        writeRSSFeed<ostream*>(writer, &os, a);
        assert(str(os) == emptyFeed);
    }
    {
        const list<value> a = content(readRSSFeed(mklist(emptyFeed)));
        ostringstream os;
        writeRSSFeed<ostream*>(writer, &os, a);
        assert(str(os) == emptyFeed);
    }
    {
        const list<value> i1 = nilListValue + element + "item"
                        + (nilListValue + element + "name" + "Apple")
                        + (nilListValue + element + "price" + "$2.99");

        const list<value> i2 = nilListValue + element + "item"
                        + (nilListValue + element + "name" + "Orange")
                        + (nilListValue + element + "price" + "$3.55");

        const list<value> i = nilListValue
            + value(nilListValue + element + value("entry")
                + value(nilListValue + element + value("title") + value(string("fruit")))
                + value(nilListValue + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b")))
                + value(nilListValue + element + value("content") + value(i1)))
            + value(nilListValue + element + value("entry")
                + value(nilListValue + element + value("title") + value(string("fruit")))
                + value(nilListValue + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c")))
                + value(nilListValue + element + value("content") + value(i2)));
        
        const list<value> a = nilListValue + (append<value>(nilListValue + element + value("feed")
                + value(nilListValue + element + value("title") + value(string("Feed")))
                + value(nilListValue + element + value("id") + value("1234")),
                i));

        ostringstream os;
        writeRSSFeed<ostream*>(writer, &os, a);
        assert(str(os) == itemFeed);
    }
    {
        const list<value> a = content(readRSSFeed(mklist(itemFeed)));
        ostringstream os;
        writeRSSFeed<ostream*>(writer, &os, a);
        assert(str(os) == itemFeed);
    }
    {
        const list<value> a = elementsToValues(content(readRSSFeed(mklist(itemFeed))));
        ostringstream os;
        writeRSSFeed<ostream*>(writer, &os, valuesToElements(a));
        assert(str(os) == itemFeed);
    }
    return true;
}

}
}

int main() {
    const tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::rss::testEntry();
    tuscany::rss::testFeed();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
