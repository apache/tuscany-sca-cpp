/*
 *
 *  Copyright 2005 The Apache Software Foundation or its licensors, as applicable.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/* $Rev$ $Date: 2006/04/18 12:33:33 $ */

#include "commonj/sdo/SAX2Parser.h"
#include "libxml/SAX2.h"
#include "commonj/sdo/SDORuntimeException.h"
using namespace commonj::sdo;

/**
 * SAX2Parser holds the methods which will be called back.
 * The implementation uses libxml, which provides callbacks for
 * errors, warnings , elements etc. These methods correspond to thos
 * callbacks.
 * Callbacks from libxml to these C methods are converted into calls
 * to the C++ equivalent (with some parameter manipulation
 * In the callback methods the void* ctx is a pointer to 'this' SAX2Parser
 */



int sdo_isStandalone(void *ctx)
{
    return 0;
}


int sdo_hasInternalSubset(void *ctx)
{
    return(0);
}

int sdo_hasExternalSubset(void *ctx)
{
    return(0);
}

void sdo_internalSubset(void *ctx, const xmlChar *name,
                        const xmlChar *ExternalID, const xmlChar *SystemID)
{
}


void sdo_externalSubset(void *ctx, const xmlChar *name,
                        const xmlChar *ExternalID, const xmlChar *SystemID)
{
}

xmlParserInputPtr sdo_resolveEntity(void *ctx, const xmlChar *publicId, const xmlChar *systemId)
{
    return(NULL);
}


xmlEntityPtr sdo_getEntity(void *ctx, const xmlChar *name)
{
    return(NULL);
}


xmlEntityPtr sdo_getParameterEntity(void *ctx, const xmlChar *name)
{
    return(NULL);
}


void sdo_entityDecl(void *ctx, const xmlChar *name, int type,
                    const xmlChar *publicId, const xmlChar *systemId, xmlChar *content)
{
}


void sdo_attributeDecl(void *ctx, const xmlChar * elem,
                       const xmlChar * name, int type, int def,
                       const xmlChar * defaultValue, xmlEnumerationPtr tree)
{
}

void sdo_elementDecl(void *ctx, const xmlChar *name, int type,
                     xmlElementContentPtr content)
{
}


void sdo_notationDecl(void *ctx, const xmlChar *name,
                      const xmlChar *publicId, const xmlChar *systemId)
{
}

void sdo_unparsedEntityDecl(void *ctx, const xmlChar *name,
                            const xmlChar *publicId, const xmlChar *systemId,
                            const xmlChar *notationName)
{
}


void sdo_setDocumentLocator(void *ctx, xmlSAXLocatorPtr loc)
{
}


void sdo_startDocument(void *ctx)
{    
    if (!((SAX2Parser*)ctx)->parserError)
        ((SAX2Parser*)ctx)->startDocument();
}


void sdo_endDocument(void *ctx)
{
    if (!((SAX2Parser*)ctx)->parserError)
        ((SAX2Parser*)ctx)->endDocument();
}


void sdo_startElement(void *ctx, const xmlChar *name, const xmlChar **atts)
{
    //    ((SAX2Parser*)ctx)->startElement(name, atts);
}


void sdo_endElement(void *ctx, const xmlChar *name)
{
    //    ((SAX2Parser*)ctx)->endElement(name);
}


void sdo_characters(void *ctx, const xmlChar *ch, int len)
{
    if (!((SAX2Parser*)ctx)->parserError)
        ((SAX2Parser*)ctx)->characters(SDOXMLString(ch, 0, len));
}


void sdo_reference(void *ctx, const xmlChar *name)
{
}


void sdo_ignorableWhitespace(void *ctx, const xmlChar *ch, int len)
{
}


void sdo_processingInstruction(void *ctx, const xmlChar *target,
                               const xmlChar *data)
{
}


void sdo_cdataBlock(void *ctx, const xmlChar *value, int len)
{
}

void sdo_comment(void *ctx, const xmlChar *value)
{
}


void sdo_warning(void *ctx, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    ((SAX2Parser*)ctx)->warning(msg, args);
    va_end(args);
}

void sdo_error(void *ctx, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    ((SAX2Parser*)ctx)->error(msg, args);
    va_end(args);
    
    
}

