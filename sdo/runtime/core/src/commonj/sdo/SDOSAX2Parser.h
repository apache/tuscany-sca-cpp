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

#ifndef _SDOSAX2PARSER_H_
#define _SDOSAX2PARSER_H_
#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/SAX2Parser.h"

#include "commonj/sdo/DataFactory.h"
#include "commonj/sdo/DataObject.h"
#include "commonj/sdo/SAX2Namespaces.h"
#include "commonj/sdo/PropertySetting.h"
#include "commonj/sdo/ChangeSummaryImpl.h"
#include "commonj/sdo/ChangeSummaryBuilder.h"


#include <stack>

namespace commonj
{
    namespace sdo
    {


/**
 * SDOSAX2Parser implements SAX2Parser.
 * This class gets called back by the libxml library, and
 * deals with the creation of data objects from XML.
 */

        class SDOSAX2Parser : public SAX2Parser
        {

            
        public:
            

            SDOSAX2Parser(
                DataFactoryPtr df,
                const SDOXMLString& targetNamespaceURI,
                DataObjectPtr& rootDO,
                ParserErrorSetter* insetter);
            
            virtual ~SDOSAX2Parser();

            virtual void startDocument();
            virtual void endDocument();
            
            virtual bool setDO(DataObjectPtr newDO, 
                         SDOXMLString& propertyName);

            virtual void setAttributes(
                SDOXMLString& tns,
                const SAX2Namespaces& namespaces,
                const SAX2Attributes& attributes);

            virtual void  handleOpenAttribute(
                                SDOXMLString& tns,
                                const char* propuri,
                                const char* propname,
                                const char* value);

            virtual const PropertyImpl* handleOpenType(
                        SDOXMLString& tns,
                        const SDOXMLString& localname,
                        DataObjectImpl* dob,
                        const SAX2Namespaces& namespaces,
                        const SAX2Attributes& attributes,
                        SDOXMLString& xsitypeURI,
                        SDOXMLString& xsitypeName,
                        bool bToBeNull);

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

            friend std::istream& operator>>(std::istream& input, SDOSAX2Parser& parser);
            friend std::istringstream& operator>>(std::istringstream& input, SDOSAX2Parser& parser);

            
        private:
            SDOXMLString targetNamespaceURI;
            DataFactoryPtr dataFactory;
            DataObjectPtr&  rootDataObject;

            
            std::stack<DataObjectPtr>    dataObjectStack;
            
            DataObjectPtr    currentDataObject;
            bool            isDataGraph;
            
            void            setCurrentDataObject(DataObjectPtr currentDO);
            const Type*        currentDataObjectType;
            const SDOXMLString& getSDOName(const Type& type, const SDOXMLString& localName);

            PropertySetting currentPropertySetting;

            void reset();

            bool setNamespaces;
            SAX2Namespaces documentNamespaces;

            bool changeSummary;
            DataObjectPtr changeSummaryDO;
            bool changeSummaryLogging;
            bool dealingWithChangeSummary;
            ChangeSummaryBuilder* csbuilder;

            bool ignoreEvents;
            struct ignoretag
            {
                SDOXMLString localname;
                SDOXMLString uri;
                SDOXMLString prefix;
                int             tagCount;
            } ignoreTag;


            typedef std::map<SDOXMLString, DataObjectPtr> ID_MAP;
            ID_MAP IDMap;

            class IDRef
            {
            public:
                IDRef(DataObjectPtr dataobj,
                const SDOXMLString& prop,
                const SDOXMLString& val)
                : dataObject(dataobj), property(prop), value(val)
                {}

                DataObjectPtr dataObject;
                SDOXMLString property;
                SDOXMLString value;
            };

            typedef std::list<IDRef> ID_REFS;
            ID_REFS IDRefs;                    
        };
    } // End - namespace sdo
} // End - namespace commonj

#endif //_SDOSAX2PARSER_H_
