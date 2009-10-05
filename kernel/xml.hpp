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

namespace tuscany {

/**
 * Encapsulates a libxml2 xmlTextReader and its state.
 */
class XMLReader {
public:
    enum TokenType {
        None = 0, Element = 1, Attribute = 2, EndElement = 15, Identifier = 100, Text = 101, End = 103
    };

    XMLReader(xmlTextReaderPtr xml) : xml(xml), tokenType(None) {
        xmlTextReaderSetParserProp(xml, XML_PARSER_DEFAULTATTRS, 1);
        xmlTextReaderSetParserProp(xml, XML_PARSER_SUBST_ENTITIES, 1);
    }

    ~XMLReader() {
        xmlFreeTextReader(xml);
    }

    /**
     * Read the next token and return its type.
     */
    int read() {
        if (tokenType == End)
            return tokenType;
        if (tokenType == Element) {
            isEmptyElement = xmlTextReaderIsEmptyElement(xml);
            hasValue = xmlTextReaderHasValue(xml);
            hasAttributes = xmlTextReaderHasAttributes(xml);
            return tokenType = Identifier;
        }
        if (hasValue && tokenType == Identifier)
            return tokenType = Text;
        if (hasAttributes && (tokenType == Identifier || tokenType == Text) && xmlTextReaderMoveToFirstAttribute(xml) == 1)
            return tokenType = Attribute;
        if (tokenType == Attribute && xmlTextReaderMoveToNextAttribute(xml) == 1)
            return tokenType = Attribute;
        if (isEmptyElement && (tokenType == Identifier || tokenType == Text || tokenType == Attribute))
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
const value attribute("attribute");
const value element("element");

/**
 * Read an XML identifier.
 */
const value readIdentifier(XMLReader& reader) {
    const char* name = (const char*)xmlTextReaderConstName(reader);
    return value(name);
}

/**
 * Read XML text.
 */
const value readText(XMLReader& reader) {
    const char *val = (const char*)xmlTextReaderConstValue(reader);
    return value(std::string(val));
}

/**
 * Read an XML attribute.
 */
const value readAttribute(XMLReader& reader) {
    const char *name = (const char*)xmlTextReaderConstName(reader);
    const char *val = (const char*)xmlTextReaderConstValue(reader);
    return value(makeList(attribute, value(name), value(std::string(val))));
}

/**
 * Read an XML token.
 */
const value readToken(XMLReader& reader) {
    const int tokenType = reader.read();
    if (tokenType == XMLReader::End)
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
 * Read a list of XML tokens.
 */
const list<value> readList(const list<value>& listSoFar, XMLReader& reader) {
    const value token = readToken(reader);
    if(isNil(token) || endElement == token)
        return reverse(listSoFar);
    if(startElement == token)
        return readList(cons(value(readList(makeList(element), reader)), listSoFar), reader);
    return readList(cons(token, listSoFar), reader);
}

/**
 * Read a list of values from a libxml2 XML reader.
 */
const list<value> read(XMLReader& reader) {
    value nextToken = readToken(reader);
    if (startElement == nextToken)
        return makeList(value(readList(makeList(element), reader)));
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
 * Returns true if a value is an XML attribute.
 */
const bool isAttribute(const list<value>& l) {
    return !isNil(l) && car(l) == attribute;
}

/**
 * Returns the name of an XML attribute.
 */
const std::string attributeName(const list<value>& l) {
    return cadr(l);
}

/**
 * Returns the text value of an XML attribute.
 */
const std::string attributeText(const list<value>& l) {
    return caddr(l);
}

/**
 * Returns true if a value is an XML element.
 */
const bool isElement(const list<value>& l) {
    return !isNil(l) && car(l) == element;
}

/**
 * Returns the name of an XML element.
 */
const std::string elementName(const list<value>& l) {
    return cadr(l);
}

/**
 * Returns true if an XML element contains text content.
 */
const bool elementHasText(const list<value>& l) {
    if (isNil(cddr(l)))
        return false;
    return isString(caddr(l));
}

/**
 * Returns the text content of an XML element.
 */
const std::string elementText(const list<value>& l) {
    return caddr(l);
}

/**
 * Returns the children of an XML element.
 */
const list<value> elementChildren(const list<value>& l) {
    return cddr(l);
}

/**
 * Default encoding used to write XML documents.
 */
const char* encoding = "UTF-8";

/**
 * Write a list of XML element or attribute tokens.
 */
const bool writeList(const list<value>& l, const xmlTextWriterPtr xml) {
    if (isNil(l))
        return true;

    // Write an attribute
    const list<value> token(car(l));
    if (isAttribute(token)) {
        if (xmlTextWriterWriteAttribute(xml, (const xmlChar*)attributeName(token).c_str(), (const xmlChar*)attributeText(token).c_str()) < 0)
            return false;

    } else if (isElement(token)) {

        // Write an element
        if (xmlTextWriterStartElement(xml, (const xmlChar*)elementName(token).c_str()) < 0)
            return false;
        if (elementHasText(token) && xmlTextWriterWriteString(xml, (const xmlChar*)elementText(token).c_str()) < 0)
            return false;

        // Write its children
        writeList(elementChildren(token), xml);

        if (xmlTextWriterEndElement(xml) < 0)
            return false;
    }

    // Go on
    return writeList(cdr(l), xml);
}

/**
 * Write a list of values to a libxml2 XML writer.
 */
const bool write(const list<value>& l, const xmlTextWriterPtr xml) {
    if (xmlTextWriterStartDocument(xml, NULL, encoding, NULL) < 0)
        return false;
    writeList(l, xml);
    if (xmlTextWriterEndDocument(xml) < 0)
        return false;
    return true;
}

/**
 * Context passed to the write callback function.
 */
template<typename R> class XMLWriteContext {
public:
    XMLWriteContext(const lambda<R(R, std::string)>& reduce, const R& accum) : reduce(reduce), accum(accum) {
    }
    const lambda<R(R, std::string)> reduce;
    R accum;
};

/**
 * Callback function called by libxml2 to write XML out.
 */
template<typename R> int writeCallback(void *context, const char* buffer, int len) {
    XMLWriteContext<R>& cx = *static_cast<XMLWriteContext<R>*>(context);
    cx.accum = cx.reduce(cx.accum, std::string(buffer, len));
    return len;
}

/**
 * Write a list of values as an XML document.
 */
template<typename R> const R writeXML(const lambda<R(R, std::string)>& reduce, const R& initial, const list<value>& l) {
    XMLWriteContext<R> cx(reduce, initial);
    xmlOutputBufferPtr out = xmlOutputBufferCreateIO(writeCallback<R>, NULL, &cx, NULL);
    xmlTextWriterPtr xml = xmlNewTextWriter(out);
    if (xml != NULL)
        write(l, xml);
    return cx.accum;
}

const list<std::string> writeXMLList(const list<std::string>& listSoFar, const std::string& s) {
    return cons(s, listSoFar);
}

/**
 * Write a list of values as an XML document represented as a list of strings.
 */
const list<std::string> writeXML(const list<value>& l) {
    lambda<list<std::string>(list<std::string>, std::string)> writer(writeXMLList);
    return reverse(writeXML(writer, list<std::string>(), l));
}

}
#endif /* tuscany_xml_hpp */
