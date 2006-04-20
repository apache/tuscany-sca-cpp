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

/* $Rev$ $Date: 2006/04/20 14:24:42 $ */

#include "commonj/sdo/SDOSAX2Parser.h"

#include "commonj/sdo/SDORuntimeException.h"
#include "commonj/sdo/ChangeSummary.h"
#include "commonj/sdo/XSDPropertyInfo.h"
#include "commonj/sdo/XMLQName.h"
#include "commonj/sdo/DASProperty.h"
#include "commonj/sdo/Logging.h"
#include "commonj/sdo/DASType.h"
#include "commonj/sdo/XSDTypeInfo.h"
#include "commonj/sdo/TypeImpl.h"
#include "commonj/sdo/DataObjectImpl.h"
#include "commonj/sdo/DataFactoryImpl.h"

namespace commonj
{
    namespace sdo
    {
        
        SDOSAX2Parser::SDOSAX2Parser(
            DataFactoryPtr df,
            const SDOXMLString& targetNamespace,
            DataObjectPtr& rootDO,
            ParserErrorSetter* insetter
            ) 
            
            : dataFactory(df),
            targetNamespaceURI(targetNamespace),
            rootDataObject(rootDO),
            currentDataObject(0),
            isDataGraph(false),
            ignoreEvents(false),
            dealingWithChangeSummary(false),
            csbuilder(0),
            SAX2Parser(insetter)
            
    
        {
            reset();
            if (targetNamespace.isNull())
            {
                targetNamespaceURI = "";
            }
            rootDataObject = 0;
        }
        
        SDOSAX2Parser::~SDOSAX2Parser()
        {
        }
        
        void SDOSAX2Parser::reset()
        {
            rootDataObject = 0;
            currentDataObject = 0;
            isDataGraph = false;
            ignoreEvents = false;
            changeSummary = false;
            IDMap.empty();
            IDRefs.empty();
        }
        
        
        void SDOSAX2Parser::startDocument()
        {
            LOGINFO(INFO,"SDOSAX2Parser: startDocument");
            setNamespaces = true;
            reset();
        }
        
        void SDOSAX2Parser::endDocument()
        {
            LOGENTRY(INFO,"SDOSAX2Parser: endDocument");
            // Iterate over IDREFs list and set references
            ID_REFS::iterator refsIter;
            for (refsIter = IDRefs.begin(); refsIter != IDRefs.end(); refsIter++)
            {
                try
                {
                    const Type& type = refsIter->dataObject->getType();
                    const Property& prop = refsIter->dataObject->getProperty(refsIter->property);
                    const Type& propType =    ((TypeImpl&)type).getRealPropertyType(refsIter->property);

                    // Allowing referenes to DataObjects only
                    if (!propType.isDataType())
                    {
                        DataObjectPtr reffedDO;
                        ID_MAP::iterator idIter = IDMap.find(refsIter->value);
                        if (idIter != IDMap.end())
                        {
                            reffedDO = idIter->second;
                        }
                        else
                        {
                            // assume it is an XPath?

                            // Remove #/ from front of XPATH as getDataObject doeesnt
                            // support this yet - it does now
                            //SDOXMLString xpath(refsIter->value);
                            //if (xpath.firstIndexOf('#') == 0)
                            //    xpath = xpath.substring(1);
                            //if (xpath.firstIndexOf('/') == 0)
                            //    xpath = xpath.substring(1);

                            reffedDO = rootDataObject->getDataObject(refsIter->value);
                        }

                        if (!reffedDO)
                        {
                            continue;
                        }

                        if (prop.isMany())
                        {
                            DataObjectList& dol = refsIter->dataObject->getList(prop);
                            dol.append(reffedDO);
                        }
                        else
                        {
                            refsIter->dataObject->setDataObject(prop, reffedDO); 
                        }
                    }

                }
                catch (const SDORuntimeException&)
                {
                }
            }
            try {
                // Now rebuild the changeSummary
                if (csbuilder != 0)
                {
                    csbuilder->buildChangeSummary(changeSummaryDO);
                    delete csbuilder;
                    csbuilder = 0;
                }
            }
            catch (SDORuntimeException&)
            {
            }

            LOGEXIT(INFO,"SDOSAX2Parser: endDocument");
        }


        bool SDOSAX2Parser::setDO(DataObjectPtr newDO, 
                                  SDOXMLString& propertyName)
        {
            LOGENTRY(INFO,"SDOSAX2Parser: setDO");

            if (currentDataObject)
            {
                const Type& type = currentDataObject->getType();
                // go lower level so we can find open properties w/o exception
                DataObject* dob = currentDataObject;
                const PropertyImpl* pprop = ((DataObjectImpl*)dob)->getPropertyImpl(propertyName);
                if (pprop == 0)
                {

                    LOGEXIT(INFO,"SDOSAX2Parser: setDO - exit1");
                    return false;
                }

                const Property& property = (Property&)*pprop;
                const Type& propertyType = ((TypeImpl&)type).getRealPropertyType(propertyName);
                if (currentDataObject->getType().isSequencedType())
                {
                    SequencePtr seq = currentDataObject->getSequence();
                    seq->addDataObject(property, newDO);
                }
                else
                {
                    if (!property.isMany())
                    {
                        currentDataObject->setDataObject(propertyName, newDO);
                    }
                    else
                    {
                        DataObjectList& dol = currentDataObject->getList(propertyName);
                        dol.append(newDO);
                    }
                }
            }
            
            setCurrentDataObject(newDO);

            LOGEXIT(INFO,"SDOSAX2Parser: setDO - exit2");
            return true;
        }
                    
