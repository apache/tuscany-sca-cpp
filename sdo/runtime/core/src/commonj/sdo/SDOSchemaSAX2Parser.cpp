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

/* $Rev$ $Date: 2006/03/01 08:52:41 $ */

#include "commonj/sdo/SDOSchemaSAX2Parser.h"
#include "commonj/sdo/XSDPropertyInfo.h"
#include "commonj/sdo/XSDTypeInfo.h"
#include "commonj/sdo/SDORuntimeException.h"
#include "commonj/sdo/Logging.h"

#include <stdio.h>

namespace commonj
{
    namespace sdo
    {
        
        SDOSchemaSAX2Parser::SDOSchemaSAX2Parser(SchemaInfo& schemaInf,
            ParserErrorSetter* insetter)
            : schemaInfo(schemaInf) ,SAX2Parser(insetter)
        {
            bInSchema = false;
            bInvalidElement = false;
            bInInvalidContent = false;
        }
        
        SDOSchemaSAX2Parser::~SDOSchemaSAX2Parser()
        {
        }
        
        

        // ============================================================================
        // startElementNS
        // ============================================================================
        void SDOSchemaSAX2Parser::startElementNs(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI,
            const SAX2Namespaces& namespaces,
            const SAX2Attributes& attributes)
        {
            LOGINFO_1( INFO,"SchemaParser:startElementNs:%s",(const char*)localname);

            if (URI.equalsIgnoreCase("http://www.w3.org/2001/XMLSchema"))
            {

                ///////////////////////////////////////////////////////////////////////
                // Handle schema
                // Set the URI from the targetNamespace of the xsd:schema element
                // Remember namespace mappings
                // Create the root Type
                ///////////////////////////////////////////////////////////////////////


                if (localname.equalsIgnoreCase("schema"))
                {
                    bInSchema = true;
                    // Handle namespace definitions


                    schemaInfo.getSchemaNamespaces().merge(namespaces);
                    
                    // Handle attributes
                    for (int i=0; i < attributes.size(); i++)
                    {
                        if (attributes[i].getName().equalsIgnoreCase("targetNamespace"))
                        {
                            schemaInfo.setTargetNamespaceURI(attributes[i].getValue());
                        }                        
                    }


                    currentType.uri = schemaInfo.getTargetNamespaceURI();
                    currentType.name = "RootType";
                    currentType.localname="RootType";
                    
                } // end schema handling

                // Handle <import> of other schema
                else if (localname.equalsIgnoreCase("import"))
                {
                    // TODO - import and include treated equally for now - need to
                    // separate out the namespace info for import 
                    startInclude(localname, prefix, URI, namespaces, attributes);
                }

                // Handle <include> of other schema
                else if (localname.equalsIgnoreCase("include"))
                {
                    startInclude(localname, prefix, URI, namespaces, attributes);
                }
                
                ///////////////////////////////////////////////////////////////////////
                // Handle elements and attributes
                // These become Properties of the current Type
                // ?? Any special handling of global elements???
                ///////////////////////////////////////////////////////////////////////
                else if (localname.equalsIgnoreCase("element"))
                {
                    if (!bInInvalidContent) startElement(localname, prefix, URI, namespaces, attributes);
                }
                else if (localname.equalsIgnoreCase("attribute"))
                {
                    if (!bInInvalidContent) startAttribute(localname, prefix, URI, namespaces, attributes);
                }
                else if (localname.equalsIgnoreCase("any")
                        || localname.equalsIgnoreCase("anyAttribute"))
                {
                    // the type containing this is to be created as open
                    if (!bInInvalidContent) currentType.isOpen = true;
                }
                
                
                ///////////////////////////////////////////////////////////////////////
                // Handle complexType
                // These become new types
                ///////////////////////////////////////////////////////////////////////
                else if (localname.equalsIgnoreCase("complexType"))
                {
                    if (!bInInvalidContent) startComplexType(localname, prefix, URI, namespaces, attributes);
                } // end complexType handling
                
                else if (localname.equalsIgnoreCase("choice") 
                    || localname.equalsIgnoreCase("sequence")
                    || localname.equalsIgnoreCase("all"))
                {
                    if (!bInInvalidContent) startGroup(localname, prefix, URI, namespaces, attributes);
                } // end Group handling
                
                else if (localname.equalsIgnoreCase("group") 
                      || localname.equalsIgnoreCase("attributeGroup"))
                {
                    if (setter)
                    {
                        setter->setError("Schema contains a group or attributeGroup which are not yet implemented");
                        
                    }
                }
               else if (localname.equalsIgnoreCase("list") 
                       )
                {
                    if (setter)
                    {
                        setter->setError("Schema contains a list which is not yet implemented");
                        
                    }
                }

               ///////////////////////////////////////////////////////////////////////
                // Handle simpleType
                // These become new types
                ///////////////////////////////////////////////////////////////////////
                else if (localname.equalsIgnoreCase("simpleType"))
                {
                    if (!bInInvalidContent) startSimpleType(localname, prefix, URI, namespaces, attributes);
                } // end complexType handling
                
                else if (localname.equalsIgnoreCase("restriction"))
                {
                    if (!bInInvalidContent) startRestriction(localname, prefix, URI, namespaces, attributes);
                }
                
                else if (localname.equalsIgnoreCase("extension"))
                {
                    if (!bInInvalidContent) startExtension(localname, prefix, URI, namespaces, attributes);
                }
                // Handle <import> of other schema
                else if (localname.equalsIgnoreCase("union"))
                {
                    // TODO - unions not yet supported
                    bInInvalidContent = true;
                    if (setter)
                    {
                        setter->setError("Schema contains a union which is not yet implemented");
                    }
                }
            }
            else // not in schema - check for any extra namespaces
            {
                schemaInfo.getSchemaNamespaces().merge(namespaces);
            }
            
        }            
        
        
        // ============================================================================
        // endElementNs
        // ============================================================================            
        void SDOSchemaSAX2Parser::endElementNs(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI)
        {
            LOGINFO_1( INFO,"SchemaParser:endElementNs:%s",(const char*)localname);

            if (URI.equalsIgnoreCase("http://www.w3.org/2001/XMLSchema"))
            {
                if (localname.equalsIgnoreCase("schema"))
                {
                    bInSchema = false;
                }

                ///////////////////////////////////////////////////////////////////////
                // Handle complexType
                // Pop the Type off our stack
                ///////////////////////////////////////////////////////////////////////

                // We do not support unions, so all inside a union, plus the containing 
                // element are invalid for now.
                //
                if (!bInInvalidContent)
                {
                    if (localname.equalsIgnoreCase("complexType"))
                    {
                        if (!bInvalidElement) defineType();
                    } // end complexType handling
                    else if (localname.equalsIgnoreCase("simpleType"))
                    {
                        if (!bInvalidElement) defineType();
                    } 
                
                    else if (localname.equalsIgnoreCase("schema"))
                    {
                        if (!bInvalidElement) defineType();
                    } // end complexType handling
                
                    else if (localname.equalsIgnoreCase("element")
                        || localname.equalsIgnoreCase("attribute"))
                    {
                        // PropertyDefinition should now be complete
                         if (!bInvalidElement) defineProperty();
                    } 
                    else if (localname.equalsIgnoreCase("choice") 
                        || localname.equalsIgnoreCase("sequence")
                        || localname.equalsIgnoreCase("all"))
                    {
                        if (!bInvalidElement) currentType.isMany = false;
                    }
                    bInvalidElement = false;
    
                } // bInUnsupportedContent
                if (localname.equalsIgnoreCase("union"))
                {
                    bInInvalidContent = false;
                    // the enclosing element is not useful
                    bInvalidElement = true; 
                }
            } 

        }
        
        
        // ============================================================================
        // startInclude
        // ============================================================================
        void SDOSchemaSAX2Parser::startInclude(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI,
            const SAX2Namespaces& namespaces,
            const SAX2Attributes& attributes)
        {        
            LOGINFO_1( INFO,"SchemaParser:startInclude:%s",(const char*)localname);

            if (!bInSchema) return;

            SDOXMLString schemaLocation = attributes.getValue("schemaLocation");
            if (!schemaLocation.isNull())
            {
                SchemaInfo schemaInf;
                SDOSchemaSAX2Parser schemaParser(schemaInf, (ParserErrorSetter*)setter);

                try 
                {
                    SDOXMLString sl = getCurrentFile();
                    FILE *f;
                    bool bprocessed = false;

                    if (!sl.isNull()) 
                    {
                        int i = sl.lastIndexOf('/');
                        if (i < 0)i = sl.lastIndexOf('\\');
                        if (i >= 0)
                        {
                            sl = sl.substring(0,i+1) + schemaLocation;
                            // first attempt, relative path plus the location
                            f = fopen(sl,"r+");
                            if (f != NULL)
                            {
                                fclose(f);
                                schemaParser.parse(sl);
                                bprocessed = true;
                            }
                            else // didnt find the file  
                            {
                                int j = schemaLocation.lastIndexOf('/');
                                if (j < 0)j = schemaLocation.lastIndexOf('\\');
                                if (j >= 0) 
                                {
                                    sl = sl.substring(0,i+1) + 
                                     schemaLocation.substring(0,j+1);
                                    f = fopen(sl,"r+");
                                    if (f != NULL)
                                    {
                                        fclose(f);
                                        schemaParser.parse(sl);
                                        bprocessed = true;
                                    }
                                }
                            }
                        }
                    }
                    if (!bprocessed)
                    {
                        schemaParser.parse(schemaLocation);
                    }
                }

                catch (SDOFileNotFoundException e)
                {
                    // finally give up - its not in the current path, or
                    // in the path specified
                    return;
                }


                TypeDefinitions& typedefs = schemaParser.getTypeDefinitions();
                XMLDAS_TypeDefs types = typedefs.types;
                XMLDAS_TypeDefs::iterator iter;
                for (iter=types.begin(); iter != types.end(); iter++)
                {    
                    if ((*iter).second.name.equals("RootType")
                        && currentType.name.equals("RootType")
                        &&  (*iter).second.uri.equals(currentType.uri))
                    {
                        // This must be true for an import/include to be
                        // legally positioned

                        XMLDAS_TypeDefs::iterator find = typeDefinitions.types.find(
                                (*iter).first);

                        std::list<PropertyDefinition>::iterator propit;
                        std::list<PropertyDefinition>::iterator currpropit;
                        bool found;

                        for (propit = (*iter).second.properties.begin() ; 
                             propit != (*iter).second.properties.end(); ++ propit)
                        {
                                found = false;
                             // do not merge properties whose names clash
                             for ( currpropit = currentType.properties.begin();
                                   currpropit != currentType.properties.end();
                                   ++currpropit)
                             {
                                 if ((*currpropit).name.equals((*propit).name))
                                 {
                                     found = true;
                                     break;
                                 }
                             }
                             if (!found) 
                             {
                                currentType.properties.insert(
                                     currentType.properties.end(),*propit);
                             }
                        }
                    }
                    else 
                    {
                        typeDefinitions.types.insert(*iter);
                    }
                }
            }                
        }

        
        // ============================================================================
        // startElement
        // ============================================================================
        void SDOSchemaSAX2Parser::startElement(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI,
            const SAX2Namespaces& namespaces,
            const SAX2Attributes& attributes)
        {

            if (!bInSchema) return;

            PropertyDefinition thisProperty;

            LOGINFO_1( INFO,"SchemaParser:startElement:%s",(const char*)localname);

            thisProperty.isElement =  true;
            
            setName(attributes,
                thisProperty.name,
                thisProperty.localname);

            
            setType(thisProperty, attributes, namespaces);
            
            SDOXMLString maxOccurs = attributes.getValue("maxOccurs");
            if (!maxOccurs.isNull())
            {
                if (!maxOccurs.equalsIgnoreCase("1"))
                {
                    thisProperty.isMany = true;
                }
            }

            // find aliases
            thisProperty.aliases = attributes.getValue("aliasName");
            
            // mark this as a substitution group.
            // TODO - what about properties which have already been set into the tree,
            // and might have a substitution?

            SDOXMLString substituteName = attributes.getValue("substitutionGroup");
            if (! substituteName.isNull())
            {
                XMLQName qname = resolveTypeName(
                substituteName,
                namespaces,
                thisProperty.substituteUri,
                thisProperty.substituteName);
                thisProperty.isSubstitute=true;
            }

            // count the number of elements in the group
            if (currentType.isMany)
            {
                currentType.groupElementCount++;
            }
            
            setCurrentProperty(thisProperty);    
            
        }
        
