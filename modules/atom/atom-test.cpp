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
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">item</title>"
        "<id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</id>"
        "<author>"
        "<email>jane@example.com</email>"
        "</author>"
        "<updated>Fri Jan 01 08:11:36 PDT 2012</updated>"
        "<content type=\"application/xml\">"
        "<item>"
        "<name>Apple</name>"
        "<price>$2.99</price>"
        "</item>"
        "</content>"
        "<link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b\"/>"
        "</entry>\n");

const string itemTextEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">item</title>"
        "<id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</id>"
        "<author>"
        "<email>jane@example.com</email>"
        "</author>"
        "<updated>Fri Jan 01 08:11:36 PDT 2012</updated>"
        "<content type=\"text\">Apple</content>"
        "<link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b\"/>"
        "</entry>\n");

const string itemNoContentEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">item</title>"
        "<id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</id>"
        "<author>"
        "<name>jane</name>"
        "</author>"
        "<updated>Fri Jan 01 08:11:36 PDT 2012</updated>"
        "<link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b\"/>"
        "</entry>\n");

const string incompleteEntry("<entry xmlns=\"http://www.w3.org/2005/Atom\">\n"
        "<title>item</title>"
        "<content type=\"text/xml\">"
        "<Item xmlns=\"http://services/\">"
        "<name xmlns=\"\">Orange</name>"
        "<price xmlns=\"\">3.55</price>"
        "</Item>"
        "</content>"
        "</entry>\n");

const string completedEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">item</title>"
        "<id></id>"
        "<content type=\"application/xml\">"
        "<Item xmlns=\"http://services/\">"
        "<name xmlns=\"\">Orange</name>"
        "<price xmlns=\"\">3.55</price>"
        "</Item>"
        "</content>"
        "<link href=\"\"/>"
        "</entry>\n");

const bool testEntry() {
    {
        const list<value> i = nilListValue + element + value("item")
                + value(nilListValue + element + value("name") + value(string("Apple")))
                + value(nilListValue + element + value("price") + value(string("$2.99")));
        const list<value> a = nilListValue + (nilListValue + element + value("entry")
                + value(nilListValue + element + value("title") + value(string("item")))
                + value(nilListValue + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b")))
                + value(nilListValue + element + value("author") + value(string("jane@example.com")))
                + value(nilListValue + element + value("updated") + value(string("Fri Jan 01 08:11:36 PDT 2012")))
                + value(nilListValue + element + value("content") + value(i)));
        ostringstream os;
        writeATOMEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemEntry);
    }
    {
        const list<value> a = nilListValue + (nilListValue + element + value("entry")
                + value(nilListValue + element + value("title") + value(string("item")))
                + value(nilListValue + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b")))
                + value(nilListValue + element + value("author") + value(string("jane@example.com")))
                + value(nilListValue + element + value("updated") + value(string("Fri Jan 01 08:11:36 PDT 2012")))
                + value(nilListValue + element + value("content") + value(string("Apple"))));
        ostringstream os;
        writeATOMEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemTextEntry);
    }
    {
        const list<value> a = nilListValue + (nilListValue + element + value("entry")
                + value(nilListValue + element + value("title") + value(string("item")))
                + value(nilListValue + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b")))
                + value(nilListValue + element + value("author") + value(string("jane")))
                + value(nilListValue + element + value("updated") + value(string("Fri Jan 01 08:11:36 PDT 2012"))));
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
        "<feed xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">Feed</title>"
        "<id>1234</id>"
        "</feed>\n");

const string itemFeed("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<feed xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">Feed</title>"
        "<id>1234</id>"
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">item</title>"
        "<id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</id>"
        "<author>"
        "<email>jane@example.com</email>"
        "</author>"
        "<updated>Fri Jan 01 08:11:36 PDT 2012</updated>"
        "<content type=\"application/xml\">"
        "<item>"
        "<name>Apple</name>"
        "<price>$2.99</price>"
        "</item>"
        "</content>"
        "<link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b\"/>"
        "</entry>"
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">item</title>"
        "<id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c</id>"
        "<author>"
        "<email>jane@example.com</email>"
        "</author>"
        "<updated>Fri Jan 01 08:11:36 PDT 2012</updated>"
        "<content type=\"application/xml\">"
        "<item>"
        "<name>Orange</name>"
        "<price>$3.55</price>"
        "</item>"
        "</content>"
        "<link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c\"/>"
        "</entry>"
        "</feed>\n");

const bool testFeed() {
    {
        const list<value> a = nilListValue + (nilListValue + element + value("feed")
                + value(nilListValue + element + value("title") + value(string("Feed")))
                + value(nilListValue + element + value("id") + value(string("1234")))
                + value(nilListValue + element + value("author") + value(string("jane@example.com")))
                + value(nilListValue + element + value("updated") + value(string("Fri Jan 01 08:11:36 PDT 2012"))));
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
        const list<value> i1 = nilListValue + element + "item"
                        + (nilListValue + element + "name" + "Apple")
                        + (nilListValue + element + "price" + "$2.99");

        const list<value> i2 = nilListValue + element + "item"
                        + (nilListValue + element + "name" + "Orange")
                        + (nilListValue + element + "price" + "$3.55");

        const list<value> i = nilListValue
            + value(nilListValue + element + value("entry")
                + value(nilListValue + element + value("title") + value(string("item")))
                + value(nilListValue + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b")))
                + value(nilListValue + element + value("author") + value(string("jane@example.com")))
                + value(nilListValue + element + value("updated") + value(string("Fri Jan 01 08:11:36 PDT 2012")))
                + value(nilListValue + element + value("content") + value(i1)))
            + value(nilListValue + element + value("entry")
                + value(nilListValue + element + value("title") + value(string("item")))
                + value(nilListValue + element + value("id") + value(string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c")))
                + value(nilListValue + element + value("author") + value(string("jane@example.com")))
                + value(nilListValue + element + value("updated") + value(string("Fri Jan 01 08:11:36 PDT 2012")))
                + value(nilListValue + element + value("content") + value(i2)));
        
        const list<value> a = nilListValue + (append<value>(nilListValue + element + value("feed")
                + value(nilListValue + element + value("title") + value(string("Feed")))
                + value(nilListValue + element + value("id") + value("1234")),
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
    const tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::atom::testEntry();
    tuscany::atom::testFeed();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