        void SDOSAX2Parser::handleOpenAttribute(
                                    SDOXMLString& tns,
                                    const char* propuri,
                                    const char* propname,
                                    const char* value)
        {
            // first, see if there is a global element or attribute corresponding...
            try 
            {
                DataFactory* df = dataFactory;
                const PropertyImpl* prop = 0;
                const TypeImpl* ti = 
                    ((DataFactoryImpl*)df)->findTypeImpl(propuri,"RootType");
                
                if (ti != 0)
                {
                    prop = (const PropertyImpl*)ti->getPropertyImpl(propname);
                }
                else
                {
                    ti = ((DataFactoryImpl*)df)->findTypeImpl(tns,"RootType");
                }

                if (ti != 0)
                {
                    prop = (const PropertyImpl*)ti->getPropertyImpl(propname);
                }
            
                if (prop == 0)
                {
                    currentDataObject->setCString(propname,value);
                    return;
                }

                DataObject* dob = currentDataObject;

                switch (prop->getTypeEnum())
                {
                    case Type::BooleanType:
                        ((DataObjectImpl*)dob)->defineBoolean(propname);
                        break;
                    case Type::ByteType:
                        ((DataObjectImpl*)dob)->defineByte(propname);
                        break;
                    case Type::CharacterType:
                        ((DataObjectImpl*)dob)->defineCharacter(propname);
                        break;
                    case Type::BytesType:
                        ((DataObjectImpl*)dob)->defineBytes(propname);
                        break;
                    case Type::StringType:
                        ((DataObjectImpl*)dob)->defineString(propname);
                        break;
                    case Type::ShortType:
                        ((DataObjectImpl*)dob)->defineShort(propname);
                        break;
                    case Type::IntegerType:
                        ((DataObjectImpl*)dob)->defineInteger(propname);
                        break;
                    case Type::LongType:
                        ((DataObjectImpl*)dob)->defineLong(propname);
                        break;
                    case Type::DoubleType:
                        ((DataObjectImpl*)dob)->defineDouble(propname);
                        break;
                    case Type::FloatType:
                        ((DataObjectImpl*)dob)->defineFloat(propname);
                        break;
                    case Type::DateType:
                        ((DataObjectImpl*)dob)->defineDate(propname);
                        break;
                } // switch

                // regardless of what type the property now is, we can set CString , and the
                // right conversion will happen

                currentDataObject->setCString(propname,value);
            }
            catch (SDORuntimeException)
            {
            }
            return;
        }


        void SDOSAX2Parser::setAttributes(
            SDOXMLString& tns,
            const SAX2Namespaces& namespaces,
            const SAX2Attributes& attributes) 
        {

            LOGENTRY(INFO,"SDOSAX2Parser::setAttributes");

            //////////////////////////////////////////////
            // The attributes are properties on the new DO
            // Handle attributes
            //////////////////////////////////////////////
            for (int i=0; i < attributes.size(); i++)
            {
                // Should ignore attributes like xsi:type
                if (!(attributes[i].getUri().equalsIgnoreCase("http://www.w3.org/2001/XMLSchema-instance")))
                {                            
                    try
                    {
                        const SDOXMLString& propertyName = getSDOName(*currentDataObjectType, attributes[i].getName());
                        DataObject* dob = currentDataObject;
                        const PropertyImpl* pprop = ((DataObjectImpl*)dob)->getPropertyImpl(propertyName);
                        if (pprop == 0 )
                        {
                            if (currentDataObject->getType().isOpenType())
                            {
                                // if its an open type, then attributes will be allowed to have 
                                // an invalid name, and setCString will create them all as bytes
                                handleOpenAttribute(tns, (const char*)attributes[i].getUri(),
                                                            (const char*)attributes[i].getName(),
                                                    (const char*)attributes[i].getValue());
                                
                            }
                            else 
                            {
                                LOGERROR_1(WARNING,"SDOSAX2Parser: Property not found on closed type (ignored):%s",
                                    (const char*)(attributes[i].getName()));        
                            }
                        }
                        else 
                        {
                            const Property& prop = (Property&)*pprop;
                            SDOXMLString propValue;
                            
                            XSDPropertyInfo* pi = (XSDPropertyInfo*)((DASProperty*)&prop)->getDASValue("XMLDAS::PropertyInfo");
                            if (pi && pi->getPropertyDefinition().isQName)
                            {
                                XMLQName qname(attributes[i].getValue(),
                                    documentNamespaces, namespaces);
                                propValue = qname.getSDOName();
                            }
                            else
                            {
                                propValue = attributes[i].getValue();
                            }
                    
                            if ((pi && pi->getPropertyDefinition().isIDREF)
                                || prop.isReference())
                            {
                                // remember this value to resolve later
                                IDRef ref(currentDataObject, attributes[i].getName(), propValue);
                                IDRefs.insert(IDRefs.end(), ref);
                            }
                            else
                            {    
                                if (pi && pi->getPropertyDefinition().isID)
                                {
                                    // add this ID to the map
                                    IDMap[propValue] = currentDataObject;
                                }
                                // Always set the property as a String. SDO will do the conversion
                                currentDataObject->setCString(attributes[i].getName(), propValue);
                            }
                        }
                    }
                    catch (const SDOPropertyNotFoundException&)
                    {
                        LOGERROR_1(WARNING,"SDOSAX2Parser: Error processing attribute (ignored):%s",
                            (const char*)(attributes[i].getName()));        
                    }
                }
            } // End iterate over attributes
            
            LOGEXIT(INFO,"SDOSAX2Parser:setAttributes");

        }


