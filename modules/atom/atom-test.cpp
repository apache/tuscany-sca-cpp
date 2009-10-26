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
#include <iostream>
#include <sstream>
#include <string>
#include "slist.hpp"
#include "atom.hpp"

namespace tuscany {
namespace atom {

std::ostringstream* writer(std::ostringstream* os, const std::string& s) {
    (*os) << s;
    return os;
}

std::string itemEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
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

std::string incompleteEntry("<entry xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title>item</title><content type=\"text/xml\">"
        "<Item xmlns=\"http://services/\">"
        "<name xmlns=\"\">Orange</name>"
        "<price xmlns=\"\">3.55</price>"
        "</Item>"
        "</content>"
        "</entry>");

std::string completedEntry("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
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
        const list<value> i = list<value>() << element << "item"
                << (list<value>() << element << "name" << std::string("Apple"))
                << (list<value>() << element << "price" << std::string("$2.99"));
        const list<value> a = mklist<value>(std::string("item"), std::string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
        std::ostringstream os;
        writeEntry<std::ostringstream*>(writer, &os, a);
        assert(os.str() == itemEntry);
    }
    {
        const list<value> a = readEntry(mklist(itemEntry));
        std::ostringstream os;
        writeEntry<std::ostringstream*>(writer, &os, a);
        assert(os.str() == itemEntry);
    }
    {
        const list<value> a = readEntry(mklist(incompleteEntry));
        std::ostringstream os;
        writeEntry<std::ostringstream*>(writer, &os, a);
        assert(os.str() == completedEntry);
    }
    return true;
}

std::string emptyFeed("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<feed xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title type=\"text\">Feed</title>"
        "<id>1234</id>"
        "</feed>\n");

std::string itemFeed("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
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
        std::ostringstream os;
        writeFeed<std::ostringstream*>(writer, &os, mklist<value>("Feed", "1234"));
        assert(os.str() == emptyFeed);
    }
    {
        const list<value> a = readFeed(mklist(emptyFeed));
        std::ostringstream os;
        writeFeed<std::ostringstream*>(writer, &os, a);
        assert(os.str() == emptyFeed);
    }
    {
        const list<value> i = list<value>()
                << (list<value>() << "item" << "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"
                    << (list<value>() << element << "item"
                        << (list<value>() << element << "name" << "Apple")
                        << (list<value>() << element << "price" << "$2.99")))
                << (list<value>() << "item" << "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c"
                    << (list<value>() << element << "item"
                        << (list<value>() << element << "name" << "Orange")
                        << (list<value>() << element << "price" << "$3.55")));
        const list<value> a = cons<value>("Feed", cons<value>("1234", i));
        std::ostringstream os;
        writeFeed<std::ostringstream*>(writer, &os, a);
        assert(os.str() == itemFeed);
    }
    {
        const list<value> i = list<value>()
                << (list<value>() << "item" << "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"
                    << valueToElement(list<value>() << "item"
                        << (list<value>() << "name" << "Apple")
                        << (list<value>() << "price" << "$2.99")))
                << (list<value>() << "item" << "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c"
                    << valueToElement(list<value>() << "item"
                        << (list<value>() << "name" << "Orange")
                        << (list<value>() << "price" << "$3.55")));
        const list<value> a = cons<value>("Feed", cons<value>("1234", i));
        std::ostringstream os;
        writeFeed<std::ostringstream*>(writer, &os, a);
        assert(os.str() == itemFeed);
    }
    {
        const list<value> a = readFeed(mklist(itemFeed));
        std::ostringstream os;
        writeFeed<std::ostringstream*>(writer, &os, a);
        assert(os.str() == itemFeed);
    }
    return true;
}

}
}

int main() {
    std::cout << "Testing..." << std::endl;

    tuscany::atom::testEntry();
    tuscany::atom::testFeed();

    std::cout << "OK" << std::endl;

    return 0;
}
