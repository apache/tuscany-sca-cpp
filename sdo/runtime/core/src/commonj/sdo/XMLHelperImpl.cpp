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

/* $Rev$ $Date: 2006/02/01 16:24:13 $ */

#include "commonj/sdo/SDOXMLFileWriter.h"   // Include first to avoid libxml compile problems!
#include "commonj/sdo/SDOXMLStreamWriter.h" // Include first to avoid libxml compile problems!
#include "commonj/sdo/SDOXMLBufferWriter.h" // Include first to avoid libxml compile problems!
#include "commonj/sdo/XMLHelperImpl.h"
#include "commonj/sdo/XMLDocumentImpl.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "commonj/sdo/SDOSAX2Parser.h"
#include "commonj/sdo/XSDPropertyInfo.h"
#include "commonj/sdo/XSDTypeInfo.h"
#include "commonj/sdo/SDORuntimeException.h"
#include "commonj/sdo/DataFactoryImpl.h"


namespace commonj
{
    namespace sdo
    {
        
        
        //////////////////////////////////////////////////////////////////////
        // Construction/Destruction
        //////////////////////////////////////////////////////////////////////
        
        XMLHelperImpl::XMLHelperImpl(DataFactoryPtr df)
        {
            dataFactory = (DataFactory*)df;
        }
        
        XMLHelperImpl::~XMLHelperImpl()
        {
            clearErrors();
        }

        DataFactoryPtr XMLHelperImpl::getDataFactory()
        {
            if (!dataFactory) 
            {
                dataFactory = DataFactory::getDataFactory();
            }
            return dataFactory;
        }

        XMLDocumentPtr XMLHelperImpl::createDocument(DataObjectPtr dataObject)
        {
            
            SDOXMLString rootElementName = "";
            SDOXMLString rootElementURI = "";
            if (dataObject)
            {
                // Set the root element name to the name of the containment property
                // or null if there is no container
                   try
                {
                    DataObjectPtr cont = dataObject->getContainer();
                    if (cont != 0)
                    {
                        const Property& containmentProp = dataObject->getContainmentProperty();
                        rootElementName = containmentProp.getName();
                        rootElementURI = cont->getType().getURI();
                    }
                    else
                    {
                        DataFactory* df = dataFactory;
                        rootElementURI = dataObject->getType().getURI();
                        rootElementName = ((DataFactoryImpl*)df)->getRootElementName();
                    }
                }
                catch (SDOPropertyNotFoundException&)
                {}
            }

            return new XMLDocumentImpl(dataObject, rootElementURI, rootElementName);
        }


        XMLDocumentPtr XMLHelperImpl::createDocument(const char* elementname,
                                                     const char* rootElementURI)
        {
            DataFactory* dp = (DataFactory*)getDataFactory();
            if (dp != 0)
            {
                const TypeImpl* rType = NULL;
                if (rootElementURI  != 0)
                {
                    rType = ((DataFactoryImpl*)dp)->findTypeImpl
                        (rootElementURI, "RootType");
                }
                else
                {
                    const TypeList& tl = dp->getTypes();
                    for (int i=0;i<tl.size();i++)
                    {
                        if (!strcmp("RootType",tl[i].getName()))
                        {
                            rType = ((DataFactoryImpl*)dp)->findTypeImpl
                                (tl[i].getURI(), "RootType");
                            break;
                        }
                    }
                }
                if (rType)
                {
                    if (elementname && strlen(elementname) != 0)
                    {
                        PropertyImpl* pl = rType->getPropertyImpl(elementname);
                        if (pl != 0)
                        {
                            const Type& tp = pl->getType();
                            DataObjectPtr dob = dp->create(tp);
                            return new XMLDocumentImpl(dob,
                                 tp.getURI(), /*tp.getName()*/ elementname);
                        }
                        else
                        {
                            string msg("createDocument - cannot find element ");
                            msg += elementname;
                            SDO_THROW_EXCEPTION("createDocument", SDOUnsupportedOperationException,
                            msg.c_str());
                        }
                    }
                    else
                    {
                        const Property& pl = rType->getProperty((unsigned int)0);
                        const Type& tp = pl.getType();
                        DataObjectPtr dob = dp->create(tp);
                           return new XMLDocumentImpl(dob,
                                  tp.getURI(), /*tp.getName()*/ pl.getName());
                    }
                }
                else
                {
                     string msg("createDocument - unable to find root type in namespace ");
                     if (rootElementURI != 0) 
                         msg += rootElementURI;
                     else
                         msg += " NULL";
                     
                     SDO_THROW_EXCEPTION("createDocument", SDOUnsupportedOperationException,
                     msg.c_str());
                }
            }
            return 0;
        }