        const PropertyImpl* SDOSAX2Parser::handleOpenType(
                                    SDOXMLString& tns,
                                    const SDOXMLString& localname,
                                    DataObjectImpl* dob,
                                    const SAX2Namespaces& namespaces,
                                    const SAX2Attributes& attributes,
                                    SDOXMLString& xsitypeURI,
                                    SDOXMLString& xsitypeName,
                                    bool bToBeNull)
        {
            // first, see if there is a global element or attribute corresponding...
            const PropertyImpl* pprop;
            DataObjectPtr newDO = 0;
            try 
            {
                DataFactory* df = dataFactory;
                const TypeImpl* ti = 0;
                const PropertyImpl* prop = 0;
                SDOXMLString propertyName;

                ti = ((DataFactoryImpl*)df)->findTypeImpl(tns,"RootType");
                if (ti != 0)
                {
                    propertyName = getSDOName((Type&)*ti, localname);
                    prop = ti->getPropertyImpl(propertyName);
                }
                else
                {
                    propertyName = localname;
                }

                if (prop != 0)
                {
                    if (prop->isMany())
                    {
                        pprop = ((DataObjectImpl*)dob)->defineList(propertyName);

                        // the type of the list needs to be set, as chars sets a CString
                        try 
                        {
                            DataObjectList& dl = ((DataObjectImpl*)dob)->getList(propertyName);
                            ((DataObjectListImpl*)&dl)->setType(prop->getType().getURI(),
                                prop->getType().getName());
                        }
                        catch (SDORuntimeException)
                        {
                            // let it pass - the type will be Bytes
                        }

                        if (prop->getType().isDataType())
                        {
                            currentPropertySetting = PropertySetting(currentDataObject, propertyName,
                                bToBeNull);
                        }
                        else
                        {
                            newDO = dataFactory->create(
                                prop->getType().getURI(),
                                prop->getType().getName());
                            DataObjectList& dol = dob->getList(propertyName);
                            dol.append(newDO);
                            setCurrentDataObject(newDO);
                            setAttributes(tns, namespaces,attributes);
                        }
                        return pprop;
                    }
                    else
                    {
                        switch (prop->getTypeEnum())
                        {
                            case Type::BooleanType:
                                pprop = ((DataObjectImpl*)dob)->defineBoolean(propertyName);
                                currentPropertySetting = PropertySetting(currentDataObject, propertyName,
                                    bToBeNull);
                                break;
                            case Type::ByteType:
                                pprop = ((DataObjectImpl*)dob)->defineByte(propertyName);
                                currentPropertySetting = PropertySetting(currentDataObject, propertyName,
                                    bToBeNull);
                                break;
                            case Type::CharacterType:
                                pprop = ((DataObjectImpl*)dob)->defineCharacter(propertyName);
                                currentPropertySetting = PropertySetting(currentDataObject, propertyName,
                                    bToBeNull);
                                break;
                            case Type::BytesType:
                                pprop = ((DataObjectImpl*)dob)->defineBytes(propertyName);
                                currentPropertySetting = PropertySetting(currentDataObject, propertyName,
                                    bToBeNull);
                                break;
                            case Type::StringType:
                                pprop = ((DataObjectImpl*)dob)->defineString(propertyName);
                                currentPropertySetting = PropertySetting(currentDataObject, propertyName,
                                    bToBeNull);
                                break;
                            case Type::ShortType:
                                pprop = ((DataObjectImpl*)dob)->defineShort(propertyName);
                                currentPropertySetting = PropertySetting(currentDataObject, propertyName,
                                    bToBeNull);
                                break;
                            case Type::IntegerType:
                                pprop = ((DataObjectImpl*)dob)->defineInteger(propertyName);
                                currentPropertySetting = PropertySetting(currentDataObject, propertyName,
                                    bToBeNull);
                                break;
                            case Type::LongType:
                                pprop = ((DataObjectImpl*)dob)->defineLong(propertyName);
                                currentPropertySetting = PropertySetting(currentDataObject, propertyName,
                                    bToBeNull);
                                break;
                            case Type::DoubleType:
                                pprop = ((DataObjectImpl*)dob)->defineDouble(propertyName);
                                currentPropertySetting = PropertySetting(currentDataObject, propertyName,
                                    bToBeNull);
                                break;
                            case Type::FloatType:
                                pprop = ((DataObjectImpl*)dob)->defineFloat(propertyName);
                                currentPropertySetting = PropertySetting(currentDataObject, propertyName,
                                    bToBeNull);
                                break;
                            case Type::DateType:
                                pprop = ((DataObjectImpl*)dob)->defineDate(propertyName);
                                currentPropertySetting = PropertySetting(currentDataObject, propertyName,
                                    bToBeNull);
                                break;
                            case Type::DataObjectType:
                                pprop = ((DataObjectImpl*)dob)->defineDataObject(propertyName,
                                prop->getType().getURI(), prop->getType().getName());
                                newDO = dataFactory->create(
                                    prop->getType().getURI(),
                                    prop->getType().getName());
                                dob->setDataObject(propertyName, newDO);
                                setCurrentDataObject(newDO);
                                setAttributes(tns,namespaces,attributes);
                                break;
                        }
                    } // else
                } // if prop != 0
                else 
                {
                    // The type is open, and the property doesnt exist, so we are creating
                    // a property, and need to find out the type to create.
                    // As I cannot tell if its a single value or many valued, I create all
                    // as many valued
                    // could be data object or primitive. All primitives will appear
                    // as bytes.
                    if (!xsitypeName.isNull())
                    {
                        // it has a type from xsi:type
                        pprop = ((DataObjectImpl*)dob)->defineList(propertyName);
                        newDO = dataFactory->create(xsitypeURI, xsitypeName);
                        DataObjectList& dol = dob->getList(propertyName);
                        dol.append(newDO);
                        setCurrentDataObject(newDO);
                        setAttributes(tns,namespaces,attributes);
                    }
                    else
                    {
                        pprop = ((DataObjectImpl*)dob)->defineList(propertyName);
                        currentPropertySetting = PropertySetting(currentDataObject, propertyName,
                            bToBeNull);
                    }
                }
                return pprop;
            }
            catch (SDORuntimeException)
            {
                // fail to find the property or create a dummy
                return 0;
            }
        }