        // ============================================================================
        // startAttribute
        // ============================================================================
        void SDOSchemaSAX2Parser::startAttribute(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI,
            const SAX2Namespaces& namespaces,
            const SAX2Attributes& attributes)
        {

            LOGINFO_1( INFO,"SchemaParser:startAttribute:%s",(const char*)localname);

            if (!bInSchema) return;

            PropertyDefinition thisProperty;
            
            thisProperty.isElement =  false;
            
            setName(attributes,
                thisProperty.name,
                thisProperty.localname);
            
            setType(thisProperty, attributes, namespaces);
            
            setCurrentProperty(thisProperty);                    
        }
        
        // ============================================================================
        // startComplexType
        // ============================================================================
        void SDOSchemaSAX2Parser::startComplexType(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI,
            const SAX2Namespaces& namespaces,
            const SAX2Attributes& attributes)
        {
            LOGINFO_1( INFO,"SchemaParser:startComplexType:%s",(const char*)localname);

            if (!bInSchema) return;

            TypeDefinition thisType; // set defaults
            thisType.uri=schemaInfo.getTargetNamespaceURI();
            
            setTypeName(thisType, attributes,namespaces);


            
            for (int i=0; i < attributes.size(); i++)
            {
                // If sdo:sequence="true" or mixed="true" it is sequenced
                if ( (attributes[i].getUri().equalsIgnoreCase("commonj.sdo/xml")
                    && attributes[i].getName().equalsIgnoreCase("sequence"))
                    || attributes[i].getName().equalsIgnoreCase("mixed"))
                {    
                    if (attributes[i].getValue().equals("true"))
                    {
                        thisType.isSequenced = true;
                    }
                }
                // If abstract="true" it is abstract
                else if (attributes[i].getName().equalsIgnoreCase("abstract"))
                {    
                    if (attributes[i].getValue().equals("true"))
                    {
                        thisType.isAbstract = true;
                    }
                }
                else if (attributes[i].getName().equalsIgnoreCase("aliasName"))
                {
                    thisType.aliases = attributes[i].getValue();
                }
            }
            
            setCurrentType(thisType);                
        }
        
