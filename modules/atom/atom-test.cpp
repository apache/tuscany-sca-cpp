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

string itemEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">item</title>"
        "<id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</id>"
        "<content type=\"application/xml\">"
        "<item>"
        "<name>Apple</name><price>$2.99</price>"
        "</item>"
        "</content>"
        "<link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b\"/>"
        "</entry>\n");

string itemTextEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">item</title>"
        "<id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</id>"
        "<content type=\"text\">Apple</content>"
        "<link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b\"/>"
        "</entry>\n");

string incompleteEntry("<entry xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title>item</title><content type=\"text/xml\">"
        "<Item xmlns=\"http://services/\">"
        "<name xmlns=\"\">Orange</name>"
        "<price xmlns=\"\">3.55</price>"
        "</Item>"
        "</content>"
        "</entry>");

string completedEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">item</title>"
        "<id></id>"
        "<content type=\"application/xml\">"
        "<Item xmlns=\"http://services/\">"
        "<name xmlns=\"\">Orange</name>"
        "<price xmlns=\"\">3.55</price>"
        "</Item>"
        "</content><link href=\"\"/>"
        "</entry>\n");

bool testEntry() {
    {
        const list<value> i = list<value>() + element + value("item")
                + value(list<value>() + element + value("name") + value(string("Apple")))
                + value(list<value>() + element + value("price") + value(string("$2.99")));
        const list<value> a = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
        ostringstream os;
        writeATOMEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemEntry);
    }
    {
        const list<value> a = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), string("Apple"));
        ostringstream os;
        writeATOMEntry<ostream*>(writer, &os, a);
        assert(str(os) == itemTextEntry);
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
        const list<value> a = content(readATOMEntry(mklist(incompleteEntry)));
        ostringstream os;
        writeATOMEntry<ostream*>(writer, &os, a);
        assert(str(os) == completedEntry);
    }
    return true;
}

string emptyFeed("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<feed xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">Feed</title>"
        "<id>1234</id>"
        "</feed>\n");

string itemFeed("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<feed xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">Feed</title>"
        "<id>1234</id>"
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">item</title>"
        "<id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b</id>"
        "<content type=\"application/xml\">"
        "<item>"
        "<name>Apple</name><price>$2.99</price>"
        "</item>"
        "</content>"
        "<link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b\"/>"
        "</entry>"
        "<entry xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">item</title>"
        "<id>cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c</id>"
        "<content type=\"application/xml\">"
        "<item>"
        "<name>Orange</name><price>$3.55</price>"
        "</item>"
        "</content>"
        "<link href=\"cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c\"/>"
        "</entry>"
        "</feed>\n");

bool testFeed() {
    {
        ostringstream os;
        writeATOMFeed<ostream*>(writer, &os, mklist<value>("Feed", "1234"));
        assert(str(os) == emptyFeed);
    }
    {
        const list<value> a = content(readATOMFeed(mklist(emptyFeed)));
        ostringstream os;
        writeATOMFeed<ostream*>(writer, &os, a);
        assert(str(os) == emptyFeed);
    }
    {
        const list<value> i = list<value>()
                + (list<value>() + "item" + "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"
                    + (list<value>() + element + "item"
                        + (list<value>() + element + "name" + "Apple")
                        + (list<value>() + element + "price" + "$2.99")))
                + (list<value>() + "item" + "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c"
                    + (list<value>() + element + "item"
                        + (list<value>() + element + "name" + "Orange")
                        + (list<value>() + element + "price" + "$3.55")));
        const list<value> a = cons<value>("Feed", cons<value>("1234", i));
        ostringstream os;
        writeATOMFeed<ostream*>(writer, &os, a);
        assert(str(os) == itemFeed);
    }
    {
        const list<value> i = list<value>()
                + (list<value>() + "item" + "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"
                    + valueToElement(list<value>() + "item"
                        + (list<value>() + "name" + "Apple")
                        + (list<value>() + "price" + "$2.99")))
                + (list<value>() + "item" + "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c"
                    + valueToElement(list<value>() + "item"
                        + (list<value>() + "name" + "Orange")
                        + (list<value>() + "price" + "$3.55")));
        const list<value> a = cons<value>("Feed", cons<value>("1234", i));
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
    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::atom::testEntry();
    tuscany::atom::testFeed();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
