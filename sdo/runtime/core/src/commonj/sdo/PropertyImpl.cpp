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
 
#include <iostream>
using namespace std;

#ifndef SDO_EXPORTS 
#define SDO_EXPORTS
#endif
#include "commonj/sdo/export.h"


#include "commonj/sdo/Sequence.h"
#include "commonj/sdo/DataObject.h"
#include "commonj/sdo/TypeList.h"

#include "commonj/sdo/Type.h"
#include "commonj/sdo/TypeImpl.h"
#include "commonj/sdo/Property.h"
#include "commonj/sdo/PropertyImpl.h"
#include "commonj/sdo/DataFactoryImpl.h"
#include "commonj/sdo/SDORuntimeException.h"

namespace commonj{
    namespace sdo{


        Substitution::Substitution() : name(), type(0)
        {
        }

        // Standard Constructor
        Substitution::Substitution(DataFactoryPtr mdg,
            const SDOString& inname,
            const Type& intype) : name(inname)
        {
            DataFactory* f = (DataFactory*)mdg;

            type = ((DataFactoryImpl*)f)->findType(intype.getURI(),intype.getName());
        }

        // Copy constructor
        Substitution::Substitution(const Substitution& s) : name(s.name), type(s.type)
        {
        }

        Substitution::~Substitution()
        {
        }


    ///////////////////////////////////////////////////////////////////////////
    // construction by DAS 
    ///////////////////////////////////////////////////////////////////////////
    
    PropertyImpl::PropertyImpl(const Type& cont, 
                        const char* inname, 
                        const TypeImpl& intype, 
                        bool many ,    
                        bool ro ,
                        bool contain) : containertype(cont), type (intype)
    {
        // name = new char[strlen(inname)+1];
        // strcpy(name,inname);
		if (inname != 0)
		{
			name = inname;
		}
		else
		{
			name.erase();
		}
        defvalue = 0;
        defvaluelength = 0;
        opposite = 0;
        stringdef = 0;
        bisMany = many;
        bisReadOnly = ro;
        bisContainer = contain;
        bDefaulted=false;
        if (contain == false && intype.isDataObjectType())
        {
            bisReference = true;
        }
        else 
        {
            bisReference = false;
        }
    }

    PropertyImpl::PropertyImpl(const Type& cont,
        const SDOString& inname,
        const TypeImpl& intype,
        bool many,
        bool ro,
        bool contain) :
    containertype(cont),
        name(inname),
        type(intype),
        bisMany(many),
        bisReadOnly(ro),
        bisContainer(contain),
        bDefaulted(false),
        opposite(0),
        stringdef(0),
        defvalue(0),
        defvaluelength(0)
    {
        if (contain == false && intype.isDataObjectType())
        {
            bisReference = true;
        }
        else 
        {
            bisReference = false;
        }
    }

      PropertyImpl::PropertyImpl(const PropertyImpl& p) :
        type((*(p.getTypeImpl()))),
        containertype(p.getContainingType()),
        name(p.name),
        bisMany(p.bisMany),
        bisReadOnly(p.bisReadOnly),
        bisContainer(p.bisContainer),
        bDefaulted(false),
        opposite(0),
        defvalue(0),
        defvaluelength(0),
        stringdef(0)
      {
        if (bisContainer == false && type.isDataObjectType())
          {
            bisReference = true;
          }
        else 
          {
            bisReference = false;
          }
      }

