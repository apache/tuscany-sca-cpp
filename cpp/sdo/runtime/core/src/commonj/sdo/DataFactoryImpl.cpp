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

/* $Rev$ $Date: 2006/02/08 14:43:56 $ */

//////////////////////////////////////////////////////////////////////
// DataFactoryImpl.cpp: implementation of the DataFactory class.
//
//////////////////////////////////////////////////////////////////////
#include "commonj/sdo/SDORuntimeException.h"

#include "commonj/sdo/DataFactoryImpl.h"
#include "commonj/sdo/DataObjectImpl.h"

#include "commonj/sdo/Logger.h"


#include "commonj/sdo/PropertyList.h"

#include <iostream>
#include <stdio.h>


using namespace std;
using namespace commonj::sdo;

namespace commonj{
namespace sdo {

// ===================================================================
// Constructor
// ===================================================================
DataFactoryImpl::DataFactoryImpl()
{

    /* add the primitives to every mdg - */

    isResolved = false;
    addType(Type::SDOTypeNamespaceURI,"BigDecimal");
    addType(Type::SDOTypeNamespaceURI,"BigInteger");
    addType(Type::SDOTypeNamespaceURI,"Boolean");

    addType(Type::SDOTypeNamespaceURI,"Byte");
    addType(Type::SDOTypeNamespaceURI,"Bytes");
    addType(Type::SDOTypeNamespaceURI,"Character");

    addType(Type::SDOTypeNamespaceURI,"String");
    addType(Type::SDOTypeNamespaceURI,"DataObject");
    addType(Type::SDOTypeNamespaceURI,"Date");
    addType(Type::SDOTypeNamespaceURI,"Double");
    addType(Type::SDOTypeNamespaceURI,"Float");
    addType(Type::SDOTypeNamespaceURI,"Integer");
    addType(Type::SDOTypeNamespaceURI,"Long");
    addType(Type::SDOTypeNamespaceURI,"Short");
    addType(Type::SDOTypeNamespaceURI,"URI");

    // abstract
    addType(Type::SDOTypeNamespaceURI,"ChangeSummary");

    rootElementName = 0;


}

// ===================================================================
// Destructor
// ===================================================================
DataFactoryImpl::~DataFactoryImpl()
{

    TYPES_MAP::iterator typeIter;
    for (typeIter = types.begin() ; typeIter != types.end() ; ++typeIter)
    {
        if (strncmp((typeIter->first).c_str(),"ALIAS::", 7)) 
        {
            delete typeIter->second;
        }
    }
    if (rootElementName != 0)
    {
        delete rootElementName;
        rootElementName = 0;
    }

}

// ===================================================================
// get the root element name 
// ===================================================================
const char* DataFactoryImpl::getRootElementName() const
{
    return (const char*)rootElementName;
}

// ===================================================================
// set the root element name 
// ===================================================================
void DataFactoryImpl::setRootElementName(const char* ren)
{
    if (rootElementName != 0)
    {
        delete rootElementName;
        rootElementName = 0;
    }
    if (ren != 0 && (strlen(ren) != 0)) 
    {
        rootElementName = new char[strlen(ren) +1];
        strcpy(rootElementName, ren);
    }
}

// ===================================================================
// copy constructor
// ===================================================================
DataFactoryImpl::DataFactoryImpl(const DataFactoryImpl& inmdg)
{
    isResolved = false;
    rootElementName = 0;
    setRootElementName(inmdg.getRootElementName());
    copyTypes(inmdg);
}

// ===================================================================
// Assignment operator
// ===================================================================
DataFactoryImpl& DataFactoryImpl::operator=(const DataFactoryImpl& inmdg)
{
    if (this != &inmdg)
    {
        rootElementName = 0;
        copyTypes(inmdg);
        setRootElementName(inmdg.getRootElementName());
    }
    return *this;
}

// ===================================================================
// copy Types to this DataFactory
// ===================================================================
void DataFactoryImpl::copyTypes(const DataFactoryImpl& inmdg)
{

    if (isResolved)
    {
    SDO_THROW_EXCEPTION("copyTypes",
        SDOUnsupportedOperationException, "Copying Type after data graph completed");
    }

    TYPES_MAP::const_iterator typeIter;
    TYPES_MAP::iterator typeIter2;
    char* fullTypeName;

    for (typeIter = inmdg.types.begin() ; typeIter != inmdg.types.end() ; ++typeIter)
    {
        // add this type to this metadata
        addType((typeIter->second)->getURI(), (typeIter->second)->getName());

        // re-find the type we just added.
        fullTypeName = getFullTypeName(
                (typeIter->second)->getURI(), 
                (typeIter->second)->getName());
        typeIter2 = types.find(fullTypeName);
        if (fullTypeName)delete fullTypeName;

        // copy the aliases , if there are any.

        if ((typeIter->second)->getAliasCount() > 0)
        {
            for (int j=0;j<(typeIter->second)->getAliasCount();j++)
            {
                (typeIter2->second)->setAlias(
                    (typeIter->second)->getAlias());
            }
        }

        
        // Now add all the properties
        PropertyList props = typeIter->second->getProperties();
        for (int i=0; i < props.size(); i++)
        {
            // Ensure the properties type is added
            const Type& propType = props[i].getType();
            addType(propType.getURI(), propType.getName());

            // Now add the property
            addPropertyToType((typeIter->second)->getURI(),
                              (typeIter->second)->getName(),
                              props[i].getName(),
                              propType.getURI(), 
                              propType.getName(),
                              props[i].isMany(),
                              props[i].isReadOnly(),
                              props[i].isContainment());

            // copy the aliases if there are any.
            if (props[i].getAliasCount() > 0) 
            {

                PropertyImpl* p = (typeIter2->second)->
                    getPropertyImpl(props[i].getName());
                if (p != 0)
                {
                    for (int j=0;j<p->getAliasCount();j++)
                    {
                        p->setAlias(props[i].getAlias(j));
                    }
                }

            }

        } // end - iterate over Properties
    } // end - iterate over Types
}

// ===================================================================
// addType - adds a new Type if it does not already exist
// ===================================================================
void DataFactoryImpl::addType(const char* uri, const char* inTypeName, 
                                bool isSeq,
                                bool isOp,
                                bool isAbs,
                                bool isData)
{
    if (isResolved)
    {
    SDO_THROW_EXCEPTION("DataFactory::addType",
        SDOUnsupportedOperationException, "Adding Type after data graph completed");
    }

    if (inTypeName == 0 || strlen(inTypeName) == 0)
    {
    SDO_THROW_EXCEPTION("DataFactory::addType",
        SDOIllegalArgumentException, " Type has empty name");
    }

    
    if (findType(uri, inTypeName) == 0) 
    {
        char* fullTypeName = getFullTypeName(uri, inTypeName);
        types[fullTypeName] = new TypeImpl(uri, inTypeName, isSeq, isOp, isAbs, isData);
        if (fullTypeName)delete fullTypeName;

    }
}

// ===================================================================
//  Check whether a change summary would clash.
// ===================================================================

bool DataFactoryImpl::recursiveCheck(TypeImpl* cs, TypeImpl* t)
{
    if (cs->isDataType()) return false;

    if (! strcmp(cs->getName(), t->getName()) &&
        ! strcmp(cs->getURI() , t->getURI()) )
    {
        return true;
    }

    PropertyList pl = cs->getProperties();
    
    for (int i=0 ; i < pl.size() ; i++ )
    {
        if (recursiveCheck((TypeImpl*)&(pl[i].getType()), t)) return true;
    }
    return false;
}

// ===================================================================
//  Check whether a change summary would clash.
// ===================================================================
bool DataFactoryImpl::checkForValidChangeSummary(TypeImpl* t)
{
    // None of the containing types can have a cs already.
    // None of the properties of this type can hold a type
    // which has a change summary.
    if (isResolved)
    {
    SDO_THROW_EXCEPTION("DataFactory::addChangeSummary",
        SDOUnsupportedOperationException, "Adding Change Summary after data graph completed");
    }

    if (cstypes.size() > 0) {
        for (int i = 0 ;i < cstypes.size(); i++) 
        {
            if (recursiveCheck(cstypes[i], t)) 
            {
                return false;

            }
        }
    }
    cstypes.push_back(t);
    return true;
}

// ===================================================================
// addPropertyToType - adds a Property to an existing Type
// ===================================================================
void DataFactoryImpl::addPropertyToType(const char* uri, 
                                      const char* inTypeName, 
                                      const char* propname,
                                      const char* propTypeUri,
                                      const char* propTypeName,
                                      bool    many)
{
    char* fullPropTypeName = getFullTypeName(propTypeUri, propTypeName);
    TYPES_MAP::iterator typeIter;
    typeIter = types.find(fullPropTypeName);
    if (fullPropTypeName)delete fullPropTypeName;
    if (typeIter != types.end())
    {
        addPropertyToType(uri,inTypeName, 
                                  propname,
                                  propTypeUri,
                                  propTypeName,
                                  many, 
                                  false,
                                  !(typeIter->second)->isDataType());
    }
}

void DataFactoryImpl::addPropertyToType(const char* uri, 
                                      const char* inTypeName, 
                                      const char* propname,
                                      const char* propTypeUri,
                                      const char* propTypeName,
                                      bool    many,
                                      bool    rdonly,
                                      bool cont)
{
    if (isResolved)
    {
    SDO_THROW_EXCEPTION("DataFactory::addPropertyToType",
        SDOUnsupportedOperationException, "Adding Properties after data graph completed");
    }

    TYPES_MAP::iterator typeIter, typeIter2;

    char* fullTypeName = getFullTypeName(uri, inTypeName);
    typeIter = types.find(fullTypeName);
    if (fullTypeName)delete fullTypeName;

    if(typeIter == types.end())
    {
        string msg("Type not found: ");
        if (uri != 0)msg += uri;
        msg += " ";
        if (inTypeName != 0)msg += inTypeName;
        SDO_THROW_EXCEPTION("addPropertyToType",
        SDOTypeNotFoundException, msg.c_str());

    }

    if ((typeIter->second)->isDataType())
    {
        string msg("Cannot add a properties to data types: ");
        msg += (typeIter->second)->getName();
        SDO_THROW_EXCEPTION("addPropertyToType",
        SDOIllegalArgumentException, msg.c_str());
    }

    fullTypeName = getFullTypeName(propTypeUri, propTypeName);
    typeIter2 = types.find(fullTypeName);
    if (fullTypeName)delete fullTypeName;
    
    if (typeIter2 == types.end())
    {
        string msg("Type not found: ");
        if (propTypeUri != 0)msg += propTypeUri;
        msg += " ";
        if (propTypeName != 0)msg += propTypeName;
        SDO_THROW_EXCEPTION("addPropertyToType",
        SDOTypeNotFoundException, msg.c_str());
    }
    
    // Check if its a ChangeSummary
    if (propTypeUri != 0 && !strcmp(propTypeUri,Type::SDOTypeNamespaceURI) &&
        !strcmp(propTypeName,"ChangeSummary") )
    {
        if (checkForValidChangeSummary(typeIter->second)) 
        {
            // The change summary is allowable if we got to here - force the right params.
            // we will not use this property - its just for compatibility.
            // we have to use getChangeSummary to get the change summary, 
            // and isChangeSummaryType to see if this is a type which may have
            // a change summary.
            (typeIter->second)->addChangeSummary();
            // dont even show the property - its not needed
            //((typeIter->second)->addProperty(propname, *(typeIter2->second),false,true, false));

        }
        return;
    }
    

    if ((typeIter->second)->isDataType())
    {
        string msg("Cannot add property to a data type : ");
        msg += (typeIter->second)->getName();
        SDO_THROW_EXCEPTION("addPropertyToType",
        SDOIllegalArgumentException, msg.c_str());
           // cannot add a property to a primitive
    }

    // @PGR@ containment should be ignored for DataType
/*    if ((typeIter2->second)->isDataType() && cont == true)
    {
        string msg("Data types may not be containment : ");
        msg += (typeIter2->second)->getName();
        SDO_THROW_EXCEPTION("addPropertyToType",
        SDOIllegalArgumentException, msg.c_str());
        // cannot try to make a property containment on a data type
    }
*/
    ((typeIter->second)->addProperty(propname, *(typeIter2->second),many,rdonly, cont));
    return;
}

// ===================================================================
// addPropertyToType - adds a Property to an existing Type
// ===================================================================

void DataFactoryImpl::addPropertyToType(const char* uri, 
                                      const char* inTypeName, 
                                      const char* propname,
                                      const Type& tprop,
                                      bool    many)
{
    addPropertyToType(uri, 
                    inTypeName, 
                    propname,
                    tprop,
                    many,
                    false,
                    !tprop.isDataType());
}


void DataFactoryImpl::addPropertyToType(const char* uri, 
                                      const char* inTypeName, 
                                      const char* propname,
                                      const Type& tprop,
                                      bool    many,
                                      bool    rdonly,
                                      bool cont)
{
    addPropertyToType(uri, 
                      inTypeName,
                      propname,
                      tprop.getURI(),
                      tprop.getName(),
                      many,
                      rdonly, cont);
}

// ===================================================================
// addPropertyToType - adds a Property to an existing Type
// ===================================================================
void DataFactoryImpl::addPropertyToType(const Type& cont,
                                      const char* propname,
                                      const char* propTypeUri,
                                      const char* propTypeName,
                                      bool  many)
{
    addPropertyToType(cont.getURI(),
                      cont.getName(),
                      propname,
                      propTypeUri,
                      propTypeName,
                      many);
}

void DataFactoryImpl::addPropertyToType(const Type& cont,
                                      const char* propname,
                                      const char* propTypeUri,
                                      const char* propTypeName,
                                      bool  many,
                                      bool  rdonly,
                                      bool contain)
{
    addPropertyToType(cont.getURI(),
                      cont.getName(),
                      propname,
                      propTypeUri,
                      propTypeName,
                      many,
                      rdonly,
                      contain);
}

// ===================================================================
// addPropertyToType - adds a Property to an existing Type
// ===================================================================
void DataFactoryImpl::addPropertyToType(const Type& tp,
                                      const char* propname,
                                      const Type& tprop,
                                      bool  many)
{
        addPropertyToType(tp.getURI(),
                      tp.getName(),
                      propname,
                      tprop.getURI(),
                      tprop.getName(),
                      many);
}

void DataFactoryImpl::addPropertyToType(const Type& tp,
                                      const char* propname,
                                      const Type& tprop,
                                      bool  many,
                                      bool  rdonly,
                                      bool cont)
{
    addPropertyToType(tp.getURI(),
                      tp.getName(),
                      propname,
                      tprop.getURI(),
                      tprop.getName(),
                      many,
                      rdonly,
                      cont);
}

// ===================================================================
// getFullTypeName - return the name used as a key in the types map
// ===================================================================
char* DataFactoryImpl::getFullTypeName(const char* uri, const char* inTypeName) const
{
    char* c;
    if (uri != 0 && inTypeName != 0) 
    {
        c = new char[strlen(uri) + strlen(inTypeName) + 2];
        sprintf(c,"%s#%s",uri,inTypeName);
        return c;
    }
    if (uri != 0)
    {
        c = new char[strlen(uri) + 2];
        sprintf(c,"%s#",uri);
        return c;
    }
    c = new char[strlen(inTypeName) + 2];
    sprintf(c,"#%s",inTypeName);
    return c;
}

// ===================================================================
// getFullTypeName - return the name used as a key in the types map
// ===================================================================
char* DataFactoryImpl::getAliasTypeName(const char* uri, const char* inTypeName) const
{
    char* c;
    if (uri != 0 && inTypeName != 0) 
    {
        c = new char[strlen(uri) + strlen(inTypeName) + 9];
        sprintf(c,"ALIAS::%s#%s",uri,inTypeName);
        return c;
    }
    if (uri != 0)
    {
        c = new char[strlen(uri) + 9];
        sprintf(c,"ALIAS::%s#",uri);
        return c;
    }
    c = new char[strlen(inTypeName) + 9];
    sprintf(c,"ALIAS::#%s",inTypeName);
    return c;
}

// ===================================================================
// getType - return a pointer to the required Type
// ===================================================================
const Type& DataFactoryImpl::getType(const char* uri, const char* inTypeName) const
{

    const Type* type = findType(uri, inTypeName);

    if (type == 0)
    {
        string msg("Type not found :");
        if (uri != 0) msg += uri;
        msg += " ";
        if (inTypeName != 0) msg += inTypeName;
        SDO_THROW_EXCEPTION("getType" ,
        SDOTypeNotFoundException, msg.c_str());
    }
    
    return *type;
}

// ===================================================================
// setBaseType - sets the type from which this type inherits properties
// ===================================================================

void DataFactoryImpl::setBaseType( const Type& type,
                  const Type& base) 
{
    setBaseType(type.getURI(),type.getName(),base.getURI(), base.getName());
}

// ===================================================================
// setBaseType - sets the type from which this type inherits properties
// ===================================================================

void DataFactoryImpl::setBaseType( const char* typeuri,
                  const char* typenam,
                  const char* baseuri,
                     const char* basename)
{
    const TypeImpl* base = findTypeImpl(baseuri, basename);
    if (base == 0)
    {
        string msg("Type not found :");
        if (baseuri != 0) msg += baseuri;
        msg += " ";
        if (basename != 0) msg += basename;
        SDO_THROW_EXCEPTION("setBaseType" ,
        SDOTypeNotFoundException, msg.c_str());
    }

    TYPES_MAP::const_iterator typeIter;

    char* fullTypeName = getFullTypeName(typeuri, typenam);
    typeIter = types.find(fullTypeName);
    if (fullTypeName)delete fullTypeName;
    
    if(typeIter == types.end())
    {
        string msg("Type not found :");
        if (typeuri != 0) msg += typeuri;
        msg += " ";
        if (typenam != 0) msg += typenam;
        SDO_THROW_EXCEPTION("setBaseType" ,
        SDOTypeNotFoundException, msg.c_str());
    }

    (typeIter->second)->setBaseType(base);
}


// ===================================================================
// setPropertySubstitute - additional type for a property
// ===================================================================