        // ============================================================================
        // startSimpleType
        // ============================================================================
        void SDOSchemaSAX2Parser::startSimpleType(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI,
            const SAX2Namespaces& namespaces,
            const SAX2Attributes& attributes)
        {
            LOGINFO_1( INFO,"SchemaParser:startSimpleType:%s",(const char*)localname);

            if (!bInSchema) return;

            TypeDefinition thisType; // set defaults
            thisType.uri=schemaInfo.getTargetNamespaceURI();
            thisType.dataType = true;
            
            for (int i=0; i < attributes.size(); i++)
            {
                // If abstract="true" it is abstract
                if (attributes[i].getName().equalsIgnoreCase("abstract"))
                {    
                    if (attributes[i].getValue().equals("true"))
                    {
                        thisType.isAbstract = true;
                    }
                }
                else if (attributes[i].getName().equalsIgnoreCase("aliasName"))
                {
                    thisType.aliases = attributes[i].getValue();
                }
            }
            setTypeName(thisType, attributes,namespaces);

            // see if the type is going to be abstract...

            
            setCurrentType(thisType);                
        }
        
        // ============================================================================
        // startRestriction
        // ============================================================================
        void SDOSchemaSAX2Parser::startRestriction(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI,
            const SAX2Namespaces& namespaces,
            const SAX2Attributes& attributes)
        {
            LOGINFO_1( INFO,"SchemaParser:startRestriction:%s",(const char*)localname);

            if (!bInSchema) return;

            SDOXMLString base = attributes.getValue("base");
            if (!base.isNull())
            {
                // Resolve typename to uri:name
                XMLQName qname = resolveTypeName(
                    base,
                    namespaces,
                    currentType.parentTypeUri,
                    currentType.parentTypeName);

                currentType.isRestriction=true;


                if(qname.getLocalName().equals("QName"))
                {
                    currentType.isQName = true;
                }                    
            }
        }
        