void sdo_fatalError(void *ctx, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    ((SAX2Parser*)ctx)->fatalError(msg, args);
    va_end(args);
}



// ===============
// SAX2 callbacks
// ===============
void sdo_startElementNs(void *ctx,
                        const xmlChar *localname,
                        const xmlChar *prefix,
                        const xmlChar *URI,
                        int nb_namespaces,
                        const xmlChar **namespaces,
                        int nb_attributes,
                        int nb_defaulted,
                        const xmlChar **attributes)
{
    if (!((SAX2Parser*)ctx)->parserError)
        ((SAX2Parser*)ctx)->startElementNs(
        localname,
        prefix,
        URI,
        SAX2Namespaces(nb_namespaces, namespaces),
        SAX2Attributes(nb_attributes, nb_defaulted, attributes));
}


void sdo_endElementNs(void *ctx,
                      const xmlChar *localname,
                      const xmlChar *prefix,
                      const xmlChar *URI)
{
    if (!((SAX2Parser*)ctx)->parserError)
        ((SAX2Parser*)ctx)->endElementNs(localname, prefix, URI);
}


// The callback method structure
xmlSAXHandler SDOSAX2HandlerStruct = {
    sdo_internalSubset,
        sdo_isStandalone,
        sdo_hasInternalSubset,
        sdo_hasExternalSubset,
        sdo_resolveEntity,
        sdo_getEntity,
        sdo_entityDecl,
        sdo_notationDecl,
        sdo_attributeDecl,
        sdo_elementDecl,
        sdo_unparsedEntityDecl,
        sdo_setDocumentLocator,
        sdo_startDocument,
        sdo_endDocument,
        sdo_startElement,
        sdo_endElement,
        sdo_reference,
        sdo_characters,
        sdo_ignorableWhitespace,
        sdo_processingInstruction,
        sdo_comment,
        sdo_warning,
        sdo_error,
        sdo_fatalError,
        sdo_getParameterEntity,
        sdo_cdataBlock,
        sdo_externalSubset,
        XML_SAX2_MAGIC,
        NULL,
        sdo_startElementNs,
        sdo_endElementNs,
        NULL
};


namespace commonj
{
    namespace sdo
    {
        
        
        
        
        SAX2Parser::SAX2Parser()
        {
            setter = 0;
            parserError = false;
            currentFile = 0;
        }

        SAX2Parser::SAX2Parser(ParserErrorSetter* insetter)
        {
            setter = insetter;
            parserError = false;
            currentFile = 0;
        }
        
        SAX2Parser::~SAX2Parser()
        {
            // xmlCleanupParser();
            if (currentFile != 0)delete currentFile;
            
        }
        
        const char* SAX2Parser::getCurrentFile() const
        {
            return currentFile;
        }


        void SAX2Parser::setCurrentFile(const char* filename)
        {
            if (currentFile != 0)
            {
                delete currentFile;
            }
            currentFile = new char[strlen(filename)+1];
            strcpy(currentFile,filename);
        }

         int SAX2Parser::parse(const char* filename)
        {
            
            parserError = false;
            xmlSAXHandlerPtr handler = &SDOSAX2HandlerStruct;

            if (currentFile != 0)
            {
                delete currentFile;
            }
            currentFile = new char[strlen(filename)+1];
            strcpy(currentFile,filename);


            int rc = xmlSAXUserParseFile(handler, this, filename);
            if (rc == -1)
            {
                sdo_error(this, "xmlSAXUserParseFile returned an error %d", rc);
                SDO_THROW_EXCEPTION("parse", SDOFileNotFoundException,messageBuffer);
            }
            return rc;
        }

        void SAX2Parser::startDocument()
        {
        }

        void SAX2Parser::endDocument()
        {
        }
        
        void SAX2Parser::startElementNs(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI,
            const SAX2Namespaces& namespaces,
            const SAX2Attributes& attributes)
        {
        }
        
        void SAX2Parser::endElementNs(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI)
        {
        }
                    
        void SAX2Parser::characters(const SDOXMLString& chars)
        {
        }
                    
        
        void SAX2Parser::warning(const char* msg, va_list args)
        {
            // warnings are perhaps too frequent
        //    vsprintf(messageBuffer, msg, args);
        //    if (setter != 0)setter->setError(messageBuffer);
        }

