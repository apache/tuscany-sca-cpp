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

#include "commonj/sdo/SDOXMLWriter.h"
#include "commonj/sdo/SDOXMLString.h"
#include "commonj/sdo/SDOString.h"
#include "iostream"
using namespace::std;
#include "commonj/sdo/DASProperty.h"
#include "commonj/sdo/XSDPropertyInfo.h"
#include "commonj/sdo/XSDTypeInfo.h"
#include "commonj/sdo/ChangeSummary.h"
#include "commonj/sdo/Sequence.h"
#include "commonj/sdo/SDORuntimeException.h"
#include "commonj/sdo/XMLQName.h"
#include "commonj/sdo/DataObjectImpl.h"

namespace commonj
{
    namespace sdo
    {
        
        
        
        
        SDOXMLWriter::SDOXMLWriter(
            DataFactoryPtr dataFact)
            : dataFactory(dataFact)
        {
            
        }
        
        SDOXMLWriter::~SDOXMLWriter()
        {
            freeWriter();
        }
        
        void SDOXMLWriter::setWriter(xmlTextWriterPtr textWriter)
        {
            writer = textWriter;
        }
        
        void SDOXMLWriter::freeWriter()
        {
            if (writer != NULL)
            {
                xmlFreeTextWriter(writer);
                writer = NULL;
            }
        }
        
        int SDOXMLWriter::write(XMLDocumentPtr doc, int indent)
        {
            if (!doc)
            {
                return 0;
            }
            
            if (writer == NULL)
            {
                // Throw exception
                return -1;
            }
            
            int rc = 0;
            
            //namespaceUriStack.empty();
            //namespaceUriStack.push(SDOXMLString());
            //namespaces.empty();
            //namespaceStack.push(namespaces);
            namespaceMap.empty();
            
            if (indent >= 0)
            {
                xmlTextWriterSetIndent(writer, 1);
                if (indent > 0)
                {
                    char * chars = new char[indent+1];
                    for (int i=0;i<indent;i++)chars[i] = ' ';
                    chars[indent] = 0;
                    xmlTextWriterSetIndentString(writer, SDOXMLString(chars));
                    delete chars;
                }
                else
                {
                    xmlTextWriterSetIndentString(writer, SDOXMLString(""));
                }
            }
            
            if (doc->getXMLDeclaration())
            {
                rc = xmlTextWriterStartDocument(writer, doc->getXMLVersion(), doc->getEncoding(), NULL);
                if (rc < 0) {
                    SDO_THROW_EXCEPTION("write", SDOXMLParserException, "xmlTextWriterStartDocument failed");
                }
            }
            
            DataObjectPtr root = doc->getRootDataObject();
            if (root)
            {
                const Type& rootType = root->getType();
                SDOXMLString rootTypeURI = rootType.getURI();
                SDOXMLString rootTypeName = rootType.getName();
                
                // For the root DataObject we need to determine the element name
                SDOXMLString elementURI = doc->getRootElementURI();
                if (elementURI.isNull() || elementURI.equals(""))
                {
                    elementURI = rootTypeURI;
                }
                SDOXMLString elementName = doc->getRootElementName();
                if (elementName.isNull() || elementName.equals(""))
                {
                    elementName = rootTypeName;
                    elementName = elementName.toLower(0,1);
                }
                
                // If the element name is defined as a global element then we
                // can supress the writing of xsi:type according to the spec
                bool writeXSIType = true;

                try
                {
                    // Locate the RootType
                    const Type& rootTy = dataFactory->getType(elementURI, "RootType");
                    // Does a property exist with the given element name?
                    const Property& rootProp = rootTy.getProperty((const char*)elementName);
                    // Is this property of the correct Type?
                    const Type& rootPropType = rootProp.getType();
                    if (rootTypeURI == (SDOXMLString)rootPropType.getURI()
                        && rootTypeName == (SDOXMLString)rootPropType.getName())
                    {
                        writeXSIType = false;
                    }
                }
                catch(SDORuntimeException&)
                {
                }
                
                // Supress the writing of xsi:type as well for DataObjects of type
                // commonj.sdo#OpenDataObject
                if (writeXSIType &&
                    rootTypeURI.equals("commonj.sdo") && rootTypeName.equals("OpenDataObject"))
                {
                    writeXSIType = false;
                }

                writeDO(root, elementURI, elementName, writeXSIType, true);
            }
            rc = xmlTextWriterEndDocument(writer);
            if (rc < 0) {
                    SDO_THROW_EXCEPTION("write", SDOXMLParserException, "xmlTextWriterEndDocument failed");
                return rc;
            }
            
            xmlTextWriterFlush(writer);
            freeWriter();
            
            return rc;
        }