        XMLDocumentPtr XMLHelperImpl::createDocument(
            DataObjectPtr dataObject,
            const char* rootElementURI,
            const char* rootElementName)
        {
            return new XMLDocumentImpl(dataObject, rootElementURI, rootElementName);
        }
        
        XMLDocumentPtr XMLHelperImpl::loadFile(
            const char* xmlFile,
            const char* targetNamespaceURI)
        {
            DataObjectPtr rootDataObject;
            clearErrors();
            SDOSAX2Parser sdoParser(getDataFactory(), targetNamespaceURI, rootDataObject,
                this);
            if (sdoParser.parse(xmlFile) == 0)
            {                
                return createDocument(rootDataObject);
            }
            return 0;
        }

        
        XMLDocumentPtr XMLHelperImpl::load(
            istream& inXml,
            const char* targetNamespaceURI)
        {
            DataObjectPtr rootDataObject;
            SDOSAX2Parser sdoParser(getDataFactory(), targetNamespaceURI, rootDataObject,
                this);
            clearErrors();
            inXml>>sdoParser;
            return createDocument(rootDataObject);
        }
        
        XMLDocumentPtr XMLHelperImpl::load(
            const char* inXml,
            const char* targetNamespaceURI)
        {
            istringstream str(inXml);
            return load(str, targetNamespaceURI);
        }
        
        void XMLHelperImpl::save(XMLDocumentPtr doc, const char* xmlFile, int indent)
        {
            SDOXMLFileWriter writer(xmlFile, dataFactory);
            writer.write(doc, indent);
        }
        
        void XMLHelperImpl::save(
            DataObjectPtr dataObject,
            const char* rootElementURI,
            const char* rootElementName,
            const char* xmlFile,
            int indent)
        {
            save(createDocument(dataObject,rootElementURI, rootElementName), xmlFile,
                indent);
        }
        
        
        // Serializes the datagraph to a stream
        void XMLHelperImpl::save(XMLDocumentPtr doc, std::ostream& outXml,
            int indent)
        {
            SDOXMLStreamWriter writer(outXml, dataFactory);
            writer.write(doc, indent);                
        }
        void XMLHelperImpl::save(
            DataObjectPtr dataObject,
            const char* rootElementURI,
            const char* rootElementName,
            std::ostream& outXml,
            int indent )
        {
            save(createDocument(dataObject,rootElementURI, rootElementName), outXml, indent);
        }
        
        // Serializes the datagraph to a string
        char* XMLHelperImpl::save(XMLDocumentPtr doc,
            int indent)
        {
            SDOXMLBufferWriter writer(dataFactory);
            writer.write(doc, indent);
            SDOXMLString ret = writer.getBuffer();
            char* retString = new char[strlen(ret) +1];
            strcpy(retString, ret);
            return retString;
        }
        char* XMLHelperImpl::save(
            DataObjectPtr dataObject,
            const char* rootElementURI,
            const char* rootElementName,
            int indent)
        {
            return save(createDocument(dataObject,rootElementURI, rootElementName),
                indent);
        }

        int XMLHelperImpl::getErrorCount() const
        {
            return parseErrors.size();
        }


        const char* XMLHelperImpl::getErrorMessage(int errnum) const
        {
            if (errnum >= 0 && errnum < parseErrors.size())
            {
                return parseErrors[errnum];
            }
            return 0;
        }

        void XMLHelperImpl::setError(const char* message)
        {
            if (message == 0) return;
            char * m = new char[strlen(message) + 1];
            strcpy(m,message);
            m[strlen(message)] = 0;
            parseErrors.push_back(m);
        }

        void XMLHelperImpl::clearErrors()
        {
            while (!parseErrors.empty()) 
            {
                if (*parseErrors.begin() != 0)
                {
                    delete (char*)(*parseErrors.begin());
                }
                parseErrors.erase(parseErrors.begin());
            }
        }
        
    } // End - namespace sdo
} // End - namespace commonj
