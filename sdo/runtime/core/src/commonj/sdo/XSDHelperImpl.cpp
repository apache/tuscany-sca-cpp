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

#include "commonj/sdo/SDOXMLFileWriter.h"   // Include first to avoid libxml compile problems!
#include "commonj/sdo/SDOXMLStreamWriter.h" // Include first to avoid libxml compile problems!
#include "commonj/sdo/SDOXMLBufferWriter.h" // Include first to avoid libxml compile problems!
#include "commonj/sdo/SDOXSDFileWriter.h"
#include "commonj/sdo/SDOXSDStreamWriter.h"
#include "commonj/sdo/SDOXSDBufferWriter.h"
#include "commonj/sdo/XSDHelperImpl.h"
#include "commonj/sdo/XMLDocumentImpl.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "commonj/sdo/SDOSchemaSAX2Parser.h"
#include "commonj/sdo/SDOSAX2Parser.h"
#include "commonj/sdo/XSDPropertyInfo.h"
#include "commonj/sdo/XSDTypeInfo.h"

#include "commonj/sdo/SDORuntimeException.h"
#include "commonj/sdo/DASProperty.h"
#include "commonj/sdo/Logging.h"
#include "commonj/sdo/Type.h"

namespace commonj
{
    namespace sdo
    {
    
        /**  XSDHelperImpl
         *
         * XSDHelperImpl is the implementation of the abstract class
         * XSDHelper.
         * Provides the means of loading and saving XSD information 
         * from/to the metadata (Types and properties)
         */

        XSDHelperImpl::XSDHelperImpl(DataFactoryPtr df)
        {
            dataFactory = (DataFactory*)df;
            if (!dataFactory) 
            {
                dataFactory = DataFactory::getDataFactory();
            }
        }
        
        XSDHelperImpl::~XSDHelperImpl()
        {
            clearErrors();
        }
        
        /**  define builds types/properties from file or stream
         *
         * define/defineFile
         *
         * Populates the data factory with Types and Properties from the schema
         * Loads from file, stream or char* buffer.
         * The return value is the URI of the root Type
         *
         */

        const char* XSDHelperImpl::defineFile(const char* schema)
        {

            SDOSchemaSAX2Parser schemaParser(schemaInfo, this);
            clearErrors();
            if (schemaParser.parse(schema) == 0)
            {
                defineTypes(schemaParser.getTypeDefinitions());
                return schemaInfo.getTargetNamespaceURI();
            }
            return 0;
        }
        
        const char*  XSDHelperImpl::define(std::istream& schema)
        {
            SDOSchemaSAX2Parser schemaParser(schemaInfo, this);
            clearErrors();
            schema  >> schemaParser;
            defineTypes(schemaParser.getTypeDefinitions());
            return schemaInfo.getTargetNamespaceURI();
        }
        
        const char*  XSDHelperImpl::define(const char* schema)
        {
            std::istringstream str(schema);
            SDOSchemaSAX2Parser schemaParser(schemaInfo, this);
            clearErrors();
            str  >> schemaParser;
            defineTypes(schemaParser.getTypeDefinitions());
            return schemaInfo.getTargetNamespaceURI();
            return define(str);
        }
        
        void XSDHelperImpl::newSubstitute(const char* entryName,
                                          PropertyDefinitionImpl& prop)
        {
            try 
            {
                const char* typeUri = prop.substituteUri.isNull() ? 
                            prop.typeUri : prop.substituteUri; 
                const Type& rootType = dataFactory->getType(typeUri, entryName);
                PropertyList pl = rootType.getProperties();
                for (int j = 0; j < pl.size(); j++)
                {
                    if (!pl[j].getType().isDataType()
                        && strcmp(pl[j].getType().getURI(),Type::SDOTypeNamespaceURI.c_str()))
                    {
                        // recurse the tree..
                        newSubstitute(pl[j].getType().getName(),
                                    prop);

                        if (!strcmp(pl[j].getName(),prop.substituteName))
                        {
                            dataFactory->setPropertySubstitute(rootType.getURI(),
                                                           rootType.getName(),
                                                        pl[j].getName(), 
                                                        prop.name,
                                                        typeUri,
                                                        prop.typeName);
                            XSDPropertyInfo* pi = (XSDPropertyInfo*)
                            ((DASProperty*)&pl[j])->getDASValue("XMLDAS::PropertyInfo");
                            if (pi)
                            {
                                PropertyDefinitionImpl& propdef = (PropertyDefinitionImpl&)pi->getPropertyDefinition();
                                propdef.substituteNames.push_back(prop.name);
                                propdef.substituteLocalNames.push_back(prop.localname);
                            }

                        }
                        
                    }
                }
            }
            catch (const SDORuntimeException&)
            {
            }
        }

