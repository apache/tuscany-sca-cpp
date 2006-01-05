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

#include "commonj/sdo/Logger.h"

#include "commonj/sdo/PropertyImpl.h"
#include "commonj/sdo/TypeImpl.h"

#include <iostream>
using namespace std;

#include <list>
#include <vector>

#include "commonj/sdo/SDORuntimeException.h"

namespace commonj{
namespace sdo{

 

    char* TypeImpl::types[num_types] = {
    "Other Types",
    "BigDecimal",
    "BigInteger",
    "Boolean"   , 
    "Byte",
    "Bytes",
    "Character",
    "Date"      ,
    "Double"    ,
    "Float"     ,
    "Integer"   ,
    "Long"      ,
    "Short"     ,
    "String"    ,
    "URI"        ,
    "DataObject",
    "ChangeSummary",
    "Text"
     };


    ///////////////////////////////////////////////////////////////////////////
    //
    ///////////////////////////////////////////////////////////////////////////

    void TypeImpl::addChangeSummary(void) 
    {
        changeSummaryType = true;
    }

    bool TypeImpl::isChangeSummaryType() const
    {
        return changeSummaryType;
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    ///////////////////////////////////////////////////////////////////////////

    bool TypeImpl::isDataType(void) const
    {
        return isPrimitive;
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    ///////////////////////////////////////////////////////////////////////////

    TypeImpl::TypeImpl(const TypeImpl& t)  
    {
        isResolving = false;
        isResolved = false;
    }

     TypeImpl::TypeImpl(const Type* base, const char* uri, 
         const char* inname, 
         bool isSeq,
         bool isOp,
         bool isAbs,
         bool isData) 
     {
        init(uri,inname,isSeq,isOp, isAbs, isData);
        baseType = (TypeImpl*)base;
     }

     TypeImpl::TypeImpl(const char* uri, const char* inname, 
         bool isSeq,
         bool isOp,
         bool isAbs,
         bool isData) 
         
     {
        init(uri,inname,isSeq,isOp,isAbs, isData);
        baseType = 0;
     }

     void TypeImpl::init(const char* uri, const char* inname, 
         bool isSeq,
         bool isOp,
         bool isAbs,
         bool isData)
     {
        isResolving = false;
        isResolved = false;
        localPropsSize = 0;
        changeSummaryType = false;
        isSequenced = isSeq;
        isOpen = isOp;
        isAbstract = isAbs;
        isPrimitive = isData;
        name = new char[strlen(inname)+1];
        strcpy(name,inname);

        if (uri == 0)
        {
            typeURI = new char[1];
            typeURI[0] = 0;
            typeEnum = DataObjectType;
            return;
        }

        typeURI = new char[strlen(uri)+1];
        strcpy(typeURI,uri);

        if (!strcmp(uri,Type::SDOTypeNamespaceURI))  {
            for (int i = 0; i < num_types ; i++) {
                if (!strcmp(inname,types[i])) {
                    typeEnum = (Types)i;
                    if (i <= UriType) isPrimitive = true;
                    // Note - you could create Text, but it would
                    // not be recognised as primitive.
                    return;
                }
            }
        }

        // All other types are data object types - for now 
        typeEnum = DataObjectType;
     }


    ///////////////////////////////////////////////////////////////////////////
    //
    ///////////////////////////////////////////////////////////////////////////
    TypeImpl::TypeImpl()
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    ///////////////////////////////////////////////////////////////////////////

    TypeImpl::~TypeImpl()
    {
         // Types free their corresponding properties, but not the types
         // of those properties 
         // Need to free only those we allocated
         unsigned int count = 0, start = props.size() - localPropsSize;
         std::list<PropertyImpl*>::iterator i;
         for (i= props.begin(); i != props.end() ; ++i)
         {
             if (count >= start) { 
                 delete(*i);
             }
             count++;
         }
         if (name != 0)delete name;
         if (typeURI != 0) delete typeURI;
         for (int j = 0; j < aliases.size();j++)
         {
             delete aliases[j];
         }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Sets a data type as sequenced.
    ///////////////////////////////////////////////////////////////////////////

    void TypeImpl::setSequenced(bool set) 
    {
          isSequenced = set;
    }
 
    ///////////////////////////////////////////////////////////////////////////
    // Sets a data type as open.
    ///////////////////////////////////////////////////////////////////////////

    void TypeImpl::setOpen(bool set) 
    {
          isOpen = set;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Sets a data type as open.
    ///////////////////////////////////////////////////////////////////////////

    void TypeImpl::setBaseType(const Type* bt) 
    {
        baseType = (TypeImpl*)bt;
        
        // DataType and Sequenced must be the same as the base Type
        isPrimitive = baseType->isPrimitive;
        if (isPrimitive)
        {
            typeEnum = baseType->typeEnum;
        }

        if (isSequenced && !baseType->isSequenced)
        {
            // Its an error to be sequenced and inherit from a
            // non-sequenced type 
            string msg("Cannot be sequenced and inherit from a non-sequenced base type:");
            msg += getURI();
            msg += " ";
            msg += getName();
            SDO_THROW_EXCEPTION("setBaseType", 
            SDOUnsupportedOperationException, 
            msg.c_str());
        }

        isSequenced = baseType->isSequenced; 
        // if the base is open then this type must be open too.

        if (baseType->isOpenType())
        {
            isOpen = true;
        }
    }

    bool TypeImpl::equals(const Type& tother) const
    {
        if (strcmp(typeURI,tother.getURI())) return false;
        // TODO - aliases
        if (strcmp(name, tother.getName())) return false;
        return true;
    }


    const Type* TypeImpl::getBaseType() const 
    {
        return (Type*)baseType;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Returns the name of the type.
    ///////////////////////////////////////////////////////////////////////////

    const char* TypeImpl::getName() const
    {
          return name;
    }
  
    void TypeImpl::setAlias(const char* alias)
    {
        char* tmp = new char[strlen(alias)+1];
        strcpy(tmp,alias);
        aliases.push_back(tmp); 
    }


    

    const char* TypeImpl::getAlias(unsigned int index) const
    {
        if (index < aliases.size())
        {
            return aliases[index];
        }
        return name;
    }

    unsigned int TypeImpl::getAliasCount() const
    {
        return aliases.size();
    }
    ///////////////////////////////////////////////////////////////////////////
    // Returns the namespace URI of the type.
    ///////////////////////////////////////////////////////////////////////////
    const char* TypeImpl::getURI() const
    {
          return typeURI;
    }

    void TypeImpl::throwIfNestedChangeSummary() const
    {
        PROPERTY_LIST::const_iterator i;
        for (i = props.begin(); i != props.end(); ++i)
        {

            const TypeImpl *t = ((*i)->getTypeImpl());
            if (t != 0)
            {
                if (t->isDataObjectType())
                {
                    if (t->isChangeSummaryType())
                    {
                        string msg(" Nested change summary type:");
                        msg += t->getURI();
                        msg += " ";
                        msg += t->getName();
                        SDO_THROW_EXCEPTION("resolve", 
                        SDOUnsupportedOperationException, 
                        msg.c_str());
                    }
                    t->throwIfNestedChangeSummary();
                }
            }
        }
    }

    void TypeImpl::validateChangeSummary()
    {
        if (!isChangeSummaryType()) return;
        throwIfNestedChangeSummary();
    }

    PROPERTY_LIST TypeImpl::getCompoundProperties()
    {
        return props;
    }

    void TypeImpl::initCompoundProperties()
    {
        // We have already met this type in the resolution heirarchy
        if (isResolving) 
        {
            string msg(" circular type dependency on:");
            msg += getURI();
            msg += " ";
            msg += getName();

            SDO_THROW_EXCEPTION("initCompoundProperties", 
            SDOUnsupportedOperationException, 
            msg.c_str());
        }

        if (isResolved)
        {
            return;
        }

        isResolving = true;
        

        if (baseType != 0) 
        {
            baseType->initCompoundProperties();
            PROPERTY_LIST pl = baseType->getCompoundProperties();
            localPropsSize = props.size(); 
            props.insert(props.begin(),pl.begin(), pl.end());
            isPrimitive = !(baseType->isDataObjectType());
        }
        if (isPrimitive && (props.size() > 0))
        {
            string msg("Add property to data type:");
            msg += getURI();
            msg += " ";
            msg += getName();
            SDO_THROW_EXCEPTION("initCompoundProperties", 
            SDOUnsupportedOperationException, 
            msg.c_str());
        }
        isResolved  = true;
        isResolving = false;
        return;
    }


    ///////////////////////////////////////////////////////////////////////////
    // Returns the list of the properties of this type.
    ///////////////////////////////////////////////////////////////////////////
    PropertyList TypeImpl::getProperties() const
    {
        return props;
    }

    unsigned int TypeImpl::getPropertiesSize() const
    {
        return props.size();
    }

    ///////////////////////////////////////////////////////////////////////////
    // Returns the property with the specified name.
    ///////////////////////////////////////////////////////////////////////////
    const Property& TypeImpl::getProperty(const char* propertyName) const 
    {
        PropertyImpl* pi = getPropertyImpl(propertyName);
        if (pi == 0)
        {
            string msg("Property not found:");
            msg += propertyName;
            SDO_THROW_EXCEPTION("getProperty", 
            SDOPropertyNotFoundException, msg.c_str());
        }
        return (Property&)*pi;
    }


    ///////////////////////////////////////////////////////////////////////////
    // Substitute Support - get the real underlying type
    ///////////////////////////////////////////////////////////////////////////

    const Type& TypeImpl::getRealPropertyType(const char* propertyName) const
    {
        const TypeImpl* ti = getRealPropertyTypeImpl(propertyName);
        if (ti != 0)return (Type&)*ti;

        string msg("Property not found:");
        msg += propertyName;
        SDO_THROW_EXCEPTION("getProperty", 
        SDOPropertyNotFoundException, msg.c_str());

    }

    
    const TypeImpl* TypeImpl::getRealPropertyTypeImpl(const char* propertyName) const
    {

        std::list<PropertyImpl*>::const_iterator i;    
        for (i = props.begin(); i != props.end(); ++i)
        {
            if (!strcmp(propertyName,(*i)->getName()))
            {
                    return ((*i)->getTypeImpl());
            }
            for (int k=0;k < (*i)->getAliasCount(); k++)
            {
                if (!strcmp(propertyName,(*i)->getAlias(k)))
                {
                    return ((*i)->getTypeImpl());
                }
            }
            for (int j=0;j < (*i)->getSubstitutionCount(); j++)
            {
                if (!strcmp(propertyName,(*i)->getSubstitutionName(j)))
                {
                    return (TypeImpl*)(*i)->getSubstitutionType(j);
                }
            }
        }
        return 0;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Returns the property with the specified name.
    ///////////////////////////////////////////////////////////////////////////
    PropertyImpl* TypeImpl::getPropertyImpl(const char* propertyName) const
    {

        // Extension - find the property from an xpath
        // not very elegant, but it will do.
        // you should not be able to have "." and "[" before a "/" - this is assumed.

        if (propertyName == 0 || strlen(propertyName) == 0) return 0;

        char*    tokenend    = strchr(propertyName,'/');
        char *    bracket        = strchr(propertyName,'[');
        char*    dot            = strchr(propertyName,'.');
        char*   copy;
        

        int len = strlen(propertyName);
        if (tokenend != 0) 
        {
            len = tokenend  - propertyName;
            if (bracket != 0 && bracket <  tokenend) len = bracket  - propertyName;
            if (dot     != 0 && dot     <  tokenend) len = dot      - propertyName;
        }
        else 
        {
            if (bracket != 0) len = bracket - propertyName;
            if (dot     != 0) len = dot     - propertyName;
        }
        if (len != 0)
        {
            copy = new char[len+1];
            strncpy(copy, propertyName,len);
            copy[len] = 0;
        }
        else
        {
            copy = new char[strlen(propertyName)+1];
            strcpy(copy,propertyName);
        }
            
        std::list<PropertyImpl*>::const_iterator i;    
        for (i = props.begin(); i != props.end(); ++i)
        {
            if (!strcmp(copy,(*i)->getName()))
            {
                delete copy;
                if (tokenend && strlen(tokenend) > 1) 
                { 
                    const TypeImpl* ti = (*i)->getTypeImpl();
                    if (ti != 0)
                    {
                        PropertyImpl* p = ti->
                            getPropertyImpl((const char *)(tokenend+1));
                        return p;
                    }
                    else
                    {
                        return (PropertyImpl*)(*i);
                    }
                }
                else {
                    return (PropertyImpl*)(*i);
                }
            }
            for (int j=0;j < (*i)->getSubstitutionCount(); j++)
            {
                if (!strcmp(copy,(*i)->getSubstitutionName(j)))
                {
                    delete copy;
                    if (tokenend && strlen(tokenend) > 1) 
                    {     
                        const TypeImpl* ti = (*i)->getTypeImpl();
                        if (ti != 0)
                        {
                            PropertyImpl* p = ti->
                            getPropertyImpl((const char *)(tokenend+1));
                            return p;
                        }
                        else
                        {
                            return (PropertyImpl*)(*i);
                        }
                    }
                    else {
                        return (PropertyImpl*)(*i);
                    }
                }
            }
            for (int k=0;k < (*i)->getAliasCount(); k++)
            {
                if (!strcmp(copy,(*i)->getAlias(k)))
                {
                    delete copy;
                    if (tokenend && strlen(tokenend) > 1) 
                    {     
                        const TypeImpl* ti = (*i)->getTypeImpl();
                        if (ti != 0)
                        {
                            PropertyImpl* p = ti->
                            getPropertyImpl((const char *)(tokenend+1));
                            return p;
                        }
                        else
                        {
                            return (PropertyImpl*)(*i);
                        }
                    }
                    else {
                        return (PropertyImpl*)(*i);
                    }
                }
            }
        }
        return 0;
    }


    ///////////////////////////////////////////////////////////////////////////
    // Returns the property with the specified name.
    ///////////////////////////////////////////////////////////////////////////
    PropertyImpl* TypeImpl::getPropertyImplPure(const char* propertyName) const
    {

        
        std::list<PropertyImpl*>::const_iterator i;    
        for (i = props.begin(); i != props.end(); ++i)
        {
            if (!strcmp(propertyName,(*i)->getName()))
            {
                return (PropertyImpl*)(*i);
            }
            for (int k=0;k < (*i)->getAliasCount(); k++)
            {
                if (!strcmp(propertyName,(*i)->getAlias(k)))
                {
                    return (PropertyImpl*)(*i);
                }
            }
        }
        return 0;
    }


    ///////////////////////////////////////////////////////////////////////////
    // Returns the property index with the specified name.
    ///////////////////////////////////////////////////////////////////////////
    unsigned int TypeImpl::getPropertyIndex(const char* propertyName) const 
    {
        std::list<PropertyImpl*>::const_iterator i;    
        int j = 0;
        for (i = props.begin(); i != props.end(); ++i)
        {
            if (!strcmp(propertyName,(*i)->getName()))
            {
                return j;
            }
            j++;
        }
        string msg("Property not found:");
        msg += propertyName;
        SDO_THROW_EXCEPTION("getPropertyIndex", 
            SDOPropertyNotFoundException, msg.c_str());
    }
    ///////////////////////////////////////////////////////////////////////////
    // Returns the property with the specified name.
    ///////////////////////////////////////////////////////////////////////////
    const Property& TypeImpl::getProperty(unsigned int index) const
    {
        PropertyImpl* pi = getPropertyImpl(index);
        if (pi == 0)
        {
            string msg("Property not found for index:");
            msg += index;
            SDO_THROW_EXCEPTION("getProperty" ,
            SDOPropertyNotFoundException, msg.c_str());
        }
        return (Property&)*pi;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Returns the property with the specified index.
    ///////////////////////////////////////////////////////////////////////////
    PropertyImpl* TypeImpl::getPropertyImpl(unsigned int index) const
    {
        std::list<PropertyImpl*>::const_iterator i;
        int count = 0;
        for (i = props.begin() ; i != props.end() ; ++i)
        {
            if (count == index)return (PropertyImpl*)(*i);
            count++;
        }
        return 0;
    }


    ///////////////////////////////////////////////////////////////////////////
    // Returns the property with the specified name.
    ///////////////////////////////////////////////////////////////////////////
    void TypeImpl::addProperty(const char* propname,
                         const TypeImpl& t, 
                         bool many, bool rdonly, bool cont) 
    {

        props.insert(props.end(),new PropertyImpl(*this, 
                             propname, t, many,rdonly, cont));
        localPropsSize++;

    }

    ///////////////////////////////////////////////////////////////////////////
    // Indicates if this Type specifies DataObjects..
    ///////////////////////////////////////////////////////////////////////////
    bool TypeImpl::isDataObjectType() const
    {
          return !isPrimitive;
    }
  
    ///////////////////////////////////////////////////////////////////////////
    // Indicates if this Type specifies Sequenced DataObjects.
    ///////////////////////////////////////////////////////////////////////////
    bool TypeImpl::isSequencedType() const
    {
          return isSequenced;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Indicates if this Type allows any form of open content.
    ///////////////////////////////////////////////////////////////////////////
    bool TypeImpl::isOpenType() const
    {
        /* TODO - handle open types */
          return isOpen;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Indicates if this type may not be instantiated.
    ///////////////////////////////////////////////////////////////////////////
    bool TypeImpl::isAbstractType() const
    {
        return isAbstract;
    }

    void TypeImpl::setAbstract(bool set)
    {
        isAbstract = set;
    }

    ///////////////////////////////////////////////////////////////////////////
    // TODO - override ==.
    ///////////////////////////////////////////////////////////////////////////
    bool TypeImpl::equals(const Type& t)
    {
        return (   strcmp(getName(),t.getName()) == 0 
          &&       strcmp(getURI (),t.getURI ()) == 0  );
    }
  
    ///////////////////////////////////////////////////////////////////////////
    // getTypeEnum.
    ///////////////////////////////////////////////////////////////////////////
    Type::Types  TypeImpl::getTypeEnum(void) const
    {
        return typeEnum;
    }


    ///////////////////////////////////////////////////////////////////////////
    // Conversions.
    // convert is used in the process of setting a value. The input is the 
    // result of a setXXX, so in the first case setBoolean. The typeEnum is the
    // type of the value being set and governs the expected final result in
    // the value* buffer.
    ///////////////////////////////////////////////////////////////////////////

    unsigned int TypeImpl::convert(void** value,const bool b) const
    {
    switch (typeEnum)
        {
        case BooleanType: 
        case ByteType:
        case CharacterType:
        case IntegerType: 
        case ShortType:
            if (*value != 0)delete (char*)(*value);
            *value = new char[sizeof(long)];
              *(long*)*value = (long)b;
            return sizeof(long);

        case LongType:
            if (*value != 0)delete (char*)(*value);
            *value = new char[sizeof(int64_t)];
              *(int64_t*)*value = (int64_t)b;
            return sizeof(int64_t);

        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            if (*value != 0)delete (char*)(*value);
            // spec change - types
            *value = new wchar_t[6];
            if (b) 
            {
                ((wchar_t*)(*value))[0] = (wchar_t)'t';
                ((wchar_t*)(*value))[1] = (wchar_t)'r';
                ((wchar_t*)(*value))[2] = (wchar_t)'u';
                ((wchar_t*)(*value))[3] = (wchar_t)'e';
                ((wchar_t*)(*value))[4] = 0;
                return  4;
            }
            else 
            {
                ((wchar_t*)(*value))[0] = (wchar_t)'f';
                ((wchar_t*)(*value))[1] = (wchar_t)'a';
                ((wchar_t*)(*value))[2] = (wchar_t)'l';
                ((wchar_t*)(*value))[3] = (wchar_t)'s';
                ((wchar_t*)(*value))[4] = (wchar_t)'e';
                ((wchar_t*)(*value))[5] = 0;
                return  5;
            }
            break;

            
        case BytesType:
            {
            if (*value != 0)delete (char*)(*value);
            // spec change - types
            *value = new char[6];
            if (b) 
            {
                strcpy((char*)*value, "true");
                return 4;
            }
            else 
            {
                strcpy((char*)*value, "false");
                return 5;
            }
            break;

            }

        case DoubleType:    
        case FloatType:    
        case DateType:
        case OtherTypes:
        case DataObjectType: 
        case ChangeSummaryType:
        default:
            {
            string msg("Cannot set Boolean on object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("setBoolean" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    return 0;
    }

    unsigned int TypeImpl::convert(void** value,const char c) const
    {
        return convert(value,(long)c);
    }

    unsigned int TypeImpl::convert(void** value,const wchar_t c) const
    {
        return convert(value,(long)c);
    }

    // This is set CString...
    unsigned int TypeImpl::convert(void** value,const char* c) const
    {

    switch (typeEnum)
        {
        case BooleanType:
            if (*value != 0)delete ((char*)*value);
            *value = new char[sizeof(long)];
            if ((c == 0) || strcmp(c,"true"))
            {
                *(long*)*value = 0;
            }
            else 
            {
                *(long*)*value = 1;
            }
            return sizeof(long);

        case ByteType:
        case CharacterType: 
        case IntegerType: 
        case ShortType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(long)];
              *(long*)*value = atoi(c);
            return sizeof(long);

        case DoubleType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(long double)];
            // TODO - atof not suitable here
            *(long double*)*value = (long double)atof(c);
            return sizeof(long double);

        case FloatType:    
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(float)];
            *(float*)*value = (float)atof(c);
            return sizeof(float);

        case LongType:   
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(int64_t)];
#if defined(WIN32)  || defined (_WINDOWS)
            *(int64_t*)*value = (int64_t)_atoi64(c);
#else 
            *(int64_t*)*value = (int64_t)strtoll(c, NULL, 0);
#endif
            return sizeof(int64_t);

        case DateType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(time_t)];
            *(time_t*)*value = (time_t)atoi(c);
            return sizeof(time_t);
            
        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            {
                if (*value != 0) delete ((wchar_t*)*value);
                wchar_t* vw = new wchar_t[strlen(c)+1];
                for (int i=0;i< strlen(c);i++)
                {
                    vw[i] = (wchar_t)c[i];
                }
                vw[strlen(c)] = 0;
                *value = (void*)vw;
                return strlen(c);
            }
            break;
        case BytesType:
            {
                if (*value != 0) delete ((char*)*value);
                char* vc = new char[strlen(c)+1];
                for (int i=0;i< strlen(c);i++)
                {
                    vc[i] =  (char)c[i];
                }
                vc[strlen(c)] = 0;
                *value = (void*)vc;
                return strlen(c);
            }
            break;
             
        case OtherTypes:
        case DataObjectType: 
        case ChangeSummaryType:
        default:
            {
            string msg("Cannot set CString on object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("setString" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    return 0;
    }

    // setString
    unsigned int TypeImpl::convert(void** value,const wchar_t* b, unsigned int len) const
    {
    int i;
    switch (typeEnum)
        {
        case BigDecimalType:
        case BigIntegerType:
        case UriType:
        case StringType:
            {
                if (*value != 0) delete ((wchar_t*)*value);
                wchar_t* vw = new wchar_t[len+1];
                for (i=0;i<len;i++)
                {
                    vw[i] = b[i];
                }
                vw[len] = 0;
                *value = (void*)vw;
                return len;
            }
            break;

        case BytesType:
            {
                if (*value != 0) delete ((char*)*value);
                char* vc = new char[len+1];
                for (i=0;i<len;i++)
                {
                    vc[i] =  (char)b[i];
                }
                vc[len] = 0;
                *value = (void*)vc;
                return len;
            }
            break;

        case BooleanType:    
            if (*value != 0)delete ((char*)*value);
            *value = new char[sizeof(long)];
            if (len > 4) 
            {
                *(long*)*value = 0;
            }
            else 
            {
                if (b[0] == (wchar_t)'t' &&
                    b[1] == (wchar_t)'r' &&
                    b[2] == (wchar_t)'u' &&
                    b[3] == (wchar_t)'e' )
                     *(long*)*value = 1;
                else *(long*)*value = 0;
            }
            return sizeof(long);

        case CharacterType: 
        case ByteType:
            if (*value != 0)delete ((char*)*value);
            *value = new char[sizeof(long)];
            if (len > 0)
            {
                *(long*)*value = (long)b[0];
            }
            else
            {
                *(long*)*value = (long)0;
            }
            return sizeof(long);

        case ShortType:
        case IntegerType:
            {
                if (*value != 0)delete ((char*)*value);
                *value = new char[sizeof(long)];
                int val = 0;
                for (int j=0;j<len;j++)
                {
                    val += (1+ (10*j)) * ((char)b[len-1-j] - (char)'0');
                }
                *(long*)*value = (long)val;
                return sizeof(long);
            }

        case LongType:
            {
                if (*value != 0)delete ((char*)*value);
                *value = new char[sizeof(int64_t)];
                int64_t val = 0;
                for (int j=0;j<len;j++)
                {
                    val += (int64_t)(1+ (10*j)) * ((char)b[len-1-j] - (char)'0');
                }
                *(int64_t*)*value = (int64_t)val;
                return sizeof(long);
            }
        case DoubleType:    
        case FloatType:    
        case DateType:
        case OtherTypes:
        case DataObjectType: 
        case ChangeSummaryType:
        default:
            {
            string msg("Cannot set String on object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("setString" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    return  0;
    }

    // setBytes
    unsigned int TypeImpl::convert(void** value,const char* b, unsigned int len) const
    {
    int i;
    switch (typeEnum)
        {
        case BytesType:
            {
                if (*value != 0) delete ((char*)*value);
                char* vc = new char[len+1];
                for (i=0;i<len;i++)
                {
                    vc[i] = (char)b[i];
                }
                vc[len] = 0;
                *value = (void*)vc;
                return len;
            }

        case BigDecimalType: 
        case BigIntegerType: 
        case UriType:
        case StringType:
            {
                if (*value != 0) delete ((wchar_t*)*value);
                wchar_t* vw = new wchar_t[len+1];
                for (i=0;i<len;i++)
                {
                    vw[i] = b[i];
                }
                vw[len] = 0;
                *value = (void*)vw;
                return len;
            }

        case BooleanType:    
            if (*value != 0)delete ((char*)*value);
            *value = new char[sizeof(long)];
            if (len > 4) 
            {
                *(long*)*value = 0;
            }
            else 
            {
                if (b[0] == (char)'t' &&
                    b[1] == (char)'r' &&
                    b[2] == (char)'u' &&
                    b[3] == (char)'e' )
                     *(long*)*value = 1;
                else *(long*)*value = 0;
            }
            return sizeof(long);

        case ByteType:
        case CharacterType: 
            if (*value != 0)delete ((char*)*value);
            *value = new char[sizeof(long)];
            if (len > 0)
            {
                *(long*)*value = (long)b[0];
            }
            else
            {
                *(long*)*value = (long)0;
            }
            return sizeof(long);

        case IntegerType: 
        case ShortType:
            {
                if (*value != 0)delete ((char*)*value);
                *value = new char[sizeof(long)];
                int val = 0;
                for (int j=0;j<len;j++)
                {
                    val += (1+ (10*j)) * ((char)b[len-1-j] - (char)'0');
                }
                *(long*)*value = (long)val;
                return sizeof(long);
            }

        case LongType:
            {
                if (*value != 0)delete ((char*)*value);
                *value = new char[sizeof(int64_t)];
                int64_t val = 0;
                for (int j=0;j<len;j++)
                {
                    val += (int64_t)(1+ (10*j)) * ((char)b[len-1-j] - (char)'0');
                }    
                *(int64_t*)*value = (int64_t)val;
                return sizeof(long);
            }

        case DoubleType:    
        case FloatType:    
        case DateType:
        case OtherTypes:
        case DataObjectType: 
        case ChangeSummaryType:
        default:
            {
            string msg("Cannot set Bytes on object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("setBytes" ,
              SDOInvalidConversionException, msg.c_str());
            return 0;
            }
        }
    return 0;
    }
    
    
    unsigned int TypeImpl::convert(void** value,const short s) const
    {
        return convert(value,(long)s);
    }

/*    unsigned int TypeImpl::convert(void** value,const int i) const
    {
        return convert(value,(long)i);
     }
*/
    unsigned int TypeImpl::convertDate(void** value, const SDODate i) const
    {
        switch (typeEnum)
        {
        case BooleanType:
            {
            string msg("Cannot set Date on object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("setDate" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        default:
            return convert(value, (long)(i.getTime()));
        }
    }

    // setInteger
    unsigned int TypeImpl::convert(void** value,const long i) const
    {
#if ! defined(WIN32)  &&  ! defined (_WINDOWS)
            char *tmp = new char[MAX_LONG_SIZE];
            int j = 0;
#endif
    switch (typeEnum)
        {
        case BooleanType:    
        case ByteType:
        case CharacterType: 
        case IntegerType: 
        case ShortType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(long)];
              *(long*)*value = (long)i;
            return sizeof(long);

        case DoubleType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(long double)];
            *(long double*)*value = (long double)i;
            return sizeof(long double);

        case FloatType:    
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(float)];
            *(float*)*value = (float)i;
            return sizeof(float);

        case LongType:      
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(int64_t)];
            *(int64_t*)*value = (int64_t)i;
            return sizeof(int64_t);

        case DateType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(time_t)];
            *(time_t*)*value = (time_t)i;
            return sizeof(time_t);
            
        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            {
                if (*value != 0) delete ((wchar_t*)*value);
                *value = new wchar_t[MAX_LONG_SIZE];
                // TODO - whats the equivalent of _ltow on linux?
#if defined(WIN32)  || defined (_WINDOWS)
                _ltow(i,(wchar_t*)*value,10);
                return wcslen((wchar_t*)(*value));
#else
                sprintf(tmp,"%d", i);
                for (j=0; j< strlen(tmp); j++) 
                {
                    ((wchar_t*)(*value))[j] = (wchar_t)tmp[j];
                }
                                ((wchar_t*)(*value))[strlen(tmp)] = 0;
                 delete tmp;
                return j;
#endif
                        
            }

        case BytesType:
            {
                if (*value != 0) delete ((wchar_t*)*value);
                *value = new char[MAX_LONG_SIZE];
                // TODO - whats the equivalent of _ltow on linux?
                sprintf((char*)*value,"%d", i);
                return strlen((char*)(*value));
                        
            }

        case OtherTypes:
        case DataObjectType: 
        case ChangeSummaryType:
        default:
            {
            string msg("Cannot set Integer on object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("setInteger" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }

    // setLongLong

    unsigned int TypeImpl::convert(void** value,const int64_t l) const
    {    
#if ! defined(WIN32)  &&  ! defined (_WINDOWS)
        char *tmp = new char[MAX_LONG_SIZE];
        int j = 0;
#endif
    switch (typeEnum)
        {
        case BooleanType:    
        case ByteType:
        case CharacterType: 
        case IntegerType: 
        case ShortType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(long)];
              *(long*)*value = (long)l;
            return sizeof(long);

        case DoubleType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(long double)];
            *(long double*)*value = (long double)l;
            return sizeof(long double);

        case FloatType:    
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(float)];
            *(float*)*value = (float)l;
            return sizeof(float);

        case LongType:      
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(int64_t)];
            *(int64_t*)*value = (int64_t)l;
            return sizeof(int64_t);

        case DateType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(time_t)];
            *(time_t*)*value = (time_t)l;
            return sizeof(time_t);
            
        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            {
                if (*value != 0) delete ((wchar_t*)*value);
                *value = new wchar_t[MAX_LONG_SIZE];
                // TODO - whats the equivalent of _ltow on linux?
#if defined(WIN32)  || defined (_WINDOWS)
                _i64tow(l,(wchar_t*)*value,10);
                return wcslen((wchar_t*)(*value));
#else
                sprintf(tmp, "%lld", l);
                for (j=0; j< strlen(tmp); j++) 
                {
                    ((wchar_t*)(*value))[j] = (wchar_t)tmp[j];
                }
                ((wchar_t*)(*value))[strlen(tmp)] = 0;
                delete tmp;
                return j;
#endif
                        
            }
             
        case BytesType:
            {
                if (*value != 0) delete ((char*)*value);
                *value = new char[MAX_LONG_SIZE];
#if defined(WIN32)  || defined (_WINDOWS)
                _i64toa(l,(char*)*value,10);
#else
                sprintf((char*)*value, "%lld", l);
#endif
                return strlen((char*)(*value));
                        
            }

        case OtherTypes:
        case DataObjectType: 
        case ChangeSummaryType:
        default:
            {
            string msg("Cannot set LongLong on object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("setLongLong" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }

    // setFloat
    unsigned int TypeImpl::convert(void** value,const float f) const
    {
    switch (typeEnum)
        {
        case BooleanType:    
        case ByteType:
        case CharacterType: 
        case IntegerType: 
        case ShortType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(long)];
              *(long*)*value = (long)f;
            return sizeof(long);

        case DoubleType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(long double)];
            *(long double*)*value = (long double)f;
            return sizeof(long double);

        case FloatType:    
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(float)];
            *(float*)*value = (float)f;
            return sizeof(float);

        case LongType:      
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(int64_t)];
            *(int64_t*)*value = (int64_t)f;
            return sizeof(int64_t);

        case DateType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(time_t)];
            *(time_t*)*value = (time_t)f;
            return sizeof(time_t);
            
        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
        case BytesType:
        case OtherTypes:
        case DataObjectType: 
        case ChangeSummaryType:
        default:
            {
            string msg("Cannot set Float on object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("setFloat" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
            break;
        }
    }

    // TODO - storing the double as a long double - perhaps I need
    // a new type?
/*    unsigned int TypeImpl::convert(void** value,const double d) const 
    {
    switch (typeEnum) 
        {
        case BooleanType:    
        case ByteType:
        case CharacterType: 
        case IntegerType: 
        case ShortType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(long)];
              *(long*)*value = (long)d;
            return sizeof(long);

        case DoubleType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(long double)];
            *(long double*)*value = (long double)d;
            return sizeof(long double);

        case FloatType:    
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(float)];
            *(float*)*value = (float)d;
            return sizeof(float);

        case LongType:      
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(int64_t)];
            *(int64_t*)*value = (int64_t)d;
            return sizeof(int64_t);

        case DateType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(time_t)];
            *(time_t*)*value = (time_t)d;
            return sizeof(time_t);
            
        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
        case BytesType:
        case OtherTypes:
        case DataObjectType: 
        case ChangeSummaryType:
        default:
            {
            string msg("Cannot set Double on object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("setDouble" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }
*/
    unsigned int TypeImpl::convert(void** value,const long double d) const 
    {
    switch (typeEnum) 
        {
        case BooleanType:    
        case ByteType:
        case CharacterType: 
        case IntegerType: 
        case ShortType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(long)];
              *(long*)*value = (long)d;
            return sizeof(long);

        case DoubleType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(long double)];
            *(long double*)*value = (long double)d;
            return sizeof(long double);

        case FloatType:    
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(float)];
            *(float*)*value = (float)d;
            return sizeof(float);

        case LongType:      
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(int64_t)];
            *(int64_t*)*value = (int64_t)d;
            return sizeof(int64_t);

        case DateType:
            if (*value != 0) delete ((char*)*value);
            *value = new char[sizeof(time_t)];
            *(time_t*)*value = (time_t)d;
            return sizeof(time_t);
            
        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
        case BytesType:
        case OtherTypes:
        case DataObjectType: 
        case ChangeSummaryType:
        default:
            {
            string msg("Cannot set Long Double on object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("setLongDouble" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }

    unsigned int TypeImpl::convert(void** value,DataObject* dob) const
    {
    switch (typeEnum) 
        {

        case OtherTypes:
        case DataObjectType:
            *(DataObject**)value = dob;
            return sizeof (void*);

        case BooleanType:
        case ByteType:
        case CharacterType:
        case IntegerType: 
        case ShortType:
        case DoubleType:
        case FloatType:
        case LongType:
        case DateType:
        case BigDecimalType: 
        case BigIntegerType: 
        case StringType: 
        case BytesType:
        case UriType:
        default:
            {
            string msg("Cannot set Data Object on object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("setDataObject" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
        return 0;
    }
 
    unsigned int TypeImpl::convertToString(void* value, wchar_t* outval, unsigned int len,
        unsigned int max) const
    {
#if ! defined(WIN32)  && ! defined (_WINDOWS)
            char* tmpstr = new char[MAX_LONG_SIZE];
            int j = 0;
#endif
    int i;
    switch (typeEnum) 
        {

        case BigDecimalType: 
        case BigIntegerType: 
        case UriType:
        case StringType:
            {
                if (value == 0) return 0;
                wchar_t* tmp = (wchar_t*)value;
                for (i=0;(i < len) && (i < max);i++)
                {
                    outval[i] = tmp[i];
                }
                return len>max?max:len;
            }
        case BytesType:
            {
                if (value == 0) return 0;
                char* tmp = (char*)value;
                for (i=0;(i < len) && (i < max);i++)
                {
                    outval[i] = (wchar_t)(tmp[i]);
                }
                return len>max?max:len;
            }

        case BooleanType:
            {
                if (max < 4) return 0;
                if (value == 0  || *(long*)value == 0) {
                    if (max < 5) return 0;
                    outval[0] = (wchar_t)'f';
                    outval[1] = (wchar_t)'a';
                    outval[2] = (wchar_t)'l';
                    outval[3] = (wchar_t)'s';
                    outval[4] = (wchar_t)'e';
                    return 5;
                }
                else
                {
                    outval[0] = (wchar_t)'t';
                    outval[1] = (wchar_t)'r';
                    outval[2] = (wchar_t)'u';
                    outval[3] = (wchar_t)'e';
                    return 4;
                }
            }
        
        case ByteType:
            {
                if (value == 0) return 0;
                long tmp = *(long*)value;
                outval[0] = (wchar_t)(tmp&0xFF);
                return 1;
            }

        case CharacterType:
            {
                if (value == 0) return 0;
                long tmp = *(long*)value;
                outval[0] = (wchar_t)(tmp & 0xFFFF);
                return 1;
            }

        case ShortType:
        case IntegerType: 
            {
                if (value == 0) return 0;
                long tmp = *(long*)value;
#if defined(WIN32)  || defined (_WINDOWS)                
                wchar_t* tmpstr = new wchar_t[MAX_LONG_SIZE];
                _ltow(tmp,tmpstr,10);
                if (wcslen(tmpstr) > max) return 0;
                _ltow(tmp,outval,10);
                return wcslen(outval);
#else
                sprintf(tmpstr, "%ld", tmp);
                if (strlen(tmpstr) > max) return 0;
                for (j=0; j< strlen(tmpstr); j++) 
                {
                    outval[j] = (wchar_t)tmpstr[j];
                }
                delete tmpstr;
                return j;
#endif
            }


        case LongType:
            {
                if (value == 0) return 0;
                int64_t tmp = *(int64_t*)value;
#if defined(WIN32)  || defined (_WINDOWS)                
                wchar_t* tmpstr = new wchar_t[MAX_LONG_SIZE];
                _i64tow(tmp,tmpstr,10);
                if (wcslen(tmpstr) > max) 
                {
                    delete tmpstr;
                    return 0;
                }
                _i64tow(tmp,outval,10);
                return wcslen(outval);
#else    
                sprintf(tmpstr, "%lld", tmp);
                if (strlen(tmpstr) > max) return 0;
                for (j=0; j< strlen(tmpstr); j++) 
                {
                    outval[j] = (wchar_t)tmpstr[j];
                }
                delete tmpstr;
                return j;
#endif
            }

        case DateType:
            {
    
                if (value == 0) return 0;
                string msg("Conversion to string not implemented from type:");
                msg += getName();
                SDO_THROW_EXCEPTION("getString" ,
                  SDOInvalidConversionException, msg.c_str());
                break;
            }

        case DoubleType:
            {
                if (value == 0) return 0;
                if (max < MAX_DOUBLE_SIZE) return 0;
                wchar_t* fmt = new wchar_t[5];
                fmt[0] = (wchar_t)'%';
                fmt[1] = (wchar_t)'.';
                fmt[2] = (wchar_t)'3';
                fmt[3] = (wchar_t)'e';
                fmt[4] = (wchar_t)0;
#if defined(WIN32)  || defined (_WINDOWS)    
                swprintf((wchar_t*)outval,fmt,*(long double*)value);
#else
                swprintf((wchar_t*)outval, wcslen((wchar_t*)outval), fmt, *(long double*)value);
#endif
                delete fmt;

                return wcslen(outval);
            }
        case FloatType:
            {
                if (value == 0) return 0;
                if (max < MAX_FLOAT_SIZE) return 0;
                wchar_t* fmt = new wchar_t[5];
                fmt[0] = (wchar_t)'%';
                fmt[1] = (wchar_t)'.';
                fmt[2] = (wchar_t)'3';
                fmt[3] = (wchar_t)'e';
                fmt[4] = (wchar_t)0;
#if defined(WIN32)  || defined (_WINDOWS)
                swprintf(outval,fmt,*(float*)value);
#else
                swprintf(outval, wcslen(outval), fmt, *(float*)value);
#endif
                delete fmt;

                return wcslen(outval);
            }

        case OtherTypes:
        case DataObjectType:
        default:
            {
            string msg("Cannot get String from object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("getString" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    return 0;
    }

    unsigned int TypeImpl::convertToBytes(void* value, char* outval, unsigned int len,
        unsigned int max) const
    {
    int i;
    switch (typeEnum) 
        {
        case BytesType:
            {
                if (value == 0) return 0;
                char* tmp = (char*)value;
                for (i=0;(i < max) && (i < len);i++)
                {
                    outval[i] = tmp[i];
                }
                return len>max?max:len;
            }

        case BigDecimalType: 
        case BigIntegerType: 
        case UriType:
        case StringType:
            {
                if (value == 0) return 0;
                wchar_t* tmp = (wchar_t*)value;
                for (i=0;(i < max) && (i < len);i++)
                {
                    outval[i] = (char)(tmp[i]);
                }
                return len>max?max:len;
            }

        case BooleanType:
            {
                if (value == 0 || *(long*)value == 0) {
                    if (max < 5) return 0;
                    outval[0] = 'f';
                    outval[1] = 'a';
                    outval[2] = 'l';
                    outval[3] = 's';
                    outval[4] = 'e';
                    return 5;
                }
                else {
                    if (max < 4) return 0;
                    outval[0] = 't';
                    outval[1] = 'r';
                    outval[2] = 'u';
                    outval[3] = 'e';
                    return 4;
                }
                return 0;
            }

        case CharacterType:
        case ByteType:
            {
                if (value == 0) return 0;
                long tmp = *(long*)value;
                outval[0] = (char)(tmp&0xFF);
                return 1;
            }

        case ShortType:
        case IntegerType:
            {
                if (value == 0) return 0;
                long tmp = *(long*)value;
                char* tmpstr = new char[MAX_LONG_SIZE];
                sprintf(tmpstr, "%ld", tmp);
                if (strlen(tmpstr) > max) 
                {
                    delete tmpstr;
                    return 0;
                }
                delete tmpstr;
                sprintf(outval, "%ld", tmp);
                return strlen(outval);
            }

            
        case LongType:
            {
                if (value == 0) return 0;
                int64_t tmp = *(int64_t*)value;
                char * tmpstr = new char[MAX_LONG_SIZE];
                sprintf(tmpstr, "%lld", tmp);
                if (strlen(tmpstr) > max) 
                {
                    delete tmpstr;
                    return 0;
                }
                delete tmpstr;
                sprintf(outval, "%lld", tmp);
                return strlen(outval);
            }
        case DateType:
            {
                if (value == 0) return 0;
                string msg("Conversion to string not implemented from type:");
                msg += getName();
                SDO_THROW_EXCEPTION("getBytes" ,
                  SDOInvalidConversionException, msg.c_str());
                break;
            }

        case DoubleType:
                if (value == 0) return 0;
                if (max < MAX_DOUBLE_SIZE) return 0;
                sprintf(outval,"%.3e",*(long double*)value);
                return strlen(outval);

        case FloatType:
                if (value == 0) return 0;
                if (max < MAX_FLOAT_SIZE) return 0;
                sprintf(outval,"%.3e",*(float*)value);
                return strlen(outval);

        case OtherTypes:
        case DataObjectType:
        default:
            {
            if (max < 9) return 0;
            sprintf(outval,"%08x",value);
            //string msg("Cannot get Bytes from object of type:");
            //msg += getName();
            //SDO_THROW_EXCEPTION("getBytes" ,
            //  SDOInvalidConversionException, msg.c_str());
            return strlen(outval);
            }
        }
    return 0;
    }


    const char* TypeImpl::convertToCString(void* value, char** asstringbuf, unsigned int len) const
    {
    int i;

    switch (typeEnum) 
        {
        case BooleanType:
            if (value == 0 || *(long*)value == 0) {
                *asstringbuf = new char[6];
                strcpy(*asstringbuf,"false");
            }
            else {
                *asstringbuf = new char[5];
                strcpy(*asstringbuf,"true");
            }
            return *asstringbuf;

        case ByteType:
            *asstringbuf = new char[MAX_LONG_SIZE];
            if (value == 0){
                sprintf(*asstringbuf,"%ld",0);
            }
            else {
                sprintf(*asstringbuf,"%ld",*(long*)value);
            }
            return *asstringbuf;

        case CharacterType:
            *asstringbuf = new char[MAX_LONG_SIZE];
            if (value == 0){
                sprintf(*asstringbuf,"%ld",0);
            }
            else {
                sprintf(*asstringbuf,"%ld", *(long*)value);
            }
            return *asstringbuf;
        
        case IntegerType: 
            *asstringbuf = new char[MAX_LONG_SIZE];
            if (value == 0){
                sprintf(*asstringbuf,"%ld",0);
            }
            else {
                sprintf(*asstringbuf,"%ld", *(long*)value);
            }
            return *asstringbuf;

        case ShortType:
            *asstringbuf = new char[MAX_LONG_SIZE];
            if (value == 0){
                sprintf(*asstringbuf,"%ld",0);
            }
            else {
                sprintf(*asstringbuf,"%ld", *(long*)value);
            }
            return *asstringbuf;

        case DoubleType:
            *asstringbuf = new char[MAX_DOUBLE_SIZE];
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                (*asstringbuf)[0] = 0;
                return *asstringbuf;
            }
            sprintf(*asstringbuf,"%.3e",*(long double*)value);
            return *asstringbuf;

        case FloatType:
            *asstringbuf = new char[MAX_FLOAT_SIZE];
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                (*asstringbuf)[0] = 0;
                return *asstringbuf;
            }
            sprintf(*asstringbuf,"%.3e", *(float*)value);
            return *asstringbuf;

        case LongType:
            {
            *asstringbuf = new char[MAX_LONG_SIZE];
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                (*asstringbuf)[0] = 0;
                return *asstringbuf;
            }
            int64_t temp = *(int64_t*)value;
#if defined(WIN32)  || defined (_WINDOWS)
            sprintf(*asstringbuf,"%I64d", temp);
#else
            sprintf(*asstringbuf,"%lld", temp);
#endif
            return *asstringbuf;
            }
        case DateType:
            *asstringbuf = new char[MAX_LONG_SIZE];
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                (*asstringbuf)[0] = 0;
                return *asstringbuf;
            }
            sprintf(*asstringbuf,"%ld", *(time_t*)value);
            return *asstringbuf;

        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            {
            if (value == 0 || len == 0)
            {
                *asstringbuf = new char[1];
                (*asstringbuf)[0] = 0;
                return *asstringbuf;
            }
            *asstringbuf = new char[len + 1];
            wchar_t* tmp = (wchar_t*)value;
            for (i=0;i< len;i++)
            {
                (*asstringbuf)[i] = tmp[i] & 0xFF;
            }
            (*asstringbuf)[i] = 0;
            return *asstringbuf;
            }

        case BytesType:
            {
            if (value == 0 || len == 0)
            {
                *asstringbuf = new char[1];
                (*asstringbuf)[0] = 0;
                return *asstringbuf;
            }
            *asstringbuf = new char[len + 1];
            char* tmp = (char*)value;
            for (i=0;i< len ;i++)
            {
                (*asstringbuf)[i] = (char)(tmp[i]);
            }
            (*asstringbuf)[i] = 0;
            return *asstringbuf;
            }

     
        case OtherTypes:
        case DataObjectType:
        default:
            {
            *asstringbuf = new char[9];
            sprintf(*asstringbuf,"%08x",value);
            //string msg("Cannot get CString from object of type:");
            //msg += getName();
            //SDO_THROW_EXCEPTION("getCString" ,
            //  SDOInvalidConversionException, msg.c_str());
            return *asstringbuf;
            }
        }
        return 0;
    }

    const bool TypeImpl::convertToBoolean(void* value, unsigned int len) const
    {
    switch (typeEnum) 
        {
        case BooleanType:
        case ByteType:
        case CharacterType:
        case IntegerType: 
        case ShortType:
            if (value == 0) return false;
            if (*(long*)value != 0) return true;
            return false;

        case DoubleType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return false;
            }
            if (*(long double*)value != 0.0) return true;
            return false;

        case FloatType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return false;
            }
            if (*(float*)value != 0.0) return true;
            return false;

        case LongType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return false;
            }
            if (*(int64_t*)value != 0)return true;
            return false;

        case DateType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return false;
            }
            if (*(time_t*)value != 0)return true;
            return false;

        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            if (value == 0 || len < 4) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return false;
            }
            if ( ((wchar_t*)value)[0] == (wchar_t)'t' &&
                 ((wchar_t*)value)[1] == (wchar_t)'r' &&
                 ((wchar_t*)value)[2] == (wchar_t)'u' &&
                 ((wchar_t*)value)[3] == (wchar_t)'e' )
                 return true;

            return false;

        case BytesType:
            if (value == 0 || len < 4) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return false;
            }
            if (!strncmp((char*)value,"true", 4)) return true;
            return false;
     
        case OtherTypes:
        case DataObjectType:
        default:
            {
            string msg("Cannot get Boolean from object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("getBoolean" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }

    }

    const char TypeImpl::convertToByte(void* value , unsigned int len) const
    {
    switch (typeEnum) 
        {
        case BooleanType:
        case ByteType:
        case CharacterType:
        case IntegerType: 
        case ShortType:
            if (value == 0) return 0;
            return (char)(*(long*)value);

        case DoubleType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (char)(*(long double*)value);

        case FloatType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (char)(*(float*)value);

        case LongType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (char)(*(int64_t*)value);

        case DateType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
    
            return (char)(*(time_t*)value);

        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            {
             if (value == 0) return 0;
             return (char)*(wchar_t*)value;
            }

        case BytesType:
            {
             if (value == 0) return 0;
             return (char)*(char*)value;
            }

     
        case OtherTypes:
        case DataObjectType:
        default:
            {
            string msg("Cannot get Byte from object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("getByte" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }

    const wchar_t TypeImpl::convertToCharacter(void* value, unsigned int len) const
    {
    switch (typeEnum) 
        {
        case BooleanType:
        case ByteType:
        case CharacterType:
        case IntegerType: 
        case ShortType:
            if (value == 0) return 0;
            return (wchar_t)(*(long*)value);

        case DoubleType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (wchar_t)(*(long double*)value);

        case FloatType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (wchar_t)(*(float*)value);

        case LongType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (wchar_t)(*(int64_t*)value);

        case DateType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (wchar_t)(*(time_t*)value);

        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            {
                if (value == 0) return 0;
                return *(wchar_t*)value;
            }

        case BytesType:
            {
                if (value == 0) return 0;
                return (wchar_t)*(char*)value;
            }

        case OtherTypes:
        case DataObjectType:
        default:
            {
            string msg("Cannot get Character from object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("getCharacter" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }
    const short TypeImpl::convertToShort(void* value, unsigned int len) const
    {
#if ! defined(WIN32)  && !defined (_WINDOWS)
            char* tmpstr = new char[len+1];
            short s = 0;
#endif
    switch (typeEnum) 
        {
        case BooleanType:
        case ByteType:
        case CharacterType:
        case IntegerType: 
        case ShortType:
            if (value == 0) return 0;
            return (short)(*(long*)value);

        case DoubleType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (short)(*(long double*)value);

        case FloatType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (short)(*(float*)value);

        case LongType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (short)(*(int64_t*)value);

        case DateType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (short)(*(time_t*)value);

        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            if (value == 0 || len == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
#if defined(WIN32)  || defined (_WINDOWS)
            return (short)_wtoi((wchar_t*)value);
#else
            for (int j=0;j< len;j++)
            {
                tmpstr[j] = (char)((char *)value)[j];
            }
            tmpstr[len] = 0;
            s = (short) atoi(tmpstr);
            delete tmpstr;
            return s;
#endif


        case BytesType:
        case OtherTypes:
        case DataObjectType:
        default:
            {
            string msg("Cannot get Short from object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("getShort" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }

    
/*    const int TypeImpl::convertToInt(void* value, unsigned int len) const
    {
    switch (typeEnum) 
        {
        case BooleanType:
        case ByteType:
        case CharacterType:
        case IntegerType: 
        case ShortType:
            if (value == 0) return 0;
            return (int)(*(long*)value);

        case DoubleType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (int)(*(long double*)value);

        case FloatType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (int)(*(float*)value);
        
        case LongType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (int)(*(int64_t*)value);

        case DateType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (int)(*(time_t*)value);

        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
#if defined(WIN32)  || defined (_WINDOWS)
            return (int)_wtoi((wchar_t*)value);
#else
            char* tmp = new char[len+1];
            int j = 0;;
            for (j=0;j<len;j++)
            {
                tmp[j] = (char)(wchar_t*)value[j];
            }
            tmp[j] = 0;
            j = atoi(tmp);
            delete tmp;
            return j;
#endif
     
        case BytesType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (int)atoi((char*)value);

        case OtherTypes:
        case DataObjectType:
        default:
            {
            string msg("Cannot get Int from object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("getInt" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }

  */
    const long TypeImpl::convertToInteger(void* value, unsigned int len) const
    {
#if ! defined(WIN32)  && ! defined (_WINDOWS)
            char* tmp = new char[len+1];
            int j = 0;
            long l = 0;
#endif
    switch (typeEnum) 
        {
        case BooleanType:
        case ByteType:
        case CharacterType:
        case IntegerType: 
        case ShortType:
            if (value == 0) return 0;
            return (long)(*(long*)value);

        case DoubleType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (long)(*(long double*)value);

        case FloatType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (long)(*(float*)value);

        case LongType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (long)(*(int64_t*)value);

        case DateType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (long)(*(time_t*)value);

        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
#if defined(WIN32)  || defined (_WINDOWS)
            return (long)_wtol((wchar_t*)value);
#else
            for (j=0;j<len;j++)
            {
                tmp[j] = (char)((wchar_t*)value)[j];
            }
            tmp[j] = 0;
            l = atol(tmp);
            delete tmp;
            return l;
#endif
     
        case BytesType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (int)atol((char*)value);

        case OtherTypes:
        case DataObjectType:
        default:
            {
            string msg("Cannot get Long from object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("getLong" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }
    const int64_t TypeImpl::convertToLong(void* value, unsigned int len) const 
    {
#if ! defined(WIN32)  && ! defined (_WINDOWS)
            char* tmp = new char[len+1];
            int j = 0;
            int64_t l = 0;
#endif
    switch (typeEnum) 
        {
        case BooleanType:
        case CharacterType:
        case ByteType:
        case IntegerType: 
        case ShortType:
            if (value == 0) return 0;
            return (int64_t)(*(long*)value);

        case DoubleType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (int64_t)(*(long double*)value);

        case FloatType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (int64_t)(*(float*)value);

        case LongType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (int64_t)(*(int64_t*)value);

        case DateType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (int64_t)(*(time_t*)value);

        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
#if defined(WIN32)  || defined (_WINDOWS)
            return (long)_wtol((wchar_t*)value);
#else
            for (j=0;j<len;j++)
            {
                tmp[j] = (char)((wchar_t*)value)[j];
            }
            tmp[j] = 0;
            l = strtoll(tmp, NULL, 0);
            delete tmp;
            return l;
#endif


        case BytesType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
#if defined(WIN32)  || defined (_WINDOWS)
            return _atoi64((char*)value);
#else
            return  strtoll((char*)value, NULL, 0);
#endif

        case OtherTypes:
        case DataObjectType:
        default:
            {
            string msg("Cannot get Long Long from object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("getLongLong" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }
    const float TypeImpl::convertToFloat(void* value, unsigned int len) const
    {
    switch (typeEnum) 
        {
        case BooleanType:
        case ByteType:
        case CharacterType:
        case IntegerType: 
        case ShortType:
            if (value == 0) return 0;
            return (float)(*(long*)value);

        case DoubleType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (float)(*(long double*)value);

        case FloatType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (float)(*(float*)value);

        case LongType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (float)(*(int64_t*)value);

        case DateType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (float)(*(time_t*)value);

        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            {
                if (value == 0) 
                {
                    // Conversions might set from a zero value, as they are used
                    // by the default Setting code as well
                    return 0.0;
                }
                char* tmp = new char[len+1];
                int j = 0;
                for (j=0;j<len;j++)
                {
                    tmp[j] = (char)((wchar_t*)value)[j];
                }
                tmp[len] = 0;
                float f = atof(tmp);
                delete tmp;
                return f;
            }
            
        case BytesType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (float)atof((char*)value);

        case OtherTypes:
        case DataObjectType:
        default:
            {
            string msg("Cannot get Float from object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("getFloat" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }
    
    /*const double TypeImpl::convertToDouble(void* value, unsigned int len) const
    {
    switch (typeEnum) 
        {
        case BooleanType:
        case ByteType:
        case CharacterType:
        case IntegerType: 
        case ShortType:
            if (value == 0) return 0;
            return (double)(*(long*)value);

        case DoubleType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (double)(*(long double*)value);

        case FloatType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (double)(*(float*)value);

        case LongType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (double)(*(int64_t*)value);

        case DateType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (double)(*(time_t*)value);

        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            {
                if (value == 0) 
                {
                    // Conversions might set from a zero value, as they are used
                    // by the default Setting code as well
                    return 0.0;
                }
                char* tmp = new char[len+1];
                int j = 0;
                for (j=0;j<len;j++)
                {
                    tmp[j] = (char)((wchar_t*)value)[j];
                }
                tmp[len] = 0;
                float f = atof(tmp);
                delete tmp;
                return (double)f;
            }
        case BytesType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (double)atof((char*)value);

        case OtherTypes:
        case DataObjectType:
        default:
            {
            string msg("Cannot get LongDouble from object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("getLongDouble" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }
*/
    const long double TypeImpl::convertToDouble(void* value, unsigned int len) const
    {
    switch (typeEnum) 
        {
        case BooleanType:
        case ByteType:
        case CharacterType:
        case IntegerType: 
        case ShortType:
            if (value == 0) return 0;
            return (long double)(*(long*)value);

        case DoubleType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (long double)(*(long double*)value);

        case FloatType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (long double)(*(float*)value);

        case LongType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (long double)(*(int64_t*)value);

        case DateType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (long double)(*(time_t*)value);

        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
            {
                if (value == 0) 
                {
                    // Conversions might set from a zero value, as they are used
                    // by the default Setting code as well
                    return 0.0;
                }
                char* tmp = new char[len+1];
                int j = 0;
                for (j=0;j<len;j++)
                {
                    tmp[j] = (char)((wchar_t*)value)[j];
                }
                tmp[len] = 0;
                float f = atof(tmp);
                delete tmp;
                return (long double)f;
            }    
            
        case BytesType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0.0;
            }
            return (long double)atof((char*)value);

        case OtherTypes:
        case DataObjectType:
            SDO_THROW_EXCEPTION("getLongDouble" ,
              SDOInvalidConversionException, "Not implemented on data object");
            break;

        default:
            {
            string msg("Cannot get LongDouble from object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("getLongDouble" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }
    const SDODate TypeImpl::convertToDate(void* value, unsigned int len) const
    {
    switch (typeEnum) 
        {
        case ByteType:
        case CharacterType:
        case IntegerType: 
        case ShortType:
            if (value == 0) return 0;
            return (time_t)(*(long*)value);

        case DoubleType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (time_t)(*(long double*)value);

        case FloatType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (time_t)(*(float*)value);

        case LongType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (time_t)(*(int64_t*)value);

        case DateType:
            if (value == 0) 
            {
                // Conversions might set from a zero value, as they are used
                // by the default Setting code as well
                return 0;
            }
            return (time_t)(*(time_t*)value);

        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case UriType:
        case BooleanType:     
        case BytesType:
        case OtherTypes:
        case DataObjectType:
        default:
            {
            string msg("Cannot get Date from object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("getDate" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }

    DataObject* TypeImpl::convertToDataObject(void* value, unsigned int len) const
    {
    switch (typeEnum) 
        {
        case OtherTypes:
        case DataObjectType:
            return (DataObject*)value;

        case BooleanType:
        case ByteType:
        case CharacterType:
        case IntegerType: 
        case ShortType:
        case DoubleType:
        case FloatType:
        case LongType:
        case DateType:
        case BigDecimalType: 
        case BigIntegerType: 
        case StringType:    
        case BytesType:
        case UriType:
        default:
            {
            string msg("Cannot get Data Object from object of type:");
            msg += getName();
            SDO_THROW_EXCEPTION("getDataObject" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
    }
};

};
