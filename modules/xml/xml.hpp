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
#include "string.hpp"
#include "list.hpp"
#include "stream.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"

namespace tuscany {
namespace xml {

/**
 * Initializes the libxml2 library.
 */
class XMLParser {
public:
    inline XMLParser() {
        debug("xml::XMLParser");
        xmlMemSetup(gc_pool_free, gc_pool_malloc, gc_pool_realloc, gc_pool_strdup);
        xmlInitParser();
    }

    inline ~XMLParser() {
    }
} xmlParser;

/**
 * Encapsulates a libxml2 xmlTextReader and its state.
 */
class XMLReader {
public:
    enum TokenType {
        None = 0, Element = 1, Attribute = 2, Text = 3, EndElement = 15, Identifier = 100, End = 101
    };

    inline XMLReader(xmlTextReaderPtr xml) : xml(xml), owner(true), tokenType(None), isEmptyElement(false), hasValue(false), hasAttributes(false) {
        debug("xml::XMLReader::xml");
        xmlTextReaderSetParserProp(xml, XML_PARSER_DEFAULTATTRS, 1);
        xmlTextReaderSetParserProp(xml, XML_PARSER_SUBST_ENTITIES, 1);
    }

    inline XMLReader(const XMLReader& r) : xml(r.xml), owner(false), tokenType(r.tokenType), isEmptyElement(r.isEmptyElement), hasValue(r.hasValue), hasAttributes(r.hasAttributes) {
        debug("xml::XMLReader::copy");
    }

    XMLReader& operator=(const XMLReader& r) = delete;

    inline ~XMLReader() {
        if (!owner)
            return;
        xmlTextReaderClose(xml);
        xmlFreeTextReader(xml);
    }