        void XSDHelperImpl::addSubstitutes(PropertyDefinitionImpl& prop,
                                            TypeDefinitionImpl& ty)
        {
            try 
            {
                const char* typeUri =  prop.substituteUri.isNull() ?
                                 ty.uri : prop.substituteUri;  
                DataFactoryImpl* df = (DataFactoryImpl*)(DataFactory*)dataFactory;
                const Type* rootType = df->findType(typeUri,"RootType");
                if (rootType == 0) return;
                PropertyList pl = rootType->getProperties();
                for (int j = 0; j < pl.size(); j++)
                {
                    XSDPropertyInfo* pi = (XSDPropertyInfo*)
                    ((DASProperty*)&pl[j])->getDASValue("XMLDAS::PropertyInfo");
                    if (pi)
                    {
                        PropertyDefinitionImpl& propdef = (PropertyDefinitionImpl&)pi->getPropertyDefinition();
                        if (propdef.isSubstitute && propdef.substituteName.equals(prop.name))
                        {

                            LOGINFO_1(INFO,"XSDHelper adding substitute for property %s", (const char*)prop.name);

                            dataFactory->setPropertySubstitute(typeUri,ty.name,
                                       prop.name, propdef.name,
                                       pl[j].getType().getURI(),
                                       pl[j].getType().getName());
                            prop.substituteNames.push_back(propdef.name);
                            prop.substituteLocalNames.push_back(propdef.localname);
                        }
                    }
                }
            }
            catch (const SDORuntimeException&)
            {
            }
        }


        /**  defineTypes
         *
         * This method works through all the data gathered during parsing
         * and defines all the types using the data factory.
         */

        void XSDHelperImpl::defineTypes(TypeDefinitions& types)
        {
            defineTypes(types.getTypeDefinitions());
        }

        
        /**  defineTypes
         *
         * This method works through all the data gathered during parsing
         * and defines all the types using the data factory.
         */