    void DataFactoryImpl::setPropertySubstitute(
            const char* uri, 
            const char* inTypeName,
            const char* propname,
            const char* subname,
            const char* subTypeUri, 
            const char* subTypeName)
    {
        const TypeImpl* cont = findTypeImpl(uri, inTypeName);
        if (cont == 0)
        {
            string msg("Type not found :");
            if (uri != 0) msg += uri;
            msg += " ";
            if (inTypeName != 0) msg += inTypeName;
            SDO_THROW_EXCEPTION("setPropertySubstitute" ,
            SDOTypeNotFoundException, msg.c_str());
        }
        PropertyImpl* pi = cont->getPropertyImpl(propname);
        const Type& tsub = getType(subTypeUri,
                                subTypeName);
        if (pi != 0)pi->setSubstitution(this,subname,tsub);
    }

    

    void DataFactoryImpl::setPropertySubstitute(
            const Type& containertype,
            const char* propname,
            const char* subname,
            const Type& subtype)
    {
        setPropertySubstitute(
            containertype.getURI(),
            containertype.getName(),
            propname,subname,
            subtype.getURI(),subtype.getName());
    }

// ===================================================================
// setDefault - sets the default value for a property of a type
// ===================================================================

    void DataFactoryImpl::setDefault(
        const Type& t, const char* propname, bool b ) 
    {
        setDefault(t.getURI(), t.getName(), propname, b);
    }