        //////////////////////////////////////////////////////////////////////////
        // Write Change Summary attributes
        //////////////////////////////////////////////////////////////////////////

        void SDOXMLWriter::handleChangeSummaryAttributes(
            ChangeSummaryPtr cs, 
            DataObjectPtr dol)
        {
            int rc;

            SettingList& sl = cs->getOldValues(dol);
            if (sl.size() == 0) 
            {
                // no attributes
                return;
            }
            for (int j=0;j< sl.size(); j++)
            {
                try {

                    if (sl.get(j)->getProperty().isMany()) 
                    {
                        // manys are elements
                        continue;
                    }
        
                    if (sl.get(j)->getProperty().getType().isDataType())
                    {
                        // data types are OK
                        rc = xmlTextWriterWriteAttribute(writer, 
                            SDOXMLString(sl.get(j)->getProperty().getName()),
                            SDOXMLString(sl.get(j)->getCStringValue()));
                    }
                    else 
                    {
                        DataObjectPtr dob = sl.get(j)->getDataObjectValue();
                        if (dob) 
                        {
                            if (cs->isDeleted(dob))
                            {
                            rc = xmlTextWriterWriteAttribute(writer, 
                                SDOXMLString(sl.get(j)->getProperty().getName()),
                                SDOXMLString(cs->getOldXpath(dob)));
                            }
                            else 
                            {
                            rc = xmlTextWriterWriteAttribute(writer, 
                                SDOXMLString(sl.get(j)->getProperty().getName()),
                                SDOXMLString(dob->objectToXPath()));
                            }
                        }
                        else
                        {
                            rc = xmlTextWriterWriteAttribute(writer, 
                                SDOXMLString(sl.get(j)->getProperty().getName()),
                                SDOXMLString(""));
                        }
                    }
                }
                catch (SDORuntimeException e)
                {
                    // ignore this attribute
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////
        // Write  Change Summary elements
        //////////////////////////////////////////////////////////////////////////
        
        void SDOXMLWriter::handleChangeSummaryElements(
            ChangeSummaryPtr cs, 
            DataObjectPtr dob)
        {
            int rc;

            SettingList& sl = cs->getOldValues(dob);
        
            if (sl.size() == 0) 
            {
            // there are no setting for this element.
            return;
            }
            
            for (int j=0;j< sl.size(); j++)
            {
                try 
                {

                    // single values will have been covered by the attributes.
                    if (!sl.get(j)->getProperty().isMany()) continue;
        
                    if (sl.get(j)->getProperty().getType().isDataType())
                    {

                        rc = xmlTextWriterWriteElement(
                            writer,
                            SDOXMLString(sl.get(j)->getProperty().getName()),
                            SDOXMLString(sl.get(j)->getCStringValue()));
                            
                    } // if datatype
                    else
                    {
                        DataObjectPtr dob2 = sl.get(j)->getDataObjectValue();
                        if (!dob2) 
                        {
                            continue;
                        }
                        if (cs->isDeleted(dob2))
                        {
                            handleChangeSummaryDeletedObject(sl.get(j)->getProperty().getName(), cs,dob2);
                        }
                        else
                        {
                            rc = xmlTextWriterStartElement(
                                writer,
                                SDOXMLString(sl.get(j)->getProperty().getName()));
                            rc = xmlTextWriterWriteAttribute(
                                writer,
                                SDOXMLString("sdo:ref"),
                                SDOXMLString(dob2->objectToXPath()));
                            rc = xmlTextWriterEndElement(
                                writer);
                        }
                    } 
                }
                catch (SDORuntimeException e)
                {
                    // ignore this element
                }
            } // for
        } 


        //////////////////////////////////////////////////////////////////////////
        // Write a deleted object and all its props
        //////////////////////////////////////////////////////////////////////////

        void SDOXMLWriter::handleChangeSummaryDeletedObject(
            const char* name, 
            ChangeSummaryPtr cs, 
            DataObjectPtr dob)
        {
        
            int rc, k; // TODO error handling
        
            SettingList& sl = cs->getOldValues(dob);
        
            rc = xmlTextWriterStartElement(
                writer,
                SDOXMLString(name));

            if (sl.size() == 0) 
            {
                rc = xmlTextWriterWriteAttribute(writer, 
                    SDOXMLString("sdo:ref"),
                    SDOXMLString(cs->getOldXpath(dob)));
                rc = xmlTextWriterEndElement(writer);
                return;
            }

        
            try 
            {
                // print single valued datatypes as attributes
        
                for (int j=0;j< sl.size(); j++)
                {
                    //if (!sl.get(j)->isSet()) 
                    //{
                    //    // unset properties dont need recording - ah but they do!
                    //
                    //    continue;
                    //}
                    if ( sl.get(j)->getProperty().isMany()) 
                    {
                        // manys are elements
                        continue;
                    }
                    if (!sl.get(j)->getProperty().getType().isDataType())
                    {
                        // data objects are element in a deleted data object.
                        continue;
                    }

                    rc = xmlTextWriterWriteAttribute(writer, 
                        SDOXMLString(sl.get(j)->getProperty().getName()),
                        SDOXMLString(sl.get(j)->getCStringValue()));

                } // for attributes
        
    
                // now we are onto the many-values, 
                // and dataobject single values.
                // 
                // handle deletions within deletions in reverse order, so they match the
                // deletion records above.

                for (k=sl.size()-1;k>=0; k--)
                {

                     if ( !sl.get(k)->getProperty().getType().isDataType() &&
                          sl.get(k)->getProperty().isMany()) 
                    {
                        // its a dataobject type
                        DataObjectPtr dob2 = sl.get(k)->getDataObjectValue();
                        if (!dob2) continue;
                        if (!cs->isDeleted(dob2)) continue;
                        handleChangeSummaryDeletedObject(sl.get(k)->
                                   getProperty().getName(),cs,dob2);
                    }
                } // for attributes

                for (k=0;k< sl.size(); k++)
                {

                     if ( !sl.get(k)->getProperty().getType().isDataType())
                    {
                        if (sl.get(k)->getProperty().isMany()) continue; 
                        // its a single valued dataobject type

                        DataObjectPtr dob2 = sl.get(k)->getDataObjectValue();
                        if (!dob2) continue;
                        if (!cs->isDeleted(dob2)) continue;
                        handleChangeSummaryDeletedObject(sl.get(k)->
                                   getProperty().getName(),cs,dob2);

                    }
                    else 
                    {
                        if ( !sl.get(k)->getProperty().isMany()) continue; 
                        
                        // could only be many valued data type
        
                        rc = xmlTextWriterWriteElement(writer, 
                            SDOXMLString(sl.get(k)->getProperty().getName()),
                            SDOXMLString(sl.get(k)->getCStringValue()));
                    }
                } // for attributes
            }
            catch (SDORuntimeException e)
            {
                 // ignore - and write the end-element
            }

            rc = xmlTextWriterEndElement(writer);
        } 


        //////////////////////////////////////////////////////////////////////////
        // Write the list of elements of a change summary
        //////////////////////////////////////////////////////////////////////////

        void SDOXMLWriter::handleSummaryChange(
            const SDOXMLString& elementName, 
            ChangeSummaryPtr cs, 
            DataObjectPtr dob)
        {
            int rc; 
            DataObject* temp = dob;
            const char* name;
            try 
            {
                name = temp->getContainmentProperty().getName();
            }
            catch (SDORuntimeException e)
            {
                // This could be a root, and have no name.
                name = 0;
            }            

            if (name == 0) 
            {
            rc = xmlTextWriterStartElement(
                writer,
                elementName);
            }
            else
            {
            rc = xmlTextWriterStartElement(
                writer,
                SDOXMLString(name));
            }

            if (rc != 0)
            {
                // failed to write an element
                return;
            }

            try 
            {
                name =  temp->objectToXPath();
            }
            catch (SDORuntimeException e)
            {
                name = 0;
            }

            rc = xmlTextWriterWriteAttribute(writer, 
                SDOXMLString("sdo:ref"),
                SDOXMLString(name));

            handleChangeSummaryAttributes(cs, temp);

            handleChangeSummaryElements(cs, temp);

            rc = xmlTextWriterEndElement(writer);

        }

        //////////////////////////////////////////////////////////////////////////
        // Write a Change Summary
        //////////////////////////////////////////////////////////////////////////

        void SDOXMLWriter::handleChangeSummary(
            const SDOXMLString& elementName,
            ChangeSummaryPtr cs)
        {
            int i;
            int rc; 

            ChangedDataObjectList& changedDOs =  cs->getChangedDataObjects();
            rc = xmlTextWriterStartElementNS(writer,
                    SDOXMLString("sdo"), SDOXMLString("changeSummary"), SDOXMLString(Type::SDOTypeNamespaceURI.c_str()));
            if (rc != 0) return;
            if (cs->isLogging())
            {
                rc = xmlTextWriterWriteAttribute(writer, 
                    SDOXMLString("logging"),
                    SDOXMLString("true"));
            }

            if (changedDOs.size() > 0)
            {

                // write the creates/deletes in the order they
                // happened, as elements.

                for (i=0;i< changedDOs.size();i++)
                {
                    if  (cs->isCreated(changedDOs[i])
                        && changedDOs.getType(i) == ChangedDataObjectList::Create) 
                    {
                        // TODO - should work out if theres a IDREF here
                        // TODO - can we have more than one create like this?
                        try
                        {
                            rc = xmlTextWriterWriteElement(writer, 
                            SDOXMLString("create"),
                            SDOXMLString(changedDOs[i]->objectToXPath()));
                        }
                        catch (SDORuntimeException e)
                        {
                                // The object was not in our tree - we ignore it.
                        }
                    }
                    if  (cs->isDeleted(changedDOs[i])
                        && changedDOs.getType(i) == ChangedDataObjectList::Delete) 
                    {
                        // TODO - should work out if theres a IDREF here
                        try 
                        {
                            rc = xmlTextWriterWriteElement(writer, 
                            SDOXMLString("delete"),
                            SDOXMLString(cs->getOldXpath(changedDOs[i])));
                        }
                        catch (SDORuntimeException e)
                        {
                            // The object was not in the deleted list - we ignore it.
                        }
                    }
                }

            
                for (i=0;i< changedDOs.size();i++)
                {
                    if (cs->isModified(changedDOs[i]))
                    {
                        handleSummaryChange(elementName, cs, changedDOs[i]);
                    }
                }
                        
            }
            rc = xmlTextWriterEndElement(writer);
        }
        
        //////////////////////////////////////////////////////////////////////////
        // Add to namespaces
        //////////////////////////////////////////////////////////////////////////
        
        void  SDOXMLWriter::addToNamespaces(DataObjectImpl* dob)
        {
            std::map<SDOXMLString,SDOXMLString>::iterator it;
            SDOXMLString uri = dob->getType().getURI();
    
            it = namespaceMap.find(uri);
            if (it == namespaceMap.end())
            {
                char buf[20];
                sprintf(buf,"%d",++spacescount);
                SDOXMLString s = SDOXMLString("tns") + buf;
                namespaceMap.insert(make_pair(uri,s));
            }

            PropertyList pl = dob->getInstanceProperties();
            for (int i = 0; i < pl.size(); i++)
            {
                if (!dob->isSet(pl[i]))continue;

                if  (pl[i].isMany())
                {
                    if (!pl[i].getType().isDataType())
                    {
                        DataObjectList& dl = dob->getList(pl[i]);
                        for (int k=0;k< dl.size() ;k++)
                        {
                            DataObjectImpl* d = (DataObjectImpl*)(DataObject*)dl[k];
                            if (d != 0)addToNamespaces(d);
                        }
                    }
                }
                else
                {
                    if (!pl[i].getType().isDataType())
                    {
                        DataObjectImpl* d = (DataObjectImpl*)(DataObject*)dob->getDataObject(pl[i]);
                        if (d != 0)addToNamespaces(d);
                    }
                    else
                    {                    
                        XSDPropertyInfo* pi = getPropertyInfo(dob->getType(), pl[i]);
                        if (pi)
                        {
                            PropertyDefinitionImpl propdef;
                            propdef = pi->getPropertyDefinition();
                            if (propdef.isElement)continue;
                            if (!propdef.isQName)continue;
                 
                            SDOXMLString propertyValue = (dob->getCString(pl[i]));
                            XMLQName qname(propertyValue);
                            
                            it = namespaceMap.find(qname.getURI());
                            if (it == namespaceMap.end())
                            {
                                char buf[20];
                                sprintf(buf,"%d",++spacescount);
                                SDOXMLString s = SDOXMLString("tns") + buf;
                                namespaceMap.insert(make_pair(qname.getURI(),s));
                            }
                        }
                    }
                }
            }
        }
            

        //////////////////////////////////////////////////////////////////////////
        // Write xmlns:xsi= if nexessary
        //////////////////////////////////////////////////////////////////////////
        void SDOXMLWriter::writeXmlnsXsi()
        {
            SDOXMLString xsins("http://www.w3.org/2001/XMLSchema-instance");

            std::map<SDOXMLString,SDOXMLString>::iterator it = namespaceMap.find(xsins);
            if (it == namespaceMap.end())
            {               
                SDOXMLString prefix("xmlns:xsi");
                xmlTextWriterWriteAttribute(writer,
                prefix, xsins);
            }
        }
        
        //////////////////////////////////////////////////////////////////////////
        // Write a DatObject tree
        //////////////////////////////////////////////////////////////////////////
        
        int SDOXMLWriter::writeDO(
            DataObjectPtr dataObject,
            const SDOXMLString& elementURI,
            const SDOXMLString& elementName,
            bool writeXSIType,
            bool isRoot)
        {

            int rc;

            if (dataObject == 0)
                return 0;


            //SDOXMLString uri;
            //if (!elementURI.equals(namespaceUriStack.top()))
            //{
            //    uri = elementURI;
            //    namespaceUriStack.push(elementURI);
            //}

            const Type& dataObjectType = dataObject->getType();

            //////////////////////////////////////////////////////////////////////////
            // suppose its a primitive type - just write the value
            //////////////////////////////////////////////////////////////////////////
            if (dataObjectType.isDataType())
            {
                if (dataObject->isNull(""))
                {
                    rc = xmlTextWriterStartElementNS(writer, 
                        NULL, elementName, elementURI);
                    if (rc < 0) 
                    {
                        SDO_THROW_EXCEPTION("writeDO", 
                            SDOXMLParserException, 
                            "xmlTextWriterStartElementNS failed");
                    }                
                    rc = xmlTextWriterWriteAttribute(writer, 
                        (const unsigned char*)"xsi:nil", 
                        (const unsigned char*)"true");
                    writeXmlnsXsi();
                    rc = xmlTextWriterEndElement(writer);
                }
                else
                {
                    xmlTextWriterWriteElement(
                    writer,
                    elementName,
                    SDOXMLString(dataObject->getCString("")));
                }

                // need to pop stacks before returning
                //if (!uri.isNull())
                //{
                //    namespaceUriStack.pop();
                //}
                return 0;

            }
            

			//namespaceStack.push(namespaces);


            if (isRoot)
            {
                tnsURI=elementURI;
                if (elementURI.equals("")) {
                    rc = xmlTextWriterStartElementNS(writer, NULL, elementName, NULL);
                }
                else
                {
                    rc = xmlTextWriterStartElementNS(writer, NULL, elementName, elementURI);
                }
                if (rc < 0) {
                    SDO_THROW_EXCEPTION("writeDO", SDOXMLParserException, "xmlTextWriterStartElementNS failed");
                }
            }
            else
            {
                //xmlTextWriterWriteString(writer,SDOXMLString("\n"));

                SDOXMLString theName=elementName;

                if (!elementURI.isNull() && !elementURI.equals(tnsURI) && !elementURI.equals(""))
                {
                    std::map<SDOXMLString,SDOXMLString>::iterator it = namespaceMap.find(elementURI);
                    if (it != namespaceMap.end())
                    {
                        theName = (*it).second;
                        theName += ":";
                        theName += elementName;
                    }
                }

                rc = xmlTextWriterStartElementNS(writer, NULL, theName, NULL);
                if (rc < 0) {
                    SDO_THROW_EXCEPTION("writeDO", SDOXMLParserException, "xmlTextWriterStartElementNS failed");
                }   
            }
            

            if (writeXSIType)
            {
                rc = xmlTextWriterWriteAttributeNS(writer, 
                    SDOXMLString("xsi"), SDOXMLString("type"), 
                    NULL,
                    /*SDOXMLString("http://www.w3.org/2001/XMLSchema-instance"),*/ 
                    SDOXMLString(dataObject->getType().getName()));
                if (isRoot)
                {
                    namespaceMap.insert(make_pair(
                        SDOXMLString("http://www.w3.org/2001/XMLSchema-instance"),
                        SDOXMLString("xsi")));
 
                }
                else
                {  
                    writeXmlnsXsi();
                }
            }


            if (isRoot)
            {
                std::map<SDOXMLString,SDOXMLString>::iterator it = namespaceMap.find(elementURI);
                if (it == namespaceMap.end())
                {
                    SDOXMLString s = SDOXMLString("tns");
                    namespaceMap.insert(make_pair(elementURI,s));
                }
                DataObjectImpl* d = (DataObjectImpl*)(DataObject*)dataObject;
                spacescount = 1;
                addToNamespaces(d);
                
/////////////////////////////////////////////////////////////////////////////////////
//                // build the namespace map, and write the items out at the
//                // top of the tree.
//                int spacecount = 0;
//                DataObjectImpl* d = (DataObjectImpl*)(DataObject*)dataObject;
//                if (d != 0)
//                {
//                    TypeList types = (d->getDataFactory())->getTypes();
//                    std::map<SDOXMLString,SDOXMLString>::iterator it;
//            
//                    for (int i = 0; i<types.size(); i++)
//                    {
//                        SDOXMLString uri = types[i].getURI();
//                        if (uri.equals(Type::SDOTypeNamespaceURI)) continue;
//                        std::map<SDOXMLString,SDOXMLString>::iterator it = namespaceMap.find(uri);
//                        if (it == namespaceMap.end())
//                        {
//                            char buf[4];
//                            if (!elementURI.isNull()) 
//                            {
//                                if (elementURI.equals(uri))
//                                {
//                                   SDOXMLString s = SDOXMLString("tns");
//                                    namespaceMap.insert(make_pair(uri,s));
//                                }
//                                else
//                                {
//                                    sprintf(buf,"%d",++spacecount);
//                                    SDOXMLString s = SDOXMLString("tns") + buf;
//                                    namespaceMap.insert(make_pair(uri,s));
//                                }
//                            }
//                        }
//                    }
////////////////////////////////////////////////////////////////////////////////////
                    
                for (it = namespaceMap.begin();it != namespaceMap.end(); ++it)
                {
                    if ((*it).first.equals("")) continue;
                    SDOXMLString space = SDOXMLString("xmlns:") + (*it).second;
                    rc = xmlTextWriterWriteAttribute(writer,
                     space, (*it).first);
                }
            }


            //////////////////////////////////////////////////////////////////////////
            // write out the type if the xsi:type if the containing type is open
            // and the property is not one of the declared properties
             //////////////////////////////////////////////////////////////////////////
            if (!writeXSIType)
            {
                DataObject* dob = dataObject;
                DataObjectImpl* cont = 
                         ((DataObjectImpl*)dob)->getContainerImpl();
                if (cont != 0)
                {
                    if (cont->getType().isOpenType())
                    {
                        //if (dataObject->getType().getURI() != 0)
                        //{
                        //    std::string value = 
                        //        dataObject->getType().getURI();
                        //    value += ":";
                        //    value += dataObject->getType().getName();
                        //    rc = xmlTextWriterWriteAttribute(writer, 
                        //        (const unsigned char*)"xsi:type", 
                        //        (const unsigned char*)value.c_str());
                        //}
                        //else
                        //{
                        if (cont->getTypeImpl().getPropertyImpl(elementName) == 0)
                        {
                            rc = xmlTextWriterWriteAttribute(writer, 
                            (const unsigned char*)"xsi:type", 
                            (const unsigned char*)dataObject->getType().getName());

                            writeXmlnsXsi();
                        }
                    }
                }
            }

            // write nil if required
            if (dataObject->isNull(""))
            {
                rc = xmlTextWriterWriteAttribute(writer, 
                (const unsigned char*)"xsi:nil", 
                (const unsigned char*)"true");
                
                writeXmlnsXsi();
            }


            //////////////////////////////////////////////////////////////////////////
            // Iterate over all the properties to find attributes
            //////////////////////////////////////////////////////////////////////////
            int i;
            int j = 1;
            PropertyList pl = dataObject->getInstanceProperties();
            for (i = 0; i < pl.size(); i++)
            {
                if (dataObject->isSet(pl[i]))
                {                    
                    SDOXMLString propertyName(pl[i].getName());
                    XSDPropertyInfo* pi = getPropertyInfo(dataObjectType, pl[i]);
                    PropertyDefinitionImpl propdef;
                    if (pi)
                    {
                        propdef = pi->getPropertyDefinition();
                        propertyName = propdef.localname;
                    }
                    
                    // Elements are written as <element>
                    if (propdef.isElement)
                        continue;
                    
                    // Many-valued properties are written as <element>
                    if (pl[i].isMany())
                        continue;
                                        
                //    if (pl[i].isContainment())
                //        continue;

                    //////////////////////////////////////////////////////////////////////
                    // Non contained properties become attributes
                    //////////////////////////////////////////////////////////////////////
                    const Type& propertyType = pl[i].getType();
                    
                    if (propertyType.isDataType())
                    {
                        SDOXMLString propertyValue = (dataObject->getCString(pl[i]));
                        if (pi && pi->getPropertyDefinition().isQName)
                        {
                            XMLQName qname(propertyValue);
                             
                            //{
                            //const SDOXMLString* prefix = namespaces.findPrefix(qname.getURI());
                            //if (prefix == 0)
                            //{
                            //    char buffer[100];
                            //    SDOXMLString pref = "tns";
                            //    sprintf(buffer, "%d", i);
                            //    pref += buffer;
                            //    namespaces.add(pref, qname.getURI());
                            //    prefix = namespaces.findPrefix(qname.getURI());
                            //}

							//if (prefix != 0 && !(*prefix).equals(""))

                            std::map<SDOXMLString,SDOXMLString>::iterator it = namespaceMap.find(qname.getURI());
                            if (it != namespaceMap.end())
                            {
								propertyValue = (*it).second + ":" + qname.getLocalName();
							}
                            else 
                            {
                                char buffer[20];
                                SDOXMLString pref = "tnss";
                                sprintf(buffer, "%d", j++);
                                pref += buffer;
                                rc = xmlTextWriterWriteAttributeNS(writer, 
									SDOXMLString("xmlns"), pref, NULL, qname.getURI());
                                propertyValue = pref + ":" + qname.getLocalName();
                            }
                            
                        }
                        rc = xmlTextWriterWriteAttribute(writer, 
                            propertyName, propertyValue);
                    }
                    else
                    {
                        // Handle non-containment reference to DataObject
                        if (pl[i].isReference())
                        {
                            writeReference(dataObject, pl[i], false);
                        }
                    }
                }
            }
            
            // --------------------
            // Handle ChangeSummary
            // --------------------
            if (dataObject->getType().isChangeSummaryType())
            {
                ChangeSummaryPtr changeSummary = dataObject->getChangeSummary();
                if (changeSummary)
                {
                    handleChangeSummary(elementName, changeSummary);
                }
            }
            
            if (dataObjectType.isSequencedType())
            {
                SequencePtr sequence  = dataObject->getSequence();
                if (sequence)
                {
                    for (i=0; i<sequence->size(); i++)
                    {
                        
                        if (sequence->isText(i))
                        {
                            rc = xmlTextWriterWriteString(
                                writer,
                                SDOXMLString(sequence->getCStringValue(i)));
                            continue;
                        } // end TextType

                        const Property& seqProp = sequence->getProperty(i);
                        SDOXMLString seqPropName = seqProp.getName();
                        const Type& seqPropType = seqProp.getType();

                        if (seqPropType.isDataObjectType())
                        {                                
                            DataObjectPtr doValue;
                            if (seqProp.isMany())
                            {
                                int index = sequence->getListIndex(i);
                                doValue = dataObject->getList(seqProp)[index];
                            }
                            else
                            {
                                doValue = dataObject->getDataObject(seqProp);
                            }

                            if (doValue)
                            {
                                // Handle non-containment reference to DataObject
                                if (seqProp.isReference())
                                {
                                    writeReference(dataObject, seqProp, true, doValue);
                                }
                                else
                                {
                                    writeDO(doValue, doValue->getType().getURI(), seqPropName);
                                }
                            }
                        } // end DataObject


                        else
                        {
                            // Sequence member is a primitive
                            xmlTextWriterWriteElement(
                                writer,
                                seqPropName,
                                SDOXMLString(sequence->getCStringValue(i)));
                            
                        } // end DataType
                    } // end - iterate over sequence
                    
                }
            
            } // end sequence handling
            
            else
            {
                
                //////////////////////////////////////////////////////////////////////////
                // Iterate over all the properties to find elements
                //////////////////////////////////////////////////////////////////////////
                for (i = 0; i < pl.size(); i++)
                {
                    if (dataObject->isSet(pl[i]))
                    {
                        
                        SDOXMLString propertyName(pl[i].getName());
                        XSDPropertyInfo* pi = getPropertyInfo(dataObjectType, pl[i]);
                        if (pi)
                        {
                            if (!pi->getPropertyDefinition().isElement)
                                continue;
                            propertyName = pi->getPropertyDefinition().localname;
                        }
                        
                        const Type& propertyType = pl[i].getType();
                        
                        //////////////////////////////////////////////////////////////////////
                        // For a many-valued property get the list of values
                        //////////////////////////////////////////////////////////////////////
                        if (pl[i].isMany())
                        {
                            DataObjectList& dol = dataObject->getList(pl[i]);
                            for (int j = 0; j <dol.size(); j++)
                            {
                                // Handle non-containment reference to DataObject
                                if (pl[i].isReference() )
                                {
                                    writeReference(dataObject, pl[i], true, dol[j]);
                                }
                                else
                                {    
                                    SDOXMLString typeURI = dol[j]->getType().getURI();
                                    writeDO(dol[j], dol[j]->getType().getURI(), propertyName);
                                }
                            }
                        } // end IsMany
                        
                        //////////////////////////////////////////////////////////////////////
                        // For a dataobject write the do
                        //////////////////////////////////////////////////////////////////////
                        else if (!propertyType.isDataType())
                        {
                            // Handle non-containment reference to DataObject
                            if (pl[i].isReference())
                            {
                                if (pi)
                                    writeReference(dataObject, pl[i], true);
                            }
                            else
                            {
                                DataObjectPtr propDO = dataObject->getDataObject(pl[i]);                
                                writeDO(propDO, propDO->getType().getURI(), propertyName);
                            }
                        }
                        
                        //////////////////////////////////////////////////////////////////////
                        // For a primitive
                        //////////////////////////////////////////////////////////////////////
                        else
                        {
                            // Only write a primitive as an element if defined by the XSD
                            if (pi)
                            {
                                const Type& tp = dataObject->getType();
                                XSDTypeInfo* typeInfo = (XSDTypeInfo*)
                                    ((DASType*)&tp)->getDASValue("XMLDAS::TypeInfo");
                                if (typeInfo && typeInfo->getTypeDefinition().isExtendedPrimitive)
                                {
                                    xmlTextWriterWriteRaw(
                                    writer,
                                    SDOXMLString(dataObject->getCString(pl[i])));
                                }
                                else
                                {
                                    if (dataObject->isNull(pl[i]))
                                    {
                                        rc = xmlTextWriterStartElementNS(writer, NULL, propertyName, NULL);
                                        if (rc < 0) 
                                        {
                                            SDO_THROW_EXCEPTION("writeDO", 
                                                                SDOXMLParserException, 
                                                                "xmlTextWriterStartElementNS failed");
                                        }
                                        rc = xmlTextWriterWriteAttribute(writer, 
                                                                         (const unsigned char*)"xsi:nil", 
                                                                         (const unsigned char*)"true");
                                        
                                        writeXmlnsXsi();
                                        rc = xmlTextWriterEndElement(writer);
                                    }
                                    else
                                    {
                                        xmlTextWriterWriteElement(
                                        writer,
                                        propertyName,
                                        SDOXMLString(dataObject->getCString(pl[i])));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            rc = xmlTextWriterEndElement(writer);
            return rc;

			//namespaces = namespaceStack.top();
			//namespaceStack.pop();
            //if (!uri.isNull())
            //{
            //    namespaceUriStack.pop();
            //}
        }
        
        XSDPropertyInfo* SDOXMLWriter::getPropertyInfo(const Type& type, const Property& property)
        {
            if (dataFactory)
            {
                return (XSDPropertyInfo*)dataFactory->getDASValue(type, property.getName(), "XMLDAS::PropertyInfo");
            }
            else
            {
                return (XSDPropertyInfo*)((DASProperty*)&property)->getDASValue("XMLDAS::PropertyInfo");
            }
            
        }

        void SDOXMLWriter::writeReference(
            DataObjectPtr dataObject, 
            const Property& property,
            bool isElement,
            DataObjectPtr refferedToObject)
        {
            DataObjectPtr reffedObject = refferedToObject;
            if (reffedObject == 0)
            {
                reffedObject = dataObject->getDataObject(property);
            }

            // Get ID from referred to DataObject or use XPath
            SDOXMLString refValue;
            XSDTypeInfo* ti = (XSDTypeInfo*)((DASType*)&reffedObject->getType())->
                getDASValue("XMLDAS::TypeInfo");
            if (ti)
            {
                TypeDefinitionImpl typeDef = ti->getTypeDefinition();
                if (!typeDef.IDPropertyName.isNull())
                {
                    refValue = reffedObject->getCString((const char*)typeDef.IDPropertyName);
                }
            }
            
            if (refValue.isNull())
            {
                // need to get XPATH
                refValue = ((DataObjectImpl*)(DataObject*)reffedObject)->objectToXPath();
            }
            
            if (!refValue.isNull())
            {
                if (isElement)
                {
                    // Set the IDREF value
                    xmlTextWriterWriteElement(writer, 
                        SDOXMLString(property.getName()), refValue);
                }
                else
                {
                    // Set the IDREF value
                    xmlTextWriterWriteAttribute(writer, 
                        SDOXMLString(property.getName()), refValue);
                }
            }
        }    
        
    } // End - namespace sdo
} // End - namespace commonj