        void SDOSAX2Parser::startElementNs(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI,
            const SAX2Namespaces& namespaces,
            const SAX2Attributes& attributes)
            
        {
            LOGENTRY(INFO,"SDOSAX2Parser: startElementNs");

            LOGINFO_1(INFO,"SDOSAX2Parser: startElementNs:%s",
                           (const char*)localname);

            bool bToBeNull = false;
            // Save the namespace information from the first element
            if (setNamespaces)
            {
                documentNamespaces = namespaces;
                setNamespaces = false;
            }
            
            if (ignoreEvents)
            {
                // Check for the tag we are waiting for
                if (   (ignoreTag.localname.equals(localname))
                    && (ignoreTag.uri.equals(URI))
                    && (ignoreTag.prefix.equals(prefix)) )
                {
                    ignoreTag.tagCount++;
                }
                LOGEXIT(INFO,"SDOSAX2Parser: startElementNs - exit1");
                return;
            }

            if (URI.equalsIgnoreCase("http://www.w3.org/2001/XMLSchema"))
            {
                // ignore anything within a schema
                LOGINFO_1(INFO,"SDOSAX2Parser ignores schema element:%s",
                                    (const char *)localname);

                // We need to ignore all events until the end tag for this element
                ignoreEvents = true;
                ignoreTag.localname = localname;
                ignoreTag.uri = URI;
                ignoreTag.prefix = prefix;
                ignoreTag.tagCount = 0;
                return;
            }

            
            if (dealingWithChangeSummary)
            {
                if (csbuilder == 0)
                {
                    LOGERROR(ERROR,"SDOSAX2Parser:Parser builds summary with no builder");
                    LOGEXIT(INFO,"SDOSAX2Parser: startElementNs - exit2");
                    return;
                }
                csbuilder->processStart(
                    localname,
                    prefix,
                    URI,
                    namespaces,
                    attributes);
                LOGEXIT(INFO,"SDOSAX2Parser: startElementNs - exit3");
                return;
            }


            if (URI.equalsIgnoreCase(Type::SDOTypeNamespaceURI))
            {
                ///////////////////////////////////////////////////////////////////////
                // Handle datagraph
                ///////////////////////////////////////////////////////////////////////
                if (localname.equalsIgnoreCase("datagraph"))
                {
                    // Remember this is a datagraph. The root DO will be created
                    // later when we can have a better guess at the namespaceURI
                    isDataGraph = true;
                } // end handling sdo:datagraph
                
                ////////////////////////////////////
                // Handle ChangeSummary on datagraph
                ////////////////////////////////////
                if (localname.equals("changeSummary"))
                {
                    changeSummary = true;
                    changeSummaryDO = currentDataObject;

                    csbuilder = new ChangeSummaryBuilder(
                        dataFactory, rootDataObject );

                    changeSummaryLogging = true;

                    
                    SDOXMLString logging = attributes.getValue("logging");
                    if (!logging.isNull())
                    {
                        if (logging.equals("false"))
                        {
                            changeSummaryLogging = false;
                        }
                    }

                    LOGINFO(INFO,"SDOSAX2Parser:Start change summary");
                    dealingWithChangeSummary = true;
                    LOGEXIT(INFO,"SDOSAX2Parser: startElementNs - exit4");
                    return;
                    
                }
                
            }
            else
            {
                ///////////////////////////////////////////////////////////////////////
                // Each element is a DataObject or a Property on the current DO
                ///////////////////////////////////////////////////////////////////////
                DataObjectPtr newDO = 0;
                
                SDOXMLString typeURI, typeName, propertyName;
                
                ///////////////////////////////////////////////////////////////////////
                // Determine the type. It is either specified by the xsi:type attribute
                // or the localname is the name of a property on "RootType"
                ///////////////////////////////////////////////////////////////////////
                int i;
                for (i=0; i < attributes.size(); i++)
                {
                    if (attributes[i].getUri().equalsIgnoreCase("http://www.w3.org/2001/XMLSchema-instance"))
                    {
                        if (attributes[i].getName().equalsIgnoreCase("type"))
                        {
                            SDOXMLString fullTypeName = attributes[i].getValue();
                            SDOXMLString pref;

                            int index = fullTypeName.firstIndexOf(':');
                            if (index < 0)
                            {
                                typeName = fullTypeName;
                            }
                            else
                            {    
                                // Is the namespace prefix defined?
                                typeName = fullTypeName.substring(index+1);
                                pref = fullTypeName.substring(0, index);
                            }

                            // Convert the prefix to a namespace URI
                            const SDOXMLString* namespaceURI = namespaces.find(pref);
                            if (namespaceURI == 0)
                            {
                                namespaceURI = documentNamespaces.find(pref);
                            }
                            if (namespaceURI != 0)
                            {
                                typeURI = *namespaceURI;
                            }
                        }
                        else if (attributes[i].getName().equalsIgnoreCase("nil"))
                        {
                            if (attributes[i].getValue().equalsIgnoreCase("true"))
                            {
                                // the current setting needs to be setNull
                                bToBeNull = true;
                            }
                        }
                    }
                } // End - attribute loop
                
                if (typeURI.isNull())
                {
                    typeURI = "";
                }
                
                SDOXMLString tns = URI;
                
                try
                {
                    if (currentDataObject == 0)
                    {
                        // This element should become the root data object
                        
                        // Target namespace will be:
                        //   the targetNamespaceURI if specified 
                        //   or the URI of xsi:type if specified
                        //   or the URI of this element
                        if (!typeURI.equals(""))
                        {
                            tns = typeURI;
                        }

                        if (!targetNamespaceURI.isNull() && !targetNamespaceURI.equals(""))
                        {
                            tns = targetNamespaceURI;
                        }
                        
                        // Check for localname as a property of the RootType
                        // if we do not already know the type
                        if (typeName.isNull())
                        {
                            const Type& rootType = dataFactory->getType(tns, "RootType");
                            propertyName = getSDOName(rootType, localname);
                            const Type& newType = 
                                ((TypeImpl&)(rootType)).getRealPropertyType(propertyName);

                            typeURI = newType.getURI();
                            typeName = newType.getName();
                        }
                        
                        // Create the root DataObject
                        if (isDataGraph)
                        {
                            DataObjectPtr rootdo = dataFactory->create(tns, "RootType");
                            setCurrentDataObject(rootdo);
                            changeSummaryDO = currentDataObject;
                        }
                        else
                        {
                            DataFactory* df = dataFactory;
                            ((DataFactoryImpl*)df)->setRootElementName(localname);
                        }
                        
                        // NOTE: always creating DO doesn't cater for DataType as top element

                        const Type& tp = dataFactory->getType(typeURI,typeName);
                        if (tp.isDataType())
                        {
                            newDO = dataFactory->create(tns, "RootType");
                            currentPropertySetting = PropertySetting(newDO, localname,
                            bToBeNull);
                        }
                        else
                        {

                            newDO = dataFactory->create(typeURI, typeName);

                            // get the type definition, and see if its an extended primitive.

                            XSDTypeInfo* typeInfo = (XSDTypeInfo*)
                              ((DASType*)&tp)->getDASValue("XMLDAS::TypeInfo");
                            if (typeInfo)
                            {
                                const TypeDefinition& typeDefinition = typeInfo->getTypeDefinition();
                                if (typeDefinition.isExtendedPrimitive)
                                {
                                    // The name of this element is the name of a property on the current DO
                                    currentPropertySetting = PropertySetting(newDO, localname,
                                    bToBeNull);
                                }
                            }
                        }
                        
                    } // End - currentDataObject == 0
                    
                    else
                    { // currentDataObject != 0
                        
                        // Get the Property from the dataObject
                        propertyName = getSDOName(*currentDataObjectType, localname);
                        const Type& type = currentDataObject->getType();

                    
                        // go lower level so we can find open properties w/o exception
                        DataObject* dob = currentDataObject;
                        const PropertyImpl* pprop = ((DataObjectImpl*)dob)->getPropertyImpl(propertyName);
                        if (pprop == 0)
                        {
                            if (type.isOpenType())
                            {
                                pprop = handleOpenType(
                                    tns,
                                    localname,
                                    (DataObjectImpl*)dob,
                                     namespaces,
                                     attributes,
                                     typeURI,
                                     typeName,
                                     bToBeNull);
                            }
                            if (pprop == 0)
                            {
                                // this is an open property , we will need to create it
                                LOGERROR_1(WARNING,"SDOSAX2Parser Unknown element:%s",
                                    (const char *)localname);

                                // We need to ignore all events until the end tag for this element
                                ignoreEvents = true;
                                ignoreTag.localname = localname;
                                ignoreTag.uri = URI;
                                ignoreTag.prefix = prefix;
                                ignoreTag.tagCount = 0;
                                if (setter != 0)
                                {
                                    char *msg = new char[strlen((const char*)localname) + 32];
                                    if (msg) {
                                        sprintf(msg,"Parser found unknown element %s",
                                            (const char*)localname);
                                        setter->setError( msg );
                                        delete msg;
                                    }
                                }
                             }
                            LOGEXIT(INFO,"SDOSAX2Parser: startElementNs - exit5");
                            return;
                        }
                        else
                        {

                            const Property& prop = (Property&)*pprop;
                            const TypeImpl* propType = ((TypeImpl&)type).getRealPropertyTypeImpl(propertyName);
                            if (propType == 0)
                            {
                                // could be a previously created open type property
                                propType = (const TypeImpl*)pprop->getTypeImpl();
                            }
                            if (propType != 0)
                            {
                                XSDPropertyInfo* pi = (XSDPropertyInfo*)((DASProperty*)&prop)->getDASValue("XMLDAS::PropertyInfo");
                                if ((pi && pi->getPropertyDefinition().isIDREF)
                                || prop.isReference())
                                {
                                    // The name of this element is the name of a property on the current DO
                                    currentPropertySetting = PropertySetting(currentDataObject, propertyName, bToBeNull,
                                        true);                        
                                }
                        
                                // If it is a DataType then we need set the value
                                else if (propType->isDataType() )
                                {
                                    // The name of this element is the name of a property on the current DO
                                    currentPropertySetting = PropertySetting(currentDataObject, propertyName,
                                        bToBeNull);
                                }
                                else
                                {

                                    // If typeName is not set then create object of Type of Property
                                    // otherwise use the typeURI and typeName specified by e.g. xsi:type
                                    if (typeName.isNull())
                                    {
                                        newDO = dataFactory->create(propType->getURI(), propType->getName());
                                    }
                                    else
                                    {
                                        newDO = dataFactory->create(typeURI, typeName);
                                    }

                                    XSDTypeInfo* typeInfo = (XSDTypeInfo*)
                                    ((DASType*)propType)->getDASValue("XMLDAS::TypeInfo");
                                    if (typeInfo && typeInfo->getTypeDefinition().isExtendedPrimitive)
                                    {
                                        // The name of this element is the name of a property on the current DO
                                        currentPropertySetting = PropertySetting(newDO, "value", bToBeNull);
                                    }

                                }
                            }
                        }  // End // currentDataObject != 0
                    } // end prop != 0
                    if (newDO)
                    {
                        if (!setDO(newDO, propertyName))
                        {
                            LOGERROR_1(WARNING,"SDOSAX2Parser Unknown element:%s",
                                (const char *)localname);

                            // We need to ignore all events until the end tag for this element
                            ignoreEvents = true;
                            ignoreTag.localname = localname;
                            ignoreTag.uri = URI;
                            ignoreTag.prefix = prefix;
                            ignoreTag.tagCount = 0;
                            if (setter != 0)
                            {
                                char *msg = new char[strlen((const char*)localname) + 32];
                                if (msg) {
                                    sprintf(msg,"Parser found unknown element %s",
                                    (const char*)localname);
                                    setter->setError( msg );
                                    delete msg;
                                }
                            }
                            LOGEXIT(INFO,"SDOSAX2Parser: startElementNs - exit6");
                            return;
                        }
                    }
                } // end try

                catch (const SDOTypeNotFoundException& )
                {
                    
                    LOGERROR_1(WARNING,"SDOSAX2Parser Unknown element:%s",
                        (const char *)localname);

                    // We need to ignore all events until the end tag for this element
                    ignoreEvents = true;
                    ignoreTag.localname = localname;
                    ignoreTag.uri = URI;
                    ignoreTag.prefix = prefix;
                    ignoreTag.tagCount = 0;
                    if (setter != 0)
                    {
                        char *msg = new char[strlen((const char*)localname) + 32];
                        if (msg) {
                            sprintf(msg,"Parser found unknown element %s",
                            (const char*)localname);
                            setter->setError( msg );
                            delete msg;
                        }
                    }
                    LOGEXIT(INFO,"SDOSAX2Parser: startElementNs - exit7");
                    return;
                }

#ifdef _DEBUG
                catch (const SDOPropertyNotFoundException& e )
#else
                catch (const SDOPropertyNotFoundException&  )
#endif
                {
                    LOGERROR_1(WARNING,"SDOSAX2Parser Unknown element exception:%s",
                        (const char *)localname);
                    LOGSDOEXCEPTION(WARNING,"Exception:",e);

                    // We need to ignore all events until the end tag for this element
                    ignoreEvents = true;
                    ignoreTag.localname = localname;
                    ignoreTag.uri = URI;
                    ignoreTag.prefix = prefix;
                    ignoreTag.tagCount = 0;
                    if (setter != 0)
                    {
                        char *msg = new char[strlen((const char*)localname) + 32];
                        if (msg)
                        {
                            sprintf(msg,"Parser found unknown element %s",
                                 (const char*)localname);
                            setter->setError( msg );
                            delete msg;
                        }
                    }
                    LOGEXIT(INFO,"SDOSAX2Parser: startElementNs - exit8");
                    return;
                }

                setAttributes(tns,namespaces, attributes);
                
            }
            LOGEXIT(INFO,"SDOSAX2Parser: startElementNs - exit9");
        }
        
        
        void SDOSAX2Parser::endElementNs(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI)
        {

            LOGENTRY(INFO,"SDOSAX2Parser: endElementNs");

            if (localname.equals("changeSummary"))
            {
                // end of change summary
                dealingWithChangeSummary = false;
                LOGINFO(INFO,"SDOSAX2Parser: Finished change summary");
                LOGEXIT(INFO,"SDOSAX2Parser: endElementNs - exit1");
                return;
            }

            if (dealingWithChangeSummary)

            {
                if (csbuilder == 0) 
                {
                    LOGERROR(WARNING,"SDOSAX2Parser: End change summary with no builder");
                    LOGEXIT(INFO,"SDOSAX2Parser: endElementNs - exit2");
                    return;
                }
                csbuilder->processEnd(localname,
                                     prefix,
                                     URI);
                LOGEXIT(INFO,"SDOSAX2Parser: endElementNs - exit3");
                return;
            }


            if (ignoreEvents)
            {
                // Check for the tag we are waiting for
                if (   (ignoreTag.localname.equals(localname))
                    && (ignoreTag.uri.equals(URI))
                    && (ignoreTag.prefix.equals(prefix)) )
                {
                    if (ignoreTag.tagCount == 0)
                    {
                        ignoreEvents = false;
                    }
                    ignoreTag.tagCount--;
                }
                LOGEXIT(INFO,"SDOSAX2Parser: endElementNs - exit4");
                return;
            }
            
            // If currentPropertySetting is set (name is not null)
            // then we need to set the property now
            if (!currentPropertySetting.name.isNull())
            {
                if (currentPropertySetting.isNULL)
                {

                    currentPropertySetting.dataObject->
                            setNull((const char*)currentPropertySetting.name);

                }
                else 
                {    
                    if (currentPropertySetting.value.isNull())
                    {
                        currentPropertySetting.value = SDOXMLString("");
                    }
                    try
                    {
                        const Type& tp = currentPropertySetting.dataObject->getType();
                        XSDTypeInfo* typeInfo = (XSDTypeInfo*)
                            ((DASType*)&tp)->getDASValue("XMLDAS::TypeInfo");
                        if (typeInfo && typeInfo->getTypeDefinition().isExtendedPrimitive)
                        {
                            const Property& p = currentPropertySetting.dataObject->getProperty(
                                "value");
                            if (p.isMany())
                            {
                                DataObjectList& dl = currentPropertySetting.dataObject->
                                getList((const char*)"value");
                                dl.append((const char*)currentPropertySetting.value);
                            }
                            else
                            {
                                currentPropertySetting.dataObject->
                                setCString((const char*)"value", currentPropertySetting.value );
                            }
                            if (dataObjectStack.size() == 0 || rootDataObject == dataObjectStack.top())
                            {
                                currentDataObject = 0;
                                currentDataObjectType = 0;
                            }
                            else
                            {
                                dataObjectStack.pop();
                                currentDataObject = dataObjectStack.top();
                                currentDataObjectType = &(currentDataObject->getType());
                            }

                        }
                        else
                        {
                            if (currentPropertySetting.isIDREF)
                            {
                                // remember this value to resolve later
                                IDRef ref(currentPropertySetting.dataObject,
                                    currentPropertySetting.name,
                                    currentPropertySetting.value );
                                IDRefs.insert(IDRefs.end(), ref);
                            }
                            else
                            {
                                if (currentPropertySetting.dataObject->getType().isSequencedType())
                                {
                                    SequencePtr seq = currentPropertySetting.dataObject->getSequence();
                                    seq->addCString(currentPropertySetting.name, currentPropertySetting.value);
                                }
                                // Always set the property as a String. SDO will do the conversion

                                // It might be a single setting for a many-valued property.
                                // may throw SDOPropertyNotFoundException

                                const Property& p = currentPropertySetting.dataObject->getProperty(
                                    currentPropertySetting.name);
                                if (p.isMany())
                                {
                                    DataObjectList& dl = currentPropertySetting.dataObject->
                                    getList((const char*)currentPropertySetting.name);
                                    dl.append((const char*)currentPropertySetting.value);
                                }
                                else
                                {
                                    currentPropertySetting.dataObject->
                                    setCString((const char*)currentPropertySetting.name, currentPropertySetting.value );
                                }
                            }
                        }
                    }
#ifdef _DEBUG
                    catch (const SDOPropertyNotFoundException& e)
#else
                    catch (const SDOPropertyNotFoundException&)
#endif
                    {
                        LOGSDOEXCEPTION(WARNING,"SDOSAX2Parser error attribute (ignored)",e);         
                    }
                }
                currentPropertySetting = PropertySetting();
                
            }
            else
            {
                if (changeSummary 
                    && changeSummaryLogging 
                    && changeSummaryDO == currentDataObject)
                {
                    // Set logging on for this DO before it is popped from stack
                    ChangeSummary* cs = currentDataObject->getChangeSummary();
                    if (cs)
                    {
                        cs->beginLogging();
                    }
                    changeSummary = false;
                }
                
                if (dataObjectStack.size() == 0 || rootDataObject == dataObjectStack.top())
                {
                    currentDataObject = 0;
                    currentDataObjectType = 0;
                }
                else
                {
                    dataObjectStack.pop();
                    currentDataObject = dataObjectStack.top();
                    currentDataObjectType = &(currentDataObject->getType());
                }
            }
            LOGEXIT(INFO,"SDOSAX2Parser: endElementNs - exit4");
        }


         
        void SDOSAX2Parser::characters(const SDOXMLString& chars)
        {
            if (dealingWithChangeSummary)
            {
                if (csbuilder == 0)
                {
                    LOGERROR(WARNING,"SDOSAX2Parser: no builder");
                    return;
                }
                csbuilder->processChars(chars);
                return;
            }

            if (ignoreEvents)
                return;

            if (!currentPropertySetting.name.isNull())
            {
                currentPropertySetting.value = currentPropertySetting.value + chars;    
                return;
            }

            
            DataObject* dob = currentDataObject;
            if ((dob != 0)  && ((DataObjectImpl*)dob)->getTypeImpl().isFromList())
            {
                // this is a list,so we need to split it up
                DataObjectList& dl = currentDataObject->getList(
                       (const char *)"values");

                const char* str = (const char*)chars;
                char* buf = new char[strlen(str)+1];
                if (!buf) return;

                strcpy(buf,str);

                int start_point = 0;
                int end_point;
                int final = strlen(buf);

                do {
                   if (start_point >= final)break;
                   while (buf[start_point] == (char)0x20 || buf[start_point] == (char)0x09
                       || buf[start_point] == (char)0x0A || buf[start_point] == (char)0x0D )start_point++;
                   end_point = start_point;
                   while (buf[end_point] != (char)0x20 && buf[end_point] != (char)0x09 &&
                          buf[end_point] != (char)0x0A && buf[end_point] != (char)0x0D &&
                                                                        buf[end_point] != 0x0)end_point++;
                   if (end_point == start_point)break; 
                   *(buf+end_point) = 0;
                   dl.append((const char*)(buf+start_point));
                   start_point = end_point + 1;
                } while(1);

                delete buf;
                return;
            }

            
            // If the current DataObject is a sequenced Type
            // then add this as text to the sequence
            if (currentDataObject && currentDataObjectType->isSequencedType())
            {
                SequencePtr seq = currentDataObject->getSequence();
                if (seq)
                {
                    seq->addText(chars);
                }
            }
        }
        
        
        void SDOSAX2Parser::setCurrentDataObject(DataObjectPtr currentDO)
        {    
            currentDataObject = currentDO;
            dataObjectStack.push(currentDataObject);
            currentDataObjectType = &(currentDataObject->getType());
            if (rootDataObject == 0)
            {
                rootDataObject = currentDataObject;
            }
        }