    void DataFactoryImpl::setDefault(
        const Type& t, const char* propname , char c) 
        
    {
        setDefault(t.getURI(), t.getName(), propname, c);
    }

    void DataFactoryImpl::setDefault(
        const Type& t, const char* propname , wchar_t c) 
    {
        setDefault(t.getURI(), t.getName(), propname, c);
    }

    void DataFactoryImpl::setDefault(
        const Type& t, const char* propname , char* c) 
    {
        setDefault(t.getURI(), t.getName(), propname, c);
    }

    void DataFactoryImpl::setDefault(
        const Type& t, const char* propname , short s) 
    {
        setDefault(t.getURI(), t.getName(), propname, s);
    }

    void DataFactoryImpl::setDefault(
        const Type& t, const char* propname , long l) 
    {
        setDefault(t.getURI(), t.getName(), propname, l);
    }

    void DataFactoryImpl::setDefault(
        const Type& t, const char* propname , int64_t i) 
    {
        setDefault(t.getURI(), t.getName(), propname, i);
    }

    void DataFactoryImpl::setDefault(
        const Type& t, const char* propname , float f) 
    {
        setDefault(t.getURI(), t.getName(), propname, f);
    }

    void DataFactoryImpl::setDefault(
        const Type& t, const char* propname , long double d) 
    {
        setDefault(t.getURI(), t.getName(), propname, d);
    }

