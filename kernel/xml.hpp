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

#ifndef tuscany_xml_hpp
#define tuscany_xml_hpp

/**
 * XML read/write functions.
 */

#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlschemas.h>
#include <libxml/globals.h>
#include <string>
#include "list.hpp"
#include "slist.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"

namespace tuscany {

/**
 * Initializes the libxml2 library.
 */
class XMLParser {
public:
    XMLParser() {
        xmlInitParser();
    }

    ~XMLParser() {
        xmlCleanupParser();
    }
};

XMLParser xmlParser;

/**
 * Encapsulates a libxml2 xmlTextReader and its state.
 */
class XMLReader {
public:
    enum TokenType {
        None = 0, Element = 1, Attribute = 2, Text = 3, EndElement = 15, Identifier = 100, End = 101
    };

    XMLReader(xmlTextReaderPtr xml) : xml(xml), tokenType(None), isEmptyElement(false), hasValue(false), hasAttributes(false) {
        xmlTextReaderSetParserProp(xml, XML_PARSER_DEFAULTATTRS, 1);
        xmlTextReaderSetParserProp(xml, XML_PARSER_SUBST_ENTITIES, 1);
    }

    ~XMLReader() {
        xmlTextReaderClose(xml);
        xmlFreeTextReader(xml);
    }

    /**
     * Read the next XML token and return its type.
     */
    int read() {
        if (tokenType == End)
            return tokenType;
        if (tokenType == Element) {
            isEmptyElement = xmlTextReaderIsEmptyElement(xml);
            hasAttributes = xmlTextReaderHasAttributes(xml);
            return tokenType = Identifier;
        }
        if (tokenType == Identifier && hasAttributes && xmlTextReaderMoveToFirstAttribute(xml) == 1)
            return tokenType = Attribute;
        if (tokenType == Attribute && xmlTextReaderMoveToNextAttribute(xml) == 1)
            return tokenType = Attribute;
        if (isEmptyElement && (tokenType == Identifier || tokenType == Attribute))
            return tokenType = EndElement;
        if (!xmlTextReaderRead(xml))
            return tokenType = End;
        return tokenType = xmlTextReaderNodeType(xml);
    }