        // ============================================================================
        // startExtension
        // ============================================================================
        void SDOSchemaSAX2Parser::startExtension(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI,
            const SAX2Namespaces& namespaces,
            const SAX2Attributes& attributes)
        {
            LOGINFO_1( INFO,"SchemaParser:startExtension:%s",(const char*)localname);

            if (!bInSchema) return;

            SDOXMLString base = attributes.getValue("base");
            if (!base.isNull())
            {
                SDOXMLString typeUri;
                SDOXMLString typeName;
                // Resolve typename to uri:name
                XMLQName qname = resolveTypeName(
                    base,
                    namespaces,
                    typeUri,
                    typeName);
                
                // If extending a simple type (an SDO DataType) we create a 
                // Property named "value" of this type rather than set the
                // simple type as a base
                currentType.isRestriction=false;
                
                // ?? Does this only apply within a <simpleContent> tag??
                if (typeUri.equalsIgnoreCase(Type::SDOTypeNamespaceURI))
                {
                    // here the type needs to be flagged so that 
                    // we know to serialize this property as an element with
                    // just the contents <element>abc</element> rather than
                    // value=abc


                    PropertyDefinition thisProperty;
                    thisProperty.name = "value";
                    thisProperty.localname = "value";
                    thisProperty.typeUri = typeUri; 
                    thisProperty.typeName = typeName; 
                    thisProperty.fullTypeName = base; 
                    thisProperty.isContainment = false;
                    thisProperty.isElement=true;

                    currentType.isExtendedPrimitive= true;
                                            
                    if(qname.getLocalName().equals("QName"))
                    {
                        thisProperty.isQName = true;
                    }                    

                    setCurrentProperty(thisProperty);
                    defineProperty();
                } 
                else
                {
                    currentType.parentTypeUri = typeUri;
                    currentType.parentTypeName = typeName;
                }
            }
        }
        