    void DataFactoryImpl::setDefault(
        const Type& t, const char* propname , const SDODate d) 
    {
        setDefault(t.getURI(), t.getName(), propname, d);
    }

    void DataFactoryImpl::setDefault(
        const Type& t, const char* propname , const wchar_t* c, unsigned int len) 
    {
        setDefault(t.getURI(), t.getName(), propname, c, len);
    }

    void DataFactoryImpl::setDefault(
        const Type& t, const char* propname , const char* c, unsigned int len) 
    {
        setDefault(t.getURI(), t.getName(), propname, c, len);
    }


    void DataFactoryImpl::setDefault(
        const char* typuri, const char* typnam, 
        const char* propname, bool b ) 
    {
        const TypeImpl* ti = findTypeImpl(typuri,typnam);
        PropertyImpl* pi = ti->getPropertyImpl(propname);
        if (pi != 0)pi->setDefault(b);
    }

    void DataFactoryImpl::setDefault(
        const char* typuri, const char* typnam, 
        const char* propname , char c) 
    {
        const TypeImpl* ti = findTypeImpl(typuri,typnam);
        PropertyImpl* pi = ti->getPropertyImpl(propname);
        if (pi != 0)pi->setDefault(c);
    }

    void DataFactoryImpl::setDefault(
        const char* typuri, const char* typnam, 
        const char* propname , wchar_t c) 
    {
        const TypeImpl* ti = findTypeImpl(typuri,typnam);
        PropertyImpl* pi = ti->getPropertyImpl(propname);
        if (pi != 0)pi->setDefault(c);
    }

