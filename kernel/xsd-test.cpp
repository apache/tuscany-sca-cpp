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
 * Test validation of a composite file against an SCDL schema.
 */

#include "string.hpp"
#include "fstream.hpp"
#include <libxml/xmlreader.h>
#include <libxml/xmlschemas.h>

namespace tuscany {

bool printNode(xmlTextReaderPtr reader) {
    const xmlChar* name = xmlTextReaderConstName(reader);
    if(name == NULL)
        name = (xmlChar *)"<unknown>";
    const xmlChar* value = xmlTextReaderConstValue(reader);
    cout << xmlTextReaderDepth(reader) << " " << xmlTextReaderNodeType(reader) << " " << name << " "
            << xmlTextReaderIsEmptyElement(reader) << " " << xmlTextReaderHasValue(reader);
    if(value == NULL)
        cout << endl;
    else
        cout << value << endl;
    return true;
}

int xmlRead(void *context, char* buffer, int len) {
    return fread(buffer, 1, len, (FILE*)context);
}

int xmlClose(void *context) {
    fclose((FILE*)context);
    return 0;
}

bool readFile(const char*xsdfilename, const char *filename) {
    cout << "Loading schemas...\n";
    const xmlDocPtr xsddoc = xmlReadFile(xsdfilename, NULL, XML_PARSE_NONET);
    const xmlSchemaParserCtxtPtr xsdctx = xmlSchemaNewDocParserCtxt(xsddoc);
    const xmlSchemaPtr xsd = xmlSchemaParse(xsdctx);
    const xmlSchemaValidCtxtPtr validctx = xmlSchemaNewValidCtxt(xsd);

    cout << "Reading file...\n";
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        const xmlTextReaderPtr reader = xmlReaderForIO(xmlRead, xmlClose, file, filename, NULL, XML_PARSE_NONET);
        xmlTextReaderSetParserProp(reader, XML_PARSER_DEFAULTATTRS, 1);
        xmlTextReaderSetParserProp(reader, XML_PARSER_SUBST_ENTITIES, 1);

        if(reader != NULL) {
            xmlTextReaderSchemaValidateCtxt(reader, validctx, 0);

            int rc;
            while((rc = xmlTextReaderRead(reader)) == 1) {
                printNode(reader);
            }
            if(xmlTextReaderIsValid(reader) != 1)
                cout << "Could not validate document" << endl;
            xmlFreeTextReader(reader);
            if(rc != 0)
                cout << "Could not parse document" << endl;
        } else
            cout << "Could not create parser" << endl;
    } else
        cout << "Could not open document" << endl;

    xmlSchemaFreeValidCtxt(validctx);
    xmlSchemaFree(xsd);
    xmlSchemaFreeParserCtxt(xsdctx);

    return true;
}

}

int main(int argc, char **argv) {
    tuscany::cout << "Testing..." << tuscany::endl;
    if(argc != 3)
        return 1;

    tuscany::readFile(argv[1], argv[2]);

    xmlCleanupParser();

    tuscany::cout << "OK" << tuscany::endl;
    return 0;
}