        // ============================================================================
        // startGroup
        // ============================================================================
        void SDOSchemaSAX2Parser::startGroup(
            const SDOXMLString& localname,
            const SDOXMLString& prefix,
            const SDOXMLString& URI,
            const SAX2Namespaces& namespaces,
            const SAX2Attributes& attributes)
        {
            LOGINFO_1( INFO,"SchemaParser:startGroup:%s",(const char*)localname);

            if (!bInSchema) return;

            SDOXMLString maxOccurs = attributes.getValue("maxOccurs");
            if (!maxOccurs.isNull())
            {
                if (!maxOccurs.equalsIgnoreCase("1"))
                {
                    currentType.isMany = true;
                }
            }
        }
        
        // ============================================================================
        // setCurrentType
        // ============================================================================
        void SDOSchemaSAX2Parser::setCurrentType(const TypeDefinition& type)
        {                
            typeStack.push(currentType);
            currentType = type;
        }
        
        // ============================================================================
        // defineType
        // ============================================================================
        void SDOSchemaSAX2Parser::defineType()
        {    
            // Set this Type as sequenced of more than one element in a group definition
            if (currentType.groupElementCount > 1)
            {
                currentType.isSequenced = true;
            }
            
            SDOXMLString typeQname = TypeDefinitions::getTypeQName(currentType.uri, currentType.localname);
            typeDefinitions.types[typeQname] = currentType;
            
            if (currentProperty.typeName.isNull())
            {
                // Set the type name to the name of this type
                currentProperty.typeUri = currentType.uri;
                currentProperty.typeName = currentType.localname;
            }
            
            if (typeStack.size() != 0)
            {
                currentType = typeStack.top();                
                typeStack.pop();
            }
            else
            {
                currentType = TypeDefinition();
            }
        }
        
