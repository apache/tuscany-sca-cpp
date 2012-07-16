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
 * Test ATOM data conversion functions.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "atom.hpp"

namespace tuscany {
namespace atom {

ostream* writer(const string& s, ostream* os) {
    (*os) << s;
    return os;
}

const string itemEntry(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">\n"
        " <title type=\"text\">item</title>\n"
        " <id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</id>\n"
        " <author>\n"
        "  <email>jane@example.com</email>\n"
        " </author>\n"
        " <updated>Fri Jan 01 08:11:36 PDT 2012</updated>\n"
        " <content type=\"application/xml\">\n"
        "  <item>\n"
        "   <name>Apple</name>\n"
        "   <price>$2.99</price>\n"
        "  </item>\n"
        " </content>\n"
        " <link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b\"/>\n"
        "</entry>\n");

const string itemTextEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">\n"
        " <title type=\"text\">item</title>\n"
        " <id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</id>\n"
        " <author>\n"
        "  <email>jane@example.com</email>\n"
        " </author>\n"
        " <updated>Fri Jan 01 08:11:36 PDT 2012</updated>\n"
        " <content type=\"text\">Apple</content>\n"
        " <link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b\"/>\n"
        "</entry>\n");

const string itemNoContentEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">\n"
        " <title type=\"text\">item</title>\n"
        " <id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</id>\n"
        " <author>\n"
        "  <name>jane</name>\n"
        " </author>\n"
        " <updated>Fri Jan 01 08:11:36 PDT 2012</updated>\n"
        " <link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b\"/>\n"
        "</entry>\n");

const string incompleteEntry("<entry xmlns=\"http://www.w3.org/2005/Atom\">\n"
        " <title>item</title>\n"
        " <content type=\"text/xml\">\n"
        "  <Item xmlns=\"http://services/\">\n"
        "   <name xmlns=\"\">Orange</name>\n"
        "   <price xmlns=\"\">3.55</price>\n"
        "  </Item>\n"
        " </content>\n"
        "</entry>\n");

const string completedEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">\n"
        " <title type=\"text\">item</title>\n"
        " <id></id>\n"
        " <content type=\"application/xml\">\n"
        "  <Item xmlns=\"http://services/\">\n"
        "   <name xmlns=\"\">Orange</name>\n"
        "   <price xmlns=\"\">3.55</price>\n"
        "  </Item>\n"
        " </content>\n"
        " <link href=\"\"/>\n"
        "</entry>\n");

bool testEntry() {
    {
        const list<value> i = list<value>() + element + value("item")
                + value(list<value>() + element + value("name") + value(string("Apple")))
                + value(list<value>() + element + value("price") + value(string("$2.99")));
        const list<value> a = list<value>() + (list<value>() + element + value("entry")
                + value(list<value>() + element + value("title") + value(string("item")))
                + value(list<value>() + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b")))
                + value(list<value>() + element + value("author") + value(string("jane@example.com")))
                + value(list<value>() + element + value("updated") + value(string("Fri Jan 01 08:11:36 PDT 2012")))
                + value(list<value>() + element + value("content") + value(i)));
        ostringstream os;
        writeATOMEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemEntry);
    }
    {
        const list<value> a = list<value>() + (list<value>() + element + value("entry")
                + value(list<value>() + element + value("title") + value(string("item")))
                + value(list<value>() + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b")))
                + value(list<value>() + element + value("author") + value(string("jane@example.com")))
                + value(list<value>() + element + value("updated") + value(string("Fri Jan 01 08:11:36 PDT 2012")))
                + value(list<value>() + element + value("content") + value(string("Apple"))));
        ostringstream os;
        writeATOMEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemTextEntry);
    }
    {
        const list<value> a = list<value>() + (list<value>() + element + value("entry")
                + value(list<value>() + element + value("title") + value(string("item")))
                + value(list<value>() + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b")))
                + value(list<value>() + element + value("author") + value(string("jane")))
                + value(list<value>() + element + value("updated") + value(string("Fri Jan 01 08:11:36 PDT 2012"))));
        ostringstream os;
        writeATOMEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemNoContentEntry);
    }
    {
        const list<value> a = content(readATOMEntry(mklist(itemEntry)));
        ostringstream os;
        writeATOMEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemEntry);
    }
    {
        const list<value> a = content(readATOMEntry(mklist(itemTextEntry)));
        ostringstream os;
        writeATOMEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemTextEntry);
    }
    {
        const list<value> a = content(readATOMEntry(mklist(itemNoContentEntry)));
        ostringstream os;
        writeATOMEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemNoContentEntry);
    }
    {
        const list<value> a = content(readATOMEntry(mklist(incompleteEntry)));
        ostringstream os;
        writeATOMEntry<ostream*>(writer, &os, a);
        assert(str(os) == completedEntry);
    }
    return true;
}