        void XSDHelperImpl::defineTypes(TypeDefinitionsImpl& typedefs) 
        {
            DataFactoryImpl* df = (DataFactoryImpl*)(DataFactory*)dataFactory;
            
            XMLDAS_TypeDefs types = typedefs.types;
            XMLDAS_TypeDefs::iterator iter;


            for (iter=types.begin(); iter != types.end(); iter++)
            {
                TypeDefinitionImpl& ty = iter->second;
                try
                {
                    /*
                    string isSeqCode = "false";
                    if (ty.isSequenced)
                        isSeqCode = "true";
                    string isOpenCode = "false";
                    if (ty.isOpen)
                        isOpenCode = "true";
                    string isAbstractCode = "false";
                    if (ty.isAbstract)
                        isAbstractCode = "true";
                    string uriCode = "0";
                    if (!ty.uri.isNull())
                    {
                        uriCode = "\"" + string(ty.uri) + "\"";
                    }

                    string addTypeCode = "dataFactory->addType(\n"
                        + uriCode + ", \"" + string(ty.name) + "\", \n"
                        + isSeqCode + ", " + isOpenCode + ", " + isAbstractCode +");";
                    cout << addTypeCode.c_str() <<endl;
                    */

                    df->addType(ty.uri, ty.name, ty.isSequenced, 
                                         ty.isOpen, ty.isAbstract, ty.dataType, ty.isFromList);
                    df->setDASValue(
                        ty.uri, ty.name,
                        "XMLDAS::TypeInfo",
                        new XSDTypeInfo(ty));

                    // add aliases
                    if (!ty.aliases.isNull())
                    {
                        SDOXMLString al = ty.aliases;
                        SDOXMLString al1 = "";
                        int index;
                        while (!al.isNull() && !al.equals(""))
                        {
                            index = al.lastIndexOf(' ');
                            if (index < 0)
                            {
                                al1 = al;
                                al = "";
                            }
                            else
                            {
                                al1 = al.substring(index + 1);
                                al = al.substring(0,index);
                            }
                            if (!al1.isNull() && !al1.equals(""))
                            {
                                df->setAlias(
                                    ty.uri,
                                    ty.name,
                                    (const char*)al1);
                            }
                        }
                    }

                }        
                catch (SDORuntimeException& e)
                {
                    SDO_RETHROW_EXCEPTION("defineTypes", e);
                }
            }
            for (iter=types.begin(); iter != types.end(); iter++)
            {
                TypeDefinitionImpl& ty = iter->second;
                if (!ty.parentTypeName.isNull())
                {
                    try 
                    {
                        /*
                        string uriCode = "0";
                        if (!ty.uri.isNull())
                        {
                            uriCode = "\"" + string(ty.uri) + "\"";
                        }
                        string parentUriCode = "0";
                        if (!ty.uri.isNull())
                        {
                            parentUriCode = "\"" + string(ty.parentTypeUri) + "\"";
                        }
                        
                        string addTypeCode = "dataFactory->setBaseType(\n"
                            + uriCode + ", \"" + string(ty.name) + "\", \n"
                            + parentUriCode + ", \"" + string(ty.parentTypeName) + "\");";

                        cout << addTypeCode.c_str() <<endl;
                        */

                       df->setBaseType(
                            ty.uri,
                            ty.name,
                            ty.parentTypeUri,
                            ty.parentTypeName,
                            ty.isRestriction);
                    }        
                    catch (SDORuntimeException& e)
                    {
                        SDO_RETHROW_EXCEPTION("defineTypes", e);
                    }                        
                }
                
                XmlDasPropertyDefs::iterator propsIter;
                for (propsIter = ty.properties.begin(); propsIter != ty.properties.end(); propsIter++)
                {
                    PropertyDefinitionImpl& prop = *propsIter;
                    
                    // For a refence we need to determine the type from the
                    // global element declaration
                    if(prop.isReference)
                    {
                        
                        bool refFound = false;

                        if (prop.name.isNull())
                            prop.name = prop.typeName;

                        
                        XMLDAS_TypeDefs::iterator refTypeIter = 
                            types.find(TypeDefinitionsImpl::getTypeQName(prop.typeUri, "RootType"));
                        if(refTypeIter != types.end())
                        {

                            TypeDefinitionImpl rootTy = refTypeIter->second;
                            
                            // find the property on the root type
                            XmlDasPropertyDefs::iterator refPropsIter;
                            for (refPropsIter = rootTy.properties.begin(); refPropsIter != rootTy.properties.end(); refPropsIter++)
                            {

                                if (refPropsIter->localname.equals(prop.typeName))
                                {
                                    prop.typeUri = refPropsIter->typeUri;
                                    prop.typeName = refPropsIter->typeName;
                                    refFound = true;
                                }
                            }
                        }
                        if (!refFound)
                        {

                                // Check if this type is already defined to the data factory
                            try
                            {
                                const Type& rootType = dataFactory->getType(prop.typeUri, "RootType");
                                PropertyList pl = rootType.getProperties();

                                for (int j = 0; j < pl.size(); j++)
                                {
                                    
                                    XSDPropertyInfo* pi = (XSDPropertyInfo*)
                                        ((DASProperty*)&pl[j])->getDASValue("XMLDAS::PropertyInfo");
                                    


                                    if (prop.typeName.equals(pl[j].getName())
                                        || (pi && prop.typeName.equals(pi->getPropertyDefinition().localname)))
                                    {
                                        prop.typeUri = pl[j].getType().getURI();
                                        prop.typeName = pl[j].getType().getName();
                                        refFound = true;
                                        break;
                                    }
                                }
                            }
                            catch (const SDORuntimeException&)
                            {
                            }
                        }
                        // If we haven't been able to resolve this reference we should ignore it
                        if (!refFound)
                        {
                            LOGINFO_1(INFO,"XsdHelper:no matching reference for %s",
                                (const char *)prop.name);
                            continue;
                        }
                    }
                    
                    if (prop.name.isNull())
                    {
                        continue;
                    }
                    XMLDAS_TypeDefs::iterator propTypeIter = 
                        types.find(TypeDefinitionsImpl::getTypeQName(prop.typeUri, prop.typeName));
                    if(propTypeIter != types.end())
                    {
                        prop.typeName = propTypeIter->second.name;
                    }

                    try 
                    {

                        df->addPropertyToType(ty.uri, ty.name,
                            prop.name,
                            prop.typeUri,
                            prop.typeName,
                            prop.isMany,
                            prop.isReadOnly,
                            prop.isContainment);

                        LOGINFO_1(INFO,"XSDHelper adds property %s",(const char*)(prop.name));

                        if (!prop.aliases.isNull())
                        {
                            SDOXMLString al = prop.aliases;
                            SDOXMLString al1 = "";
                            int index;
                            while (!al.isNull() && !al.equals(""))
                            {
                                index = al.lastIndexOf(' ');
                                if (index < 0)
                                {
                                    al1 = al;
                                    al = "";
                                }
                                else
                                {
                                    al1 = al.substring(index + 1);
                                    al = al.substring(0,index);
                                }
                                if (!al1.isNull() && !al1.equals(""))
                                {
                                    df->setAlias(
                                    (const char*)ty.uri,
                                    (const char*)ty.name,
                                    (const char*)prop.name,
                                    (const char*)al1);
                                }
                            }
                        }
                        if (prop.isSubstitute)
                        {
                            // we have a new substitute - so will check the existing graph for
                            // substitutions to insert
                            
                            LOGINFO_1(INFO,"XSDHelper: Found new substitute %s",(const char*)(prop.name));

                            newSubstitute( "RootType",prop);
                        }
                        else
                        {
                            addSubstitutes(prop,ty);
                        }
                        
                        // Do not add DASValue to ChangeSummary
                        if (!(prop.typeUri.equals(Type::SDOTypeNamespaceURI.c_str())
                            && prop.typeName.equals("ChangeSummary")))
                        {
                            df->setDASValue(
                                ty.uri, ty.name,
                                prop.name,
                                "XMLDAS::PropertyInfo",
                                new XSDPropertyInfo(prop));
                        }
                    }
                    catch (SDORuntimeException& e)
                    {
                        SDO_RETHROW_EXCEPTION("defineTypes", e);
                    }
                }
                
            }
        } // End - defineTypes
        