    PropertyImpl::~PropertyImpl()
    {
        if (defvalue != 0) delete (char*)defvalue;
        if (stringdef != 0) delete stringdef;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Setting of attributes  by DAS 
    ///////////////////////////////////////////////////////////////////////////

    void PropertyImpl::setMany(bool many)
    {
        bisMany = many;
    }

    void PropertyImpl::setReadOnly(bool readonly)
    {
        bisReadOnly = readonly;
    }

    void PropertyImpl::setContainment(bool contains)
    {
        if (contains == true && type.isDataType())
        {
            return;
        }
        bisContainer = contains;
        if (bisContainer == false && type.isDataObjectType())
        {
            bisReference = true;
        }
        else 
        {
            bisReference = false;
        }
    }


    ///////////////////////////////////////////////////////////////////////////
    // Setting of defaults by DAS 
    ///////////////////////////////////////////////////////////////////////////

    void PropertyImpl::setDefault(bool b )
    {
        setDefaultBoolean(b);
    }

    void PropertyImpl::setDefault(char c )
    {
        setDefaultByte(c);
    }

    void PropertyImpl::setDefault(wchar_t c )
    {
        setDefaultCharacter(c);
    }
    void PropertyImpl::setDefault(char* c )
    {
        setDefaultCString(c);
    }
    void PropertyImpl::setDefault(const SDOString& c)
    {
        setDefaultCString(c);
    }
    void PropertyImpl::setDefault(short s )
    {
        setDefaultShort(s);
    }
#if __WORDSIZE !=64
    void PropertyImpl::setDefault(long i )
    {
        setDefaultInteger(i);
    }
#endif
    void PropertyImpl::setDefault(int64_t i )
    {
        setDefaultLong(i);
    }
    void PropertyImpl::setDefault(float f )
    {
        setDefaultFloat(f);
    }
    void PropertyImpl::setDefault(long double d )
    {
        setDefaultDouble(d);
    }
    void PropertyImpl::setDefault(const SDODate d )
    {
        setDefaultDate(d);
    }
    void PropertyImpl::setDefault(const char* c , unsigned int len)
    {
        setDefaultBytes(c, len);
    }
    void PropertyImpl::setDefault(const SDOString& c , unsigned int len)
    {
        setDefaultBytes(c, len);
    }

    void PropertyImpl::setDefault(const wchar_t* c, unsigned int len )
    {
        setDefaultString(c, len);
    }

     ///////////////////////////////////////////////////////////////////////////
    // Substitution groups.
       ///////////////////////////////////////////////////////////////////////////
    const Type* PropertyImpl::getSubstitutionType(const char* inname) const 
    {
        for (int i=0;i<substitutions.size();i++)
        {
            if (!strcmp(inname, substitutions[i].name.c_str()))
            {
                return substitutions[i].type;
            }
        }
        return 0;
    }

    const char * PropertyImpl::getSubstitutionName(unsigned int index) const 
    {
        if (index < getSubstitutionCount())
        {
            return (substitutions[index].name.c_str());
        }
        SDO_THROW_EXCEPTION("getSubstitutionName", SDOIndexOutOfRangeException,
            "index out of range");
    }

    unsigned int PropertyImpl::getSubstitutionCount(void) const
    {
        return substitutions.size();
    }

    const Type* PropertyImpl::getSubstitutionType(unsigned int index) const 
    {
        if (index < getSubstitutionCount())
        {
            return (substitutions[index].type);
        }
        return 0;
    }

    void PropertyImpl::setSubstitution(DataFactoryPtr mdg, const char* alias, 
                                  const Type& substype)
    {
        substitutions.push_back(Substitution(mdg,alias,substype));
    }
    

     ///////////////////////////////////////////////////////////////////////////
    // Returns the name of the property.
       ///////////////////////////////////////////////////////////////////////////
    const char* PropertyImpl::getName() const
    {
          return name.c_str();
    }

//     const SDOString& PropertyImpl::getName() const
//     {
//           return name;
//     }

    void PropertyImpl::setAlias(const char* alias)
    {
        char* tmp = new char[strlen(alias)+1];
        strcpy(tmp,alias);
        aliases.push_back(tmp); 
    }

      void PropertyImpl::setAlias(const SDOString& alias)
      {
        aliases.push_back(alias);
      }

    const char* PropertyImpl::getAlias(unsigned int index) const
    {
        if (index < aliases.size())
        {
            return aliases[index].c_str();
        }
        return name.c_str();
    }

    unsigned int PropertyImpl::getAliasCount() const
    {
        return aliases.size();
    }

  
    ///////////////////////////////////////////////////////////////////////////
    // Returns the type of the property.
     ///////////////////////////////////////////////////////////////////////////
    const Type& PropertyImpl::getType() const
    {
        return (Type&)type;
    }

    const Type::Types PropertyImpl::getTypeEnum() const
    {
        return type.getTypeEnum();
    }

     ///////////////////////////////////////////////////////////////////////////
    // Returns the type of the property.
    ///////////////////////////////////////////////////////////////////////////
     const TypeImpl* PropertyImpl::getTypeImpl() const
    {
        return &type;
    }
  
    ///////////////////////////////////////////////////////////////////////////
    // Returns whether the property is many-valued.
    ///////////////////////////////////////////////////////////////////////////
    bool PropertyImpl::isMany() const
    {
          if(getTypeImpl()->isFromList())return true; 
          return bisMany;
    }
  
     ///////////////////////////////////////////////////////////////////////////
    // Returns whether the property is containment.
    ///////////////////////////////////////////////////////////////////////////
    bool PropertyImpl::isContainment() const
    {
          return bisContainer;
    }

     ///////////////////////////////////////////////////////////////////////////
    // Returns whether the property is containment.
    ///////////////////////////////////////////////////////////////////////////
    bool PropertyImpl::isReference() const
    {
          return bisReference;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Returns the containing type of this property.
    ///////////////////////////////////////////////////////////////////////////
    const Type& PropertyImpl::getContainingType() const
    {
          return containertype;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Returns the opposite.
    ///////////////////////////////////////////////////////////////////////////
    const Property* PropertyImpl::getOpposite() const
    {
          return opposite;
    }

    void PropertyImpl::setOpposite(const Property* opp) 
    {
        opposite = opp;
    }
    ///////////////////////////////////////////////////////////////////////////
    // Returns the default value this property will have in a 
    // data object where the property hasn't been set.
     ///////////////////////////////////////////////////////////////////////////

    // check whether the property has a default first
    bool PropertyImpl::isDefaulted() const
    {
        return bDefaulted;
    }

    const char*      PropertyImpl::getCStringDefault() const 
    {
        PropertyImpl* p = (PropertyImpl*)this;
        return getTypeImpl()->convertToCString(defvalue, &(p->stringdef), defvaluelength);
    }
    bool        PropertyImpl::getBooleanDefault() const
    {
        return getTypeImpl()->convertToBoolean(defvalue,defvaluelength);
    }
    char        PropertyImpl::getByteDefault() const
    {
        return getTypeImpl()->convertToByte(defvalue,defvaluelength);
    }
    wchar_t     PropertyImpl::getCharacterDefault() const
    {
        return getTypeImpl()->convertToCharacter(defvalue,defvaluelength);
    }
    short       PropertyImpl::getShortDefault() const
    {
        return getTypeImpl()->convertToShort(defvalue,defvaluelength);
    }
    long        PropertyImpl::getIntegerDefault() const
    {
        return getTypeImpl()->convertToInteger(defvalue,defvaluelength);
    }
    int64_t     PropertyImpl::getLongDefault() const
    {
        return getTypeImpl()->convertToLong(defvalue,defvaluelength);
    }
    float       PropertyImpl::getFloatDefault() const
    {
        return getTypeImpl()->convertToFloat(defvalue,defvaluelength);
    }
    long double PropertyImpl::getDoubleDefault() const
    {
        return getTypeImpl()->convertToDouble(defvalue,defvaluelength);
    }
    const SDODate      PropertyImpl::getDateDefault() const 
    {
        return getTypeImpl()->convertToDate(defvalue,defvaluelength);
    }
    unsigned int PropertyImpl::getStringDefault(wchar_t* val, unsigned int max) const
    {
        if (val == 0 || max == 0) return defvaluelength; 
        return getTypeImpl()->convertToString(defvalue, val, defvaluelength, max);
    
    }
    unsigned int PropertyImpl::getBytesDefault(char* val, unsigned int max) const
    {
        if (val == 0 || max == 0) return defvaluelength; 
        return getTypeImpl()->convertToBytes(defvalue, val, defvaluelength, max);
    }

    unsigned int PropertyImpl::getBytesDefault(SDOString& val, unsigned int max) const
    {
        if (max == 0) return defvaluelength; 
        return getTypeImpl()->convertToBytes(defvalue, val, defvaluelength, max);
    }

    unsigned int PropertyImpl::getDefaultLength() const
    {
        return defvaluelength;
    }

    ///////////////////////////////////////////////////////////////////////////
    // sets the default value by type
    ///////////////////////////////////////////////////////////////////////////

    void PropertyImpl::setDefaultCString(const char* s) 
    {
        bDefaulted=true;
        defvaluelength = getTypeImpl()->convert(&defvalue,s); 
    }
    void PropertyImpl::setDefaultCString(const SDOString& s) 
    {
        bDefaulted=true;
        defvaluelength = getTypeImpl()->convert(&defvalue, s); 
    }
    void PropertyImpl::setDefaultString(    const wchar_t* c , unsigned int len )
    {
        bDefaulted=true;
        defvaluelength = getTypeImpl()->convert(&defvalue,c, len); 
    }
    void PropertyImpl::setDefaultBytes(    const char* c , unsigned int len )
    {
        bDefaulted=true;
        defvaluelength = getTypeImpl()->convert(&defvalue,c, len); 
    }
    void PropertyImpl::setDefaultBytes(const SDOString& c , unsigned int len)
    {
        bDefaulted=true;
        defvaluelength = getTypeImpl()->convert(&defvalue,c, len); 
    }
    void PropertyImpl::setDefaultBoolean(    const bool b  )
    {
        bDefaulted=true;
        defvaluelength = getTypeImpl()->convert(&defvalue,b); 
    }
    void PropertyImpl::setDefaultByte(    const char c   )
    {
        bDefaulted=true;
        defvaluelength = getTypeImpl()->convert(&defvalue,c); 
    }
    void PropertyImpl::setDefaultCharacter(   const wchar_t c)
    {
        bDefaulted=true;
        defvaluelength = getTypeImpl()->convert(&defvalue,c); 
    }
    void PropertyImpl::setDefaultShort(   const short s  )
    {
        bDefaulted=true;
        defvaluelength = getTypeImpl()->convert(&defvalue,s); 
    }
    void PropertyImpl::setDefaultInteger( const long i    )
    {
        bDefaulted=true;
#if __WORDSIZE ==64
        defvaluelength = getTypeImpl()->convert(&defvalue,(int64_t)i); 
#else
        defvaluelength = getTypeImpl()->convert(&defvalue,i); 
#endif
    }
    void PropertyImpl::setDefaultLong(const int64_t l)
    {
        bDefaulted=true;
        defvaluelength = getTypeImpl()->convert(&defvalue,l); 
    }
    void PropertyImpl::setDefaultFloat(   const float f  )
    {
        bDefaulted=true;
        defvaluelength = getTypeImpl()->convert(&defvalue,f); 
    }
    void PropertyImpl::setDefaultDouble(  const long double d )
    {
        bDefaulted=true;
        defvaluelength = getTypeImpl()->convert(&defvalue,d); 
    }
    void PropertyImpl::setDefaultDate(    const SDODate d )
    {
        bDefaulted=true;
        defvaluelength = getTypeImpl()->convertDate(&defvalue,d); 
    }

    ///////////////////////////////////////////////////////////////////////////
    // Returns true if values for this Property cannot be modified using the SDO APIs.
    // When true, DataObject.set(Property property, Object value) throws an exception.
    // Values may change due to other factors, such as services operating on DataObjects.
     ///////////////////////////////////////////////////////////////////////////
    bool PropertyImpl::isReadOnly() const
    {
          return bisReadOnly;
    }
  
};
};