    /**
     * Read the next XML token and return its type.
     */
    inline const int read() {
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

    inline operator xmlTextReaderPtr() const {
        return xml;
    }

private:
    const xmlTextReaderPtr xml;
    const bool owner;
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
 * Read a value from a text string, automatically detects a boolean or number value.
 */
const value readTextValue(const char* const val) {
    if (!strcmp("true", val))
        return value(true);
    if (!strcmp("false", val))
        return value(false);

    char e[2];
    double d;
    const int n = sscanf(val, "%lg%1s", &d, e);
    if (n == 1)
        return value(d);

    return value(string(val));
}

/**
 * Read an XML identifier.
 */
inline const value readIdentifier(XMLReader& reader) {
    const char* const name = (const char*)xmlTextReaderConstName(reader);
    return name;
}

/**
 * Read XML text.
 */
inline const value readText(XMLReader& reader) {
    const char* const val = (const char*)xmlTextReaderConstValue(reader);
    return readTextValue(val);
}

/**
 * Read an XML attribute.
 */
inline const value readAttribute(XMLReader& reader) {
    const char* const name = (const char*)xmlTextReaderConstName(reader);
    const char* const val = (const char*)xmlTextReaderConstValue(reader);
    return mklist<value>(attribute, name, readTextValue(val));
}

/**
 * Read an XML token.
 */
inline const value readToken(XMLReader& reader) {
    const int tokenType = reader.read();
    if (tokenType == XMLReader::None || tokenType == XMLReader::End)
        return nilValue;
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
inline const list<value> readList(const list<value>& listSoFar, XMLReader& reader) {
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
inline const list<value> read(XMLReader& reader) {
    const value nextToken = readToken(reader);
    if (startElement == nextToken)
        return mklist<value>(readList(mklist(element), reader));
    return nilListValue;
}

/**
 * Context passed to the read callback function.
 */
class XMLReadContext {
public:
    inline XMLReadContext(const list<string>& ilist) : ilist(ilist) {
    }

    gc_mutable_ref<list<string> > ilist;
};

/**
 * Callback function called by libxml2 to read XML.
 */
inline int readCallback(void *context, char* buffer, int len) {
    XMLReadContext& rc = *(XMLReadContext*)context;
    if (isNil((const list<string>)rc.ilist))
        return 0;
    const list<string> f(fragment(rc.ilist, len));
    const string s(car(f));
    rc.ilist = cdr(f);
    memcpy(buffer, c_str(s), length(s));
    return (int)length(s);
}

/**
 * Return true if a list of strings contains an XML document.
 */
inline const bool isXML(const list<string>& ls) {
    if (isNil(ls))
        return false;
    return substr(car(ls), 0, 5) == "<?xml";
}

/**
 * Read a list of values from a list of strings representing an XML document.
 */
inline const failable<list<value> > readElements(const list<string>& ilist) {
    debug(ilist, "xml::readElements");
    XMLReadContext cx(ilist);
    const xmlTextReaderPtr xml = xmlReaderForIO(readCallback, NULL, &cx, NULL, NULL, XML_PARSE_NONET | XML_PARSE_NODICT);
    if (xml == NULL)
        return nilListValue;
    XMLReader reader(xml);
    return read(reader);
}

/**
 * Default encoding used to write XML documents.
 */
const char* const encoding = "UTF-8";


/**
 * Write a list of XML element or attribute tokens.
 */
inline const list<value> expandElementValues(const value& n, const list<value>& l) {
    if (isNil(l))
        return l;
    return cons<value>(value(cons<value>(element, cons<value>(n, isList(car(l))? (list<value>)car(l) : mklist(car(l))))), expandElementValues(n, cdr(l)));
}

inline const failable<bool> writeList(const list<value>& l, const xmlTextWriterPtr xml) {
    if (isNil(l))
        return true;

    // Write an attribute
    const value token(car(l));
    if (isTaggedList(token, attribute)) {
        if (xmlTextWriterWriteAttribute(xml, (const xmlChar*)c_str(string(attributeName(token))), (const xmlChar*)c_str(string(attributeValue(token)))) < 0)
            return mkfailure<bool>("xmlTextWriterWriteAttribute failed");

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
                if (xmlTextWriterStartElement(xml, (const xmlChar*)c_str(string(elementName(token)))) < 0)
                    return mkfailure<bool>("xmlTextWriterStartElement failed");

                // Write its children
                const failable<bool> w = writeList(elementChildren(token), xml);
                if (!hasContent(w))
                    return w;

                if (xmlTextWriterEndElement(xml) < 0)
                    return mkfailure<bool>("xmlTextWriterEndElement failed");
            }
        }
        else {

            // Write an element
            if (xmlTextWriterStartElement(xml, (const xmlChar*)c_str(string(elementName(token)))) < 0)
                return mkfailure<bool>("xmlTextWriterStartElement failed");

            // Write its children
            const failable<bool> w = writeList(elementChildren(token), xml);
            if (!hasContent(w))
                return w;

            if (xmlTextWriterEndElement(xml) < 0)
                return mkfailure<bool>("xmlTextWriterEndElement failed");
        }
    } else {

        // Write XML text
        if (xmlTextWriterWriteString(xml, (const xmlChar*)c_str(string(token))) < 0)
            return mkfailure<bool>("xmlTextWriterWriteString failed");
    }

    // Go on
    return writeList(cdr(l), xml);
}

/**
 * Write a list of values to a libxml2 XML writer.
 */
inline const failable<bool> write(const list<value>& l, const xmlTextWriterPtr xml, bool xmlTag) {
    if (xmlTag) {
        if (xmlTextWriterStartDocument(xml, NULL, encoding, NULL) < 0)
            return mkfailure<bool>("xmlTextWriterStartDocument failed");
    }

    const failable<bool> w = writeList(l, xml);
    if (!hasContent(w))
        return w;

    if (xmlTag) {
        if (xmlTextWriterEndDocument(xml) < 0)
            return mkfailure<bool>("xmlTextWriterEndDocument failed");
    }
    return true;
}

/**
 * Context passed to the write callback function.
 */
template<typename R> class XMLWriteContext {
public:
    inline XMLWriteContext(const lambda<const R(const string&, const R)>& reduce, const R& accum) : reduce(reduce), accum(accum) {
    }

    const lambda<const R(const string&, const R)> reduce;
    gc_mutable_ref<R> accum;
};

/**
 * Callback function called by libxml2 to write XML out.
 */
template<typename R> inline int writeCallback(void *context, const char* buffer, int len) {
    XMLWriteContext<R>& cx = *(XMLWriteContext<R>*)context;
    cx.accum = cx.reduce(string(buffer, len), cx.accum);
    return len;
}

/**
 * Convert a list of values to an XML document.
 */
template<typename R> inline const failable<R> writeElements(const lambda<const R(const string&, const R)>& reduce, const R& initial, const list<value>& l, const bool xmlTag) {
    XMLWriteContext<R> cx(reduce, initial);
    xmlOutputBufferPtr out = xmlOutputBufferCreateIO(writeCallback<R>, NULL, &cx, NULL);
    if (out == NULL)
        return mkfailure<R>("xmlOutputBufferCreateIO failed");
    xmlTextWriterPtr xml = xmlNewTextWriter(out);
    if (xml == NULL)
        return mkfailure<R>("xmlNewTextWriter failed");
    xmlTextWriterSetIndent(xml, 0);

    const failable<bool> w = write(l, xml, xmlTag);
    xmlFreeTextWriter(xml);
    if (!hasContent(w)) {
        return mkfailure<R>(w);
    }
    return (R)cx.accum;
}

template<typename R> inline const failable<R> writeElements(const lambda<const R(const string&, const R)>& reduce, const R& initial, const list<value>& l) {
    return writeElements(reduce, initial, l, true);
}

/**
 * Convert a list of values to a list of strings representing an XML document.
 */
inline const failable<list<string> > writeElements(const list<value>& l, const bool xmlTag) {
    debug(l, "xml::writeElements");
    const failable<list<string> > ls = writeElements<list<string> >(rcons<string>, list<string>(), l, xmlTag);
    if (!hasContent(ls))
        return ls;
    return reverse(list<string>(content(ls)));
}

inline const failable<list<string> > writeElements(const list<value>& l) {
    return writeElements(l, true);
}

}
}
#endif /* tuscany_xml_hpp */