        const SDOXMLString& SDOSAX2Parser::getSDOName(const Type& type, const SDOXMLString& localName)
        {

/*            XSDTypeInfo* typeInfo = (XSDTypeInfo*)((DASType*)&type)->getDASValue("XMLDAS::TypeInfo");
            if (typeInfo)
            {
                const TypeDefinition& typeDefinition = typeInfo->getTypeDefinition();
                XmlDasPropertyDefs::const_iterator propsIter;
                for (propsIter = typeDefinition.properties.begin(); propsIter != typeDefinition.properties.end(); propsIter++)
                {
                    const PropertyDefinition& prop = *propsIter;
                    if (prop.localname.equals(localName))
                    {
                        return prop.name;
                    }
                    for (int i=0;i< prop.substituteNames.size();i++)
                    {
                        if (prop.substituteLocalNames[i].equals(localName))
                        {
                            return prop.substituteNames[i];
                            // possibly should be return prop.name;
                        }
                    }
                }                    
            }
            */

            PropertyList pl = type.getProperties();
            for (int i = 0; i < pl.size(); i++)
            {
                XSDPropertyInfo* pi = (XSDPropertyInfo*)((DASProperty*)&pl[i])->getDASValue("XMLDAS::PropertyInfo");
                if (pi)
                {
                    const PropertyDefinition&  propdef = pi->getPropertyDefinition();
                    if (localName .equals(propdef.localname))
                        return propdef.name;

                    for (int j=0;j< propdef.substituteNames.size();j++)
                    {
                        if (propdef.substituteLocalNames[j].equals(localName))
                        {
                            return propdef.substituteNames[j];
                            // possibly should be return propdef.name;
                        }
                    }
                }
            }
                


            return localName;
        }


        std::istream& operator>>(std::istream& input, SDOSAX2Parser& parser)
        {
            parser.stream(input);
            
            return input;
        }
        
        std::istringstream& operator>>(std::istringstream& input, SDOSAX2Parser& parser)
        {
            parser.stream(input);
            
            return input;
        }
        
    } // End - namespace sdo
} // End - namespace commonj