    void DataFactoryImpl::setDefault(
        const char* typuri, const char* typnam, 
        const char* propname , char* c) 
    {
        const TypeImpl* ti = findTypeImpl(typuri,typnam);
        PropertyImpl* pi = ti->getPropertyImpl(propname);
        if (pi != 0)pi->setDefault(c);
    }

    void DataFactoryImpl::setDefault(
        const char* typuri, const char* typnam, 
        const char* propname , short s) 
    {
        const TypeImpl* ti = findTypeImpl(typuri,typnam);
        PropertyImpl* pi = ti->getPropertyImpl(propname);
        if (pi != 0)pi->setDefault(s);
    }

    void DataFactoryImpl::setDefault(
        const char* typuri, const char* typnam, 
        const char* propname , long l) 
    {
        const TypeImpl* ti = findTypeImpl(typuri,typnam);
        PropertyImpl* pi = ti->getPropertyImpl(propname);
        if (pi != 0)pi->setDefault(l);
    }

    void DataFactoryImpl::setDefault(
        const char* typuri, const char* typnam, 
        const char* propname , int64_t i) 
    {
        const TypeImpl* ti = findTypeImpl(typuri,typnam);
        PropertyImpl* pi = ti->getPropertyImpl(propname);
        if (pi != 0)pi->setDefault(i);
    }

