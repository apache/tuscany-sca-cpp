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
        "<item>\n"
        " <title>fruit</title>\n"
        " <link>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</link>\n"
        " <description>\n"
        "  <item>\n"
        "   <name>Apple</name>\n"
        "   <price>$2.99</price>\n"
        "  </item>\n"
        " </description>\n"
        "</item>\n");

const string itemTextEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<item>\n"
        " <title>fruit</title>\n"
        " <link>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</link>\n"
        " <description>Apple</description>\n"
        "</item>\n");

const string itemNoDescriptionEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<item>\n"
        " <title>fruit</title>\n"
        " <link>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</link>\n"
        "</item>\n");

const string incompleteEntry("<item>\n"
        " <title>fruit</title>\n"
        " <description>\n"
        "  <Item xmlns=\"http://services/\">\n"
        "   <name xmlns=\"\">Orange</name>\n"
        "   <price xmlns=\"\">3.55</price>\n"
        "  </Item>\n"
        " </description>\n"
        "</item>");

const string completedEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<item>\n"
        " <title>fruit</title>\n"
        " <link></link>\n"
        " <description>\n"
        "  <Item xmlns=\"http://services/\">\n"
        "   <name xmlns=\"\">Orange</name>\n"
        "   <price xmlns=\"\">3.55</price>\n"
        "  </Item>\n"
        " </description>\n"
        "</item>\n");

bool testEntry() {
    {
        const list<value> i = list<value>() + element + value("item")
                + value(list<value>() + element + value("name") + value(string("Apple")))
                + value(list<value>() + element + value("price") + value(string("$2.99")));
        const list<value> a = list<value>() + (list<value>() + element + value("entry")
                + value(list<value>() + element + value("title") + value(string("fruit")))
                + value(list<value>() + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b")))
                + value(list<value>() + element + value("content") + value(i)));
        ostringstream os;
        writeRSSEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemEntry);
    }
    {
        const list<value> a = list<value>() + (list<value>() + element + value("entry")
                + value(list<value>() + element + value("title") + value(string("fruit")))
                + value(list<value>() + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b")))
                + value(list<value>() + element + value("content") + value(string("Apple"))));
        ostringstream os;
        writeRSSEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemTextEntry);
    }
    {
        const list<value> a = list<value>() + (list<value>() + element + value("entry")
                + value(list<value>() + element + value("title") + value(string("fruit")))
                + value(list<value>() + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))));
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
        "<rss version=\"2.0\">\n"
        " <channel>\n"
        "  <title>Feed</title>\n"
        "  <link>1234</link>\n"
        "  <description>Feed</description>\n"
        " </channel>\n"
        "</rss>\n");

const string itemFeed("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<rss version=\"2.0\">\n"
        " <channel>\n"
        "  <title>Feed</title>\n"
        "  <link>1234</link>\n"
        "  <description>Feed</description>\n"
        "  <item>\n"
        "   <title>fruit</title>\n"
        "   <link>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</link>\n"
        "   <description>\n"
        "    <item>\n"
        "     <name>Apple</name>\n"
        "     <price>$2.99</price>\n"
        "    </item>\n"
        "   </description>\n"
        "  </item>\n"
        "  <item>\n"
        "   <title>fruit</title>\n"
        "   <link>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c</link>\n"
        "   <description>\n"
        "    <item>\n"
        "     <name>Orange</name>\n"
        "     <price>$3.55</price>\n"
        "    </item>\n"
        "   </description>\n"
        "  </item>\n"
        " </channel>\n"
        "</rss>\n");

bool testFeed() {
    {
        const list<value> a = list<value>() + (list<value>() + element + value("feed")
                + value(list<value>() + element + value("title") + value(string("Feed")))
                + value(list<value>() + element + value("id") + value(string("1234"))));
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
        const list<value> i1 = list<value>() + element + "item"
                        + (list<value>() + element + "name" + "Apple")
                        + (list<value>() + element + "price" + "$2.99");

        const list<value> i2 = list<value>() + element + "item"
                        + (list<value>() + element + "name" + "Orange")
                        + (list<value>() + element + "price" + "$3.55");

        const list<value> i = list<value>()
            + value(list<value>() + element + value("entry")
                + value(list<value>() + element + value("title") + value(string("fruit")))
                + value(list<value>() + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b")))
                + value(list<value>() + element + value("content") + value(i1)))
            + value(list<value>() + element + value("entry")
                + value(list<value>() + element + value("title") + value(string("fruit")))
                + value(list<value>() + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c")))
                + value(list<value>() + element + value("content") + value(i2)));
        
        const list<value> a = list<value>() + (append<value>(list<value>() + element + value("feed")
                + value(list<value>() + element + value("title") + value(string("Feed")))
                + value(list<value>() + element + value("id") + value("1234")),
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
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::rss::testEntry();
    tuscany::rss::testFeed();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
