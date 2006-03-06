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

/* $Rev$ $Date: 2005/12/22 16:54:15 $ */

#ifndef _SAX2PARSER_H_
#define _SAX2PARSER_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/SDOXMLString.h"
#include "commonj/sdo/SAX2Namespaces.h"
#include "commonj/sdo/SAX2Attributes.h"
#include "commonj/sdo/ParserErrorSetter.h"



#include "sstream"
namespace commonj
{
    namespace sdo
    {
        
/**
 * SAX2Parser holds the methods which will be called back.
 * The implementation uses libxml, which provides callbacks for
 * errors, warnings , elements etc. These methods correspond to thos
 * callbacks.
 * Callbacks from libxml to these C methods are converted into calls
 * to the C++ equivalent (with some parameter manipulation
 * In the callback methods the void* ctx is a pointer to 'this' SAX2Parser
 */
        class SAX2Parser
        {
            
        public:
            
            SAX2Parser();
            
            SAX2Parser(ParserErrorSetter* insetter);

            virtual ~SAX2Parser();
            
            virtual int parse (const char* filename);
            
            virtual void startDocument();
            virtual void endDocument();

            virtual void startElementNs(
                const SDOXMLString& localname,
                const SDOXMLString& prefix,
                const SDOXMLString& URI,
                const SAX2Namespaces& namespaces,
                const SAX2Attributes& attributes);
            
            virtual void endElementNs(
                const SDOXMLString& localname,
                const SDOXMLString& prefix,
                const SDOXMLString& URI);
            
            virtual void characters(const SDOXMLString& chars);
            
            virtual void warning(const char* msg, va_list args);

            virtual void fatalError(const char* msg, va_list args);
            
            virtual void error(const char* msg, va_list args);

            virtual void stream(std::istream& input);
            
            friend std::istream& operator>>(std::istream& input, SAX2Parser& parser);
            
            ParserErrorSetter* setter;
            bool parserError;

            char messageBuffer[1024];

            virtual const char* getCurrentFile() const;
        private:

            char* currentFile;


        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_SAX2PARSER_H_