    void DataFactoryImpl::setDefault(
        const char* typuri, const char* typnam, 
        const char* propname , float f) 
    {
        const TypeImpl* ti = findTypeImpl(typuri,typnam);
        PropertyImpl* pi = ti->getPropertyImpl(propname);
        if (pi != 0)pi->setDefault(f);
    }

    void DataFactoryImpl::setDefault(
        const char* typuri, const char* typnam, 
        const char* propname , long double d) 
    {
        const TypeImpl* ti = findTypeImpl(typuri,typnam);
        PropertyImpl* pi = ti->getPropertyImpl(propname);
        if (pi != 0)pi->setDefault(d);
    }

    void DataFactoryImpl::setDefault(
        const char* typuri, const char* typnam, 
        const char* propname , const SDODate d) 
    {
        const TypeImpl* ti = findTypeImpl(typuri,typnam);
        PropertyImpl* pi = ti->getPropertyImpl(propname);
        if (pi != 0)pi->setDefault(d);
    }

    void DataFactoryImpl::setDefault(
        const char* typuri, const char* typnam, 
        const char* propname , const wchar_t* c, unsigned int len) 
    {
        const TypeImpl* ti = findTypeImpl(typuri,typnam);
        PropertyImpl* pi = ti->getPropertyImpl(propname);
        if (pi != 0)pi->setDefault(c,len);
    }

    void DataFactoryImpl::setDefault(
        const char* typuri, const char* typnam, 
        const char* propname , const char* c, unsigned int len) 
    {
        const TypeImpl* ti = findTypeImpl(typuri,typnam);
        PropertyImpl* pi = ti->getPropertyImpl(propname);
        if (pi != 0)pi->setDefault(c,len);
    }