        // ============================================================================
        // setCurrentProperty
        // ============================================================================
        void SDOSchemaSAX2Parser::setCurrentProperty(const PropertyDefinition& prop)
        {                
            propertyStack.push(currentProperty);
            currentProperty = prop;
        }
        
        // ============================================================================
        // defineProperty
        // ============================================================================
        void SDOSchemaSAX2Parser::defineProperty()
        {    

            if (currentProperty.typeName.isNull())
            {
                // Set the type of this property to default (sdo:String)
                currentProperty.typeUri = Type::SDOTypeNamespaceURI;
                currentProperty.typeName = "String";
            }

            if (currentProperty.localname.isNull())
                currentProperty.localname = currentProperty.typeName;

            // Set isMany if property isMany OR if containing type isMany
            // NOTE: The above comment is as per the SDO2.0 spec however this does not
            // work when serializing a sequence containing a single-valued property and
            // then deserializing.
            // currentProperty.isMany = currentProperty.isMany || currentType.isMany;
            
            
            currentType.properties.insert(currentType.properties.end(), currentProperty);
            if (propertyStack.size() != 0)
            {
                currentProperty = propertyStack.top();                
                propertyStack.pop();
            }
            else
                currentProperty = PropertyDefinition();

        }
        
        // ============================================================================
        // setDefault
        // ============================================================================
        void SDOSchemaSAX2Parser::setDefault(
            PropertyDefinition& thisProperty,
            const SAX2Attributes& attributes)
        {
            thisProperty.defaultValue = attributes.getValue("fixed");
            if (!thisProperty.defaultValue.isNull())
            {
                thisProperty.isReadOnly = true;
            }
            else
            {
                thisProperty.defaultValue = attributes.getValue("default");
            }
        }

        // ============================================================================
        // setName
        // ============================================================================
        void SDOSchemaSAX2Parser::setName(
            const SAX2Attributes& attributes,
            SDOXMLString& sdoname,
            SDOXMLString& localname)
        {
            for (int i=0; i < attributes.size(); i++)
            {
                // Handle sdo: annotations
                if (attributes[i].getUri().equalsIgnoreCase("commonj.sdo/xml"))
                {
                    // sdo:name overrides the property name
                    if (attributes[i].getName().equalsIgnoreCase("name"))
                    {
                        sdoname = attributes[i].getValue();
                    }
                }
                else
                {
                    
                    if (attributes[i].getName().equalsIgnoreCase("name"))
                    {
                        localname  = attributes[i].getValue();
                        // If name is already set it must have been an 
                        // override using sdo:name
                        if (sdoname.isNull())
                        {
                            sdoname  = localname;
                        }
                    }
                }
            }

        }
        