const string emptyFeed("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<feed xmlns=\"http://www.w3.org/2005/Atom\">\n"
        " <title type=\"text\">Feed</title>\n"
        " <id>1234</id>\n"
        "</feed>\n");

const string itemFeed("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<feed xmlns=\"http://www.w3.org/2005/Atom\">\n"
        " <title type=\"text\">Feed</title>\n"
        " <id>1234</id>\n"
        " <entry xmlns=\"http://www.w3.org/2005/Atom\">\n"
        "  <title type=\"text\">item</title>\n"
        "  <id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</id>\n"
        "  <author>\n"
        "   <email>jane@example.com</email>\n"
        "  </author>\n"
        "  <updated>Fri Jan 01 08:11:36 PDT 2012</updated>\n"
        "  <content type=\"application/xml\">\n"
        "   <item>\n"
        "    <name>Apple</name>\n"
        "    <price>$2.99</price>\n"
        "   </item>\n"
        "  </content>\n"
        "  <link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b\"/>\n"
        " </entry>\n"
        " <entry xmlns=\"http://www.w3.org/2005/Atom\">\n"
        "  <title type=\"text\">item</title>\n"
        "  <id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c</id>\n"
        "  <author>\n"
        "   <email>jane@example.com</email>\n"
        "  </author>\n"
        "  <updated>Fri Jan 01 08:11:36 PDT 2012</updated>\n"
        "  <content type=\"application/xml\">\n"
        "   <item>\n"
        "    <name>Orange</name>\n"
        "    <price>$3.55</price>\n"
        "   </item>\n"
        "  </content>\n"
        "  <link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c\"/>\n"
        " </entry>\n"
        "</feed>\n");

bool testFeed() {
    {
        const list<value> a = list<value>() + (list<value>() + element + value("feed")
                + value(list<value>() + element + value("title") + value(string("Feed")))
                + value(list<value>() + element + value("id") + value(string("1234")))
                + value(list<value>() + element + value("author") + value(string("jane@example.com")))
                + value(list<value>() + element + value("updated") + value(string("Fri Jan 01 08:11:36 PDT 2012"))));
        ostringstream os;
        writeATOMFeed<ostream*>(writer, &os, a);
        assert(str(os) == emptyFeed);
    }
    {
        const list<value> a = content(readATOMFeed(mklist(emptyFeed)));
        ostringstream os;
        writeATOMFeed<ostream*>(writer, &os, a);
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
                + value(list<value>() + element + value("title") + value(string("item")))
                + value(list<value>() + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b")))
                + value(list<value>() + element + value("author") + value(string("jane@example.com")))
                + value(list<value>() + element + value("updated") + value(string("Fri Jan 01 08:11:36 PDT 2012")))
                + value(list<value>() + element + value("content") + value(i1)))
            + value(list<value>() + element + value("entry")
                + value(list<value>() + element + value("title") + value(string("item")))
                + value(list<value>() + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c")))
                + value(list<value>() + element + value("author") + value(string("jane@example.com")))
                + value(list<value>() + element + value("updated") + value(string("Fri Jan 01 08:11:36 PDT 2012")))
                + value(list<value>() + element + value("content") + value(i2)));
        
        const list<value> a = list<value>() + (append<value>(list<value>() + element + value("feed")
                + value(list<value>() + element + value("title") + value(string("Feed")))
                + value(list<value>() + element + value("id") + value("1234")),
                i));

        ostringstream os;
        writeATOMFeed<ostream*>(writer, &os, a);
        assert(str(os) == itemFeed);
    }
    {
        const list<value> a = content(readATOMFeed(mklist(itemFeed)));
        ostringstream os;
        writeATOMFeed<ostream*>(writer, &os, a);
        assert(str(os) == itemFeed);
    }
    {
        const list<value> a = elementsToValues(content(readATOMFeed(mklist(itemFeed))));
        ostringstream os;
        writeATOMFeed<ostream*>(writer, &os, valuesToElements(a));
        assert(str(os) == itemFeed);
    }
    return true;
}

}
}

int main() {
    tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::atom::testEntry();
    tuscany::atom::testFeed();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