    void DataFactoryImpl::setOpposite(
        const Type& typ, 
        const char* propnam, 
        const Type& opptyp,
        const char* opppropnam) 
    {
        SDO_THROW_EXCEPTION("setOpposite" ,
        SDOUnsupportedOperationException, " Not implemented");
    }

// ===================================================================
// getTypeImpl - return a pointer to the required TypeImpl
// ===================================================================
const TypeImpl& DataFactoryImpl::getTypeImpl(const char* uri, const char* inTypeName) const
{
    const TypeImpl* type = findTypeImpl(uri, inTypeName);

    if (type == 0)
    {
        string msg("Type not found :");
        if (uri != 0)msg += uri;
        msg += " ";
        if (inTypeName != 0)msg += inTypeName;
        SDO_THROW_EXCEPTION("getTypeImpl" ,
        SDOTypeNotFoundException, msg.c_str());
    }
    
    return *type;
}

// ===================================================================
// findType
// ===================================================================

const Type* DataFactoryImpl::findType(const char* uri, const char* inTypeName) const
{
    return (Type*)findTypeImpl(uri,inTypeName);
}

// ===================================================================
// findTypeImpl
// ===================================================================

const TypeImpl* DataFactoryImpl::findTypeImpl(const char* uri, const char* inTypeName) const
{
    char* fullTypeName = getFullTypeName(uri, inTypeName);
    TYPES_MAP::const_iterator typeIter;
    typeIter = types.find(fullTypeName);
    if (fullTypeName)delete fullTypeName;
    if(typeIter != types.end())
    {
        return typeIter->second;
    }
    else
    {
        // try alias names
        fullTypeName = getAliasTypeName(uri, inTypeName);
        typeIter = types.find(fullTypeName);
        if (fullTypeName)delete fullTypeName;
        if(typeIter != types.end())
        {
            return typeIter->second;
        }
    }
    return 0;
}

// ===================================================================
// setAlias - sets a new alias for this type
// ===================================================================

void DataFactoryImpl::setAlias(const char* typeuri,
                              const char* typenam,
                              const char* alias)
{

    char* fullTypeName = getFullTypeName(typeuri, typenam);
    TYPES_MAP::iterator typeIter;
    typeIter = types.find(fullTypeName);
    if (fullTypeName)delete fullTypeName;
    if(typeIter != types.end())
    {
        (typeIter->second)->setAlias(alias);
        fullTypeName = getAliasTypeName(typeuri, alias);
        types[fullTypeName] = typeIter->second;
    }

}

// ===================================================================
// setAlias - sets a new alias for this type
// ===================================================================

void DataFactoryImpl::setAlias(const char* typeuri, 
                              const char* typenam, 
                              const char* propname,
                              const char* alias)
{
    const TypeImpl&  t = getTypeImpl(typeuri, typenam);
    PropertyImpl* p  = t.getPropertyImpl(propname); 
    if (p != 0)p->setAlias(alias);

}

// ===================================================================
//  getTypes - gets the full list of types for this factory
// ===================================================================

TypeList DataFactoryImpl::getTypes() const
{
    TYPES_MAP::const_iterator typeIter;
    

    std::vector<const Type*> typeVector;

    for (typeIter = types.begin() ; typeIter != types.end();
    ++typeIter) {
        if (strncmp((typeIter->first).c_str(),"ALIAS::", 7)) {
            typeVector.insert(typeVector.end(),typeIter->second);
        }
    }


    return typeVector;
}


// ===================================================================
//  resolve - resolves the type and includes all the properties from
// the supertype. After this has been called, no further changes
// to the type hierarchy are allowed.
// ===================================================================

void DataFactoryImpl::resolve()
{
    if (isResolved) return; 

    TYPES_MAP::iterator typeIter;
    for (typeIter = types.begin() ; typeIter != types.end();
    ++typeIter) 
    {
        (typeIter->second)->initCompoundProperties();
        (typeIter->second)->validateChangeSummary();
    }

    isResolved = true;
}

// ===================================================================
//  create - creates a data object from the types available.
//  This first resolves the type hierarchy, and thus no further changes
//  to the type hierarchy are allowed.
// ===================================================================


RefCountingPointer<DataObject> DataFactoryImpl::create(const char* uri, const char* typeName) 
{

    if (!isResolved)
    {
        // Allow creation of types and properties before resolve.
        if (uri != 0 && !strcmp(uri,Type::SDOTypeNamespaceURI)) {
            if (!strcmp(typeName,"Type") || !strcmp(typeName,"Property"))
            {
                DataObject* dob = (DataObject*)(new DataObjectImpl(this, getTypeImpl(uri, typeName)));
                return dob;
            }
        }
        resolve();
    }

    const TypeImpl* ti = findTypeImpl(uri,typeName);
    if (ti == 0)
    {
        string msg("Instantiation of unknown type :");
        if (uri != 0) msg += uri;
        msg += " ";
        if (typeName != 0)msg += typeName;
        SDO_THROW_EXCEPTION("create" ,
        SDOTypeNotFoundException, msg.c_str());
    }

    if (ti->isAbstractType())
    {
        string msg("Instantiation of abstract type :");
        if (uri != 0)msg += uri;
        msg += " ";
        if (typeName != 0)msg += typeName;
        SDO_THROW_EXCEPTION("create" ,
        SDOUnsupportedOperationException, msg.c_str());
    }

    DataObject* dob = (DataObject*)(new DataObjectImpl(this, getTypeImpl(uri, typeName)));
    return dob;
}

// ===================================================================
// The openProperties map is a list of the curently present open
// properties as used by this factory. It will cause the 
// open properties to be written out as attributes and elements
// of the root data object when a graph is serialized.
// ===================================================================
    const propertyMap& DataFactoryImpl::getOpenProperties()
    {
        return openProperties;
    }