    operator xmlTextReaderPtr() const {
        return xml;
    }

private:
    const xmlTextReaderPtr xml;
    int tokenType;
    bool isEmptyElement;
    bool hasValue;
    bool hasAttributes;
};

/**
 * Constants used to tag XML tokens.
 */
const value endElement("<");
const value startElement(">");

/**
 * Read an XML identifier.
 */
const value readIdentifier(XMLReader& reader) {
    const char* name = (const char*)xmlTextReaderConstName(reader);
    return name;
}

/**
 * Read XML text.
 */
const value readText(XMLReader& reader) {
    const char *val = (const char*)xmlTextReaderConstValue(reader);
    return std::string(val);
}

/**
 * Read an XML attribute.
 */
const value readAttribute(XMLReader& reader) {
    const char *name = (const char*)xmlTextReaderConstName(reader);
    const char *val = (const char*)xmlTextReaderConstValue(reader);
    return mklist<value>(attribute, name, std::string(val));
}

/**
 * Read an XML token.
 */
const value readToken(XMLReader& reader) {
    const int tokenType = reader.read();
    if (tokenType == XMLReader::None || tokenType == XMLReader::End)
        return value();
    if (tokenType == XMLReader::Element)
        return startElement;
    if (tokenType == XMLReader::Identifier)
        return readIdentifier(reader);
    if (tokenType == XMLReader::Attribute)
        return readAttribute(reader);
    if (tokenType == XMLReader::Text)
        return readText(reader);
    if (tokenType == XMLReader::EndElement)
        return endElement;
    return readToken(reader);
}

/**
 * Read a list of values from XML tokens.
 */
const list<value> readList(const list<value>& listSoFar, XMLReader& reader) {
    const value token = readToken(reader);
    if(isNil(token) || endElement == token)
        return reverse(listSoFar);
    if(startElement == token)
        return readList(cons<value>(readList(mklist(element), reader), listSoFar), reader);
    return readList(cons(token, listSoFar), reader);
}

/**
 * Read a list of values from a libxml2 XML reader.
 */
const list<value> read(XMLReader& reader) {
    value nextToken = readToken(reader);
    if (startElement == nextToken)
        return mklist<value>(readList(mklist(element), reader));
    return list<value>();
}

/**
 * Context passed to the read callback function.
 */
class XMLReadContext {
public:
    XMLReadContext(const list<std::string>& ilist) : ilist(ilist) {
    }
    list<std::string> ilist;
};

/**
 * Callback function called by libxml2 to read XML.
 */
int readCallback(void *context, char* buffer, int len) {
    XMLReadContext& rc = *static_cast<XMLReadContext*>(context);
    if (isNil(rc.ilist))
        return 0;
    rc.ilist = fragment(rc.ilist, len);
    std::string s = car(rc.ilist);
    rc.ilist = cdr(rc.ilist);
    s.copy(buffer, s.length());
    return s.length();
}

/**
 * Read a list values from a list of strings representing an XML document.
 */
const list<value> readXML(const list<std::string>& ilist) {
    XMLReadContext cx(ilist);
    xmlTextReaderPtr xml = xmlReaderForIO(readCallback, NULL, &cx, NULL, NULL, XML_PARSE_NONET);
    if (xml == NULL)
        return list<value>();
    XMLReader reader(xml);
    return read(reader);
}

/**
 * Default encoding used to write XML documents.
 */
const char* encoding = "UTF-8";


/**
 * Write a list of XML element or attribute tokens.
 */
const list<value> expandElementValues(const value& n, const list<value>& l) {
    if (isNil(l))
        return l;
    return cons<value>(value(cons<value>(element, cons<value>(n, (list<value>)car(l)))), expandElementValues(n, cdr(l)));
}

const failable<bool, std::string> writeList(const list<value>& l, const xmlTextWriterPtr xml) {
    if (isNil(l))
        return true;

    // Write an attribute
    const value token(car(l));
    if (isTaggedList(token, attribute)) {
        if (xmlTextWriterWriteAttribute(xml, (const xmlChar*)std::string(attributeName(token)).c_str(), (const xmlChar*)std::string(attributeValue(token)).c_str()) < 0)
            return mkfailure<bool, std::string>("xmlTextWriterWriteAttribute failed");

    } else if (isTaggedList(token, element)) {

        // Write an element containing a value
        if (elementHasValue(token)) {
            const value v = elementValue(token);
            if (isList(v)) {

                // Write an element per entry in a list of values
                const list<value> e = expandElementValues(elementName(token), v);
                writeList(e, xml);

            } else {

                // Write an element with a single value
                if (xmlTextWriterStartElement(xml, (const xmlChar*)std::string(elementName(token)).c_str()) < 0)
                    return mkfailure<bool, std::string>("xmlTextWriterStartElement failed");

                // Write its children
                const failable<bool, std::string> w = writeList(elementChildren(token), xml);
                if (!hasValue(w))
                    return w;

                if (xmlTextWriterEndElement(xml) < 0)
                    return mkfailure<bool, std::string>("xmlTextWriterEndElement failed");
            }
        }
        else {

            // Write an element
            if (xmlTextWriterStartElement(xml, (const xmlChar*)std::string(elementName(token)).c_str()) < 0)
                return mkfailure<bool, std::string>("xmlTextWriterStartElement failed");

            // Write its children
            const failable<bool, std::string> w = writeList(elementChildren(token), xml);
            if (!hasValue(w))
                return w;

            if (xmlTextWriterEndElement(xml) < 0)
                return mkfailure<bool, std::string>("xmlTextWriterEndElement failed");
        }
    } else {

        // Write XML text
        if (xmlTextWriterWriteString(xml, (const xmlChar*)std::string(token).c_str()) < 0)
            return mkfailure<bool, std::string>("xmlTextWriterWriteString failed");

    }

    // Go on
    return writeList(cdr(l), xml);
}

/**
 * Write a list of values to a libxml2 XML writer.
 */
const failable<bool, std::string> write(const list<value>& l, const xmlTextWriterPtr xml) {
    if (xmlTextWriterStartDocument(xml, NULL, encoding, NULL) < 0)
        return mkfailure<bool, std::string>("xmlTextWriterStartDocument failed");

    const failable<bool, std::string> w = writeList(l, xml);
    if (!hasValue(w))
        return w;

    if (xmlTextWriterEndDocument(xml) < 0)
        return mkfailure<bool, std::string>("xmlTextWriterEndDocument failed");
    return true;
}

/**
 * Context passed to the write callback function.
 */
template<typename R> class XMLWriteContext {
public:
    XMLWriteContext(const lambda<R(std::string, R)>& reduce, const R& accum) : reduce(reduce), accum(accum) {
    }
    const lambda<R(std::string, R)> reduce;
    R accum;
};

/**
 * Callback function called by libxml2 to write XML out.
 */
template<typename R> int writeCallback(void *context, const char* buffer, int len) {
    XMLWriteContext<R>& cx = *static_cast<XMLWriteContext<R>*>(context);
    cx.accum = cx.reduce(std::string(buffer, len), cx.accum);
    return len;
}

/**
 * Convert a list of values to an XML document.
 */
template<typename R> const failable<R, std::string> writeXML(const lambda<R(std::string, R)>& reduce, const R& initial, const list<value>& l) {
    XMLWriteContext<R> cx(reduce, initial);
    xmlOutputBufferPtr out = xmlOutputBufferCreateIO(writeCallback<R>, NULL, &cx, NULL);
    if (out == NULL)
        return mkfailure<R, std::string>("xmlOutputBufferCreateIO failed");
    xmlTextWriterPtr xml = xmlNewTextWriter(out);
    if (xml == NULL)
        return mkfailure<R, std::string>("xmlNewTextWriter failed");

    const failable<bool, std::string> w = write(l, xml);
    xmlFreeTextWriter(xml);
    if (!hasValue(w)) {
        return mkfailure<R, std::string>(reason(w));
    }
    return cx.accum;
}

/**
 * Convert a list of values to a list of strings representing an XML document.
 */
const failable<list<std::string>, std::string> writeXML(const list<value>& l) {
    const failable<list<std::string>, std::string> ls = writeXML<list<std::string> >(rcons<std::string>, list<std::string>(), l);
    if (!hasValue(ls))
        return ls;
    return reverse(list<std::string>(ls));
}

}
#endif /* tuscany_xml_hpp */