        // ============================================================================
        // setType 
        // ============================================================================
        void SDOSchemaSAX2Parser::setType(
            PropertyDefinition& property,
            const SAX2Attributes& attributes,
            const SAX2Namespaces& namespaces)
        {
            property.fullLocalTypeName = attributes.getValue("type");
            if (!property.fullLocalTypeName.isNull())
            {
                XMLQName qname(property.fullLocalTypeName,schemaInfo.getSchemaNamespaces(), namespaces);
                if (qname.getLocalName().equals("IDREF")
                    || qname.getLocalName().equals("IDREFS"))
                {
                    property.fullTypeName = attributes.getValue("commonj.sdo/xml","propertyType");
                    
                    property.isIDREF = true;
                    property.isContainment = false;
                    
                    if (qname.getLocalName().equals("IDREFS"))
                    {
                        property.isMany = true;
                    }
                }
                else if (qname.getLocalName().equals("anyURI"))
                {
                    property.fullTypeName = attributes.getValue("commonj.sdo/xml","propertyType");
                }
                else if (qname.getLocalName().equals("ID"))
                {
                    property.isID = true;
                    currentType.IDPropertyName = property.name;
                }

                else
                {
                    property.fullTypeName = attributes.getValue("commonj.sdo/xml","dataType");
                }
            }
            
            else 
            {
                property.fullLocalTypeName = attributes.getValue("ref");
                if (!property.fullLocalTypeName.isNull())
                {
                    property.isReference = true;
                }
            }
            
            if (property.fullTypeName.isNull())
            {
                property.fullTypeName = property.fullLocalTypeName;
            }
            
            if (!property.fullTypeName.isNull())
            {
                // Resolve typename to uri:name
                XMLQName qname = resolveTypeName(
                    property.fullTypeName,
                    namespaces,
                    property.typeUri,
                    property.typeName);

                if(qname.getLocalName().equals("QName"))
                {
                    property.isQName = true;
                }                    
            }
            
        }
        
        // ============================================================================
        // setTypeName
        // ============================================================================
        void SDOSchemaSAX2Parser::setTypeName(
            TypeDefinition& type,
            const SAX2Attributes& attributes,
            const SAX2Namespaces& namespaces)
        {
            std::map<SDOXMLString,SDOXMLString>::iterator it;
            setName(attributes, type.name, type.localname);
            // If localname is not set it is anonymous so use the enclosing element name


            if (type.localname.isNull())
            {
                type.localname = currentProperty.name;
            }
            else
            {
                // check for an overriding url in the name
                int i;
                if ((i = type.localname.firstIndexOf(':')) > 0)
                {
                    XMLQName qname(type.localname, 
                    schemaInfo.getSchemaNamespaces(),
                    namespaces);
                    type.uri = qname.getURI();
                    type.localname = qname.getLocalName();
                }
            }
            
            // Set SDO name if not specified
            if (type.name .isNull())
            {
                type.name  = type.localname ;
            }
            else
            {
                // check for an overriding url in the name
                int i;
                if ((i = type.name.firstIndexOf(':')) > 0)
                {
                    XMLQName qname(type.name, 
                    schemaInfo.getSchemaNamespaces(),
                    namespaces);
                    type.uri = qname.getURI();
                    type.name = qname.getLocalName();
                }
            }
        }
        
        std::istream& operator>>(std::istream& input, SDOSchemaSAX2Parser& parser)
        {
            parser.stream(input);
            
            return input;
        }
        
        std::istringstream& operator>>(std::istringstream& input, SDOSchemaSAX2Parser& parser)
        {
            parser.stream(input);
            
            return input;
        }