        void SAX2Parser::fatalError(const char* msg, va_list args)
        {
            parserError = true;
            vsprintf(messageBuffer, msg, args);
            if (setter != 0)setter->setError(messageBuffer);
        }
        
        void SAX2Parser::error(const char* msg, va_list args)
        {
            vsprintf(messageBuffer, msg, args);
            if (setter != 0)setter->setError(messageBuffer);
        }

 
        void SAX2Parser::stream(std::istream& input)
        {
            char buffer[100];
            xmlSAXHandlerPtr handler = &SDOSAX2HandlerStruct;
            parserError = false;
            xmlParserCtxtPtr ctxt;

            input.read(buffer,4);
            ctxt = xmlCreatePushParserCtxt(handler, this,
                buffer, input.gcount(), NULL);
            
            while (input.read(buffer,100))
            {
                xmlParseChunk(ctxt, buffer, input.gcount(), 0);
                
            }
            
            xmlParseChunk(ctxt, buffer, input.gcount(), 1);
            xmlFreeParserCtxt(ctxt);

            if (parserError)
            {
               SDO_THROW_EXCEPTION("stream", SDOXMLParserException,messageBuffer);
            }
            
        }

        int SAX2Parser::parse_twice(const char* filename)
        {
            parserError = false;
            xmlSAXHandlerPtr handler = &SDOSAX2HandlerStruct;

            setCurrentFile(filename);

            int rc = xmlSAXUserParseFile(handler, this, filename);
            if (rc == -1)
            {
                sdo_error(this, "xmlSAXUserParseFile returned an error %d", rc);
                SDO_THROW_EXCEPTION("parse", SDOFileNotFoundException,messageBuffer);
            }

            // parse twice - first was for groups

            if (setter)setter->clearErrors();

            rc = xmlSAXUserParseFile(handler, this, filename);
            if (rc == -1)
            {
                sdo_error(this, "xmlSAXUserParseFile returned an error %d", rc);
                SDO_THROW_EXCEPTION("parse", SDOFileNotFoundException,messageBuffer);
            }
            return rc;
        }

        void SAX2Parser::stream_twice(std::istream& input)
        {

            std::vector<parse_buf_element> buffer_vec;
            int count = 0;
            parserError = false;

            xmlSAXHandlerPtr handler = &SDOSAX2HandlerStruct;
            xmlParserCtxtPtr ctxt;

            char bctx[5];
            input.read(bctx,4);
            int bcount = input.gcount();


            ctxt = xmlCreatePushParserCtxt(handler, this,
                bctx, bcount, NULL);

            buffer_vec.push_back(parse_buf_element());

            while (input.read(buffer_vec[count].buf,1000))
            {
                buffer_vec[count].len = input.gcount();
                xmlParseChunk(ctxt, buffer_vec[count].buf, 
                                    buffer_vec[count].len, 0);
                buffer_vec.push_back(parse_buf_element());
                count++;
                
            }
            
            buffer_vec[count].len = input.gcount();
            xmlParseChunk(ctxt, buffer_vec[count].buf,
                                buffer_vec[count].len, 1);
            xmlFreeParserCtxt(ctxt);

            if (parserError)
            {
               SDO_THROW_EXCEPTION("stream", SDOXMLParserException,messageBuffer);
            }
            
            if (setter)setter->clearErrors();

            ctxt = xmlCreatePushParserCtxt(handler, this,
                bctx, bcount, NULL);
            
            for (int i=0;i<buffer_vec.size();i++)
            {
                if (buffer_vec[i].len > 0)
                {
                    xmlParseChunk(ctxt, buffer_vec[i].buf, 
                                    buffer_vec[i].len, 0);
                }
            }

            if (parserError)
            {
               SDO_THROW_EXCEPTION("stream", SDOXMLParserException,messageBuffer);
            }
            
        }


        std::istream& operator>>(std::istream& input, SAX2Parser& parser)
        {
            parser.stream(input);                            
            return input;
        }
        
        
    } // End - namespace sdo
} // End - namespace commonj