        /**  getDataFactory returns the factory
         *
         */

        DataFactoryPtr XSDHelperImpl::getDataFactory()
        {
            return dataFactory;
        }
        
        
        /**  generate buildsXSD from types/properties
         *
         * generate/generateFile
         *
         * Saves the types/properties to an XSD stream or file
         *
         */

        void XSDHelperImpl::generateFile(
            const TypeList& types,
            const char* fileName, 
            const char* targetNamespaceURI,
            int indent
            )
        {
            SDOXSDFileWriter writer(fileName);
            DataFactory* fac = dataFactory;
            writer.write(types, targetNamespaceURI, 
                ((DataFactoryImpl*)fac)->getOpenProperties(), indent);
        }
        
        void XSDHelperImpl::generate(
            const TypeList& types,
            std::ostream& outXml,
            const char* targetNamespaceURI,
            int indent
            )
        {
            SDOXSDStreamWriter writer(outXml);
            DataFactory* fac = dataFactory;
            writer.write(types,    targetNamespaceURI, 
                ((DataFactoryImpl*)fac)->getOpenProperties(), indent);                
        }
        
        char* XSDHelperImpl::generate(
            const TypeList& types,
            const char* targetNamespaceURI,
            int indent
            )
        {
            SDOXSDBufferWriter writer;
            DataFactory* fac = dataFactory;
            writer.write(types, targetNamespaceURI, 
                ((DataFactoryImpl*)fac)->getOpenProperties(), indent);
            SDOXMLString ret = writer.getBuffer();
            char* retString = new char[strlen(ret) +1];
            strcpy(retString, ret);
            return retString;
        }

        int XSDHelperImpl::getErrorCount() const
        {
            return parseErrors.size();
        }


        const char* XSDHelperImpl::getErrorMessage(int errnum) const
        {
            if (errnum >= 0 && errnum < parseErrors.size())
            {
                return parseErrors[errnum];
            }
            return 0;
        }

        void XSDHelperImpl::setError(const char* message)
        {
            if (message == 0) return;
            char * m = new char[strlen(message) + 1];
            strcpy(m,message);
            m[strlen(message)] = 0;
            parseErrors.push_back(m);
        }

        void XSDHelperImpl::clearErrors()
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