        // ============================================================================
        // resolveTypeName
        // ============================================================================
        XMLQName SDOSchemaSAX2Parser::resolveTypeName(
            const SDOXMLString& fullTypeName,
            const SAX2Namespaces& namespaces,
            SDOXMLString& uri,
            SDOXMLString& name) 
        {
            XMLQName qname(fullTypeName, 
                schemaInfo.getSchemaNamespaces(),
                namespaces);

            uri = qname.getURI();
            name = qname.getLocalName();

            ///////////////////////////////////////////////////////////////////////
            // Map the xsd types to SDO Types
            ///////////////////////////////////////////////////////////////////////
            if (qname.getURI().equalsIgnoreCase("http://www.w3.org/2001/XMLSchema"))
            {
                uri = Type::SDOTypeNamespaceURI;
                if (qname.getLocalName().equalsIgnoreCase("ID"))
                {
                    name = "String";
                }
                if (qname.getLocalName().equalsIgnoreCase("NCName"))
                {
                    name = "String";
                }
                if (qname.getLocalName().equalsIgnoreCase("string"))
                {
                    name = "String";
                }
                else if (qname.getLocalName().equalsIgnoreCase("anyType"))
                {
                    name = "DataObject";
                }
                else if (qname.getLocalName().equalsIgnoreCase("int"))
                {
                    name = "Integer";
                }
                else if (qname.getLocalName().equalsIgnoreCase("integer"))
                {
                    name = "Integer";
                }
                else if (qname.getLocalName().equalsIgnoreCase("negativeInteger"))
                {
                    name = "Integer";
                }
                else if (qname.getLocalName().equalsIgnoreCase("nonNegativeInteger"))
                {
                    name = "Integer";
                }
                else if (qname.getLocalName().equalsIgnoreCase("positiveInteger"))
                {
                    name = "Integer";
                }
                else if (qname.getLocalName().equalsIgnoreCase("nonPositiveInteger"))
                {
                    name = "Integer";
                }
                else if (qname.getLocalName().equalsIgnoreCase("unsignedLong"))
                {
                    name = "Integer";
                }
                else if (qname.getLocalName().equalsIgnoreCase("unsignedShort"))
                {
                    name = "Integer";
                }
                else if (qname.getLocalName().equalsIgnoreCase("unsignedInt"))
                {
                    name = "Long";
                }
                else if (qname.getLocalName().equalsIgnoreCase("long"))
                {
                    name = "Long";
                }
                else if (qname.getLocalName().equalsIgnoreCase("double"))
                {
                    name = "Double";
                }
                else if (qname.getLocalName().equalsIgnoreCase("short"))
                {
                    name = "Short";
                }
                else if (qname.getLocalName().equalsIgnoreCase("unsignedByte"))
                {
                    name = "Short";
                }
                else if (qname.getLocalName().equalsIgnoreCase("float"))
                {
                    name = "Float";
                }
                else if (qname.getLocalName().equalsIgnoreCase("boolean"))
                {
                    name = "Boolean";
                }
                else if (qname.getLocalName().equalsIgnoreCase("byte"))
                {
                    name = "Byte";
                }
                else if (qname.getLocalName().equalsIgnoreCase("base64Binary"))
                {
                    name = "Bytes";
                }
                else if (qname.getLocalName().equalsIgnoreCase("hexBinary"))
                {
                    name = "Bytes";
                }
                else if (qname.getLocalName().equalsIgnoreCase("anyURI"))
                {
                    name = "URI";
                }
                else if (qname.getLocalName().equalsIgnoreCase("QName"))
                {
                    name = "URI";
                }
                else
                {
                    // Default unknown xs: types to string??
                    name = "String";
                }
            }
            
            // Temporary hack: ChangeSummaryType is ChangeSummary in core
            else if (qname.getURI().equalsIgnoreCase(Type::SDOTypeNamespaceURI))
            {
                if (qname.getLocalName().equalsIgnoreCase("ChangeSummaryType"))
                {
                    name = "ChangeSummary";
                }
                
            }
            else if (qname.getURI().isNull())
            {
                uri = schemaInfo.getTargetNamespaceURI();
            }

            return qname;
        }
        
    } // End - namespace sdo
    
} // End - namespace commonj