    void DataFactoryImpl::addOpenProperty(const PropertyImpl& prop)
    {
        propertyMap::iterator i;
        while ((i = openProperties.find(prop.getName())) !=
               openProperties.end())
        {
            openProperties.erase(i);
        }
        openProperties.insert(make_pair(prop.getName(),prop));
    }

    void DataFactoryImpl::removeOpenProperty(const char* name)
    {
        propertyMap::iterator i = 
            openProperties.find(name);
        if (i != openProperties.end())
        {
            openProperties.erase(i);
        }
    }


// ===================================================================
//  create - creates a data object from the types available.
//  This first resolves the type hierarchy, and thus no further changes
//  to the type hierarchy are allowed.
// ===================================================================

RefCountingPointer<DataObject>    DataFactoryImpl::create(const Type& type) 
{
    return create( type.getURI(), type.getName());
}


// ===================================================================
//  setDASValue - Set a value on a Type
// ===================================================================
void DataFactoryImpl::setDASValue(const Type& type,
                                        const char* name, 
                                        DASValue* value)
{
    setDASValue(type.getURI(), type.getName(), name, value);
}


void DataFactoryImpl::setDASValue(const char* typeuri,
                                        const char* typenam, 
                                        const char* name, 
                                        DASValue* value)
{
    TypeImpl* type = (TypeImpl*)findTypeImpl(typeuri, typenam);
    if (type != NULL)
    {
        type->setDASValue(name, value);
    }
}

// ===================================================================
//  getDASValue - retrieve a value from a Type
// ===================================================================

DASValue* DataFactoryImpl::getDASValue(const Type& type,
                                                const char* name) const
{
    return getDASValue(type.getURI(), type.getName(), name);
}

DASValue* DataFactoryImpl::getDASValue(const char* typeuri,
                                                const char* typenam,
                                                const char* name) const
{
    TypeImpl* type = (TypeImpl*)findTypeImpl(typeuri, typenam);
    if (type != NULL)
    {
        return type->getDASValue(name);
    }

    return NULL;    
}

// ===================================================================
//  setDASValue - Set a value on a Property
// ===================================================================
void DataFactoryImpl::setDASValue( 
                const Type& type,
                const char* propertyName,
                const char* name,
                DASValue* value)
{
    setDASValue(type.getURI(), type.getName(), propertyName, name, value);
}


void DataFactoryImpl::setDASValue( 
                const char* typeuri,
                const char* typenam,
                const char* propertyName,
                const char* name,
                DASValue* value)
{
    const TypeImpl* type = findTypeImpl(typeuri, typenam);
    if (type != NULL)
    {
        PropertyImpl* prop = type->getPropertyImplPure(propertyName);
        if (prop != 0)prop->setDASValue(name, value);
    }
}

// ===================================================================
//  getDASValue - retrieve a value from a Property
// ===================================================================
DASValue* DataFactoryImpl::getDASValue( 
                const Type& type,
                const char* propertyName,
                const char* name) const
{
    return getDASValue(type.getURI(), type.getName(), propertyName, name);
}

DASValue* DataFactoryImpl::getDASValue(
                const char* typeuri,
                const char* typenam,
                const char* propertyName, 
                const char* name) const
{
    const TypeImpl* type = findTypeImpl(typeuri, typenam);
    if (type != NULL)
    {
        try
        {
            PropertyImpl* prop = type->getPropertyImpl(propertyName);
            if (prop != 0)return prop->getDASValue(name);
        }
        catch (const SDOPropertyNotFoundException&)
        {
            // Ignore - return null
        }
    }

    return NULL;    
}


};
};

