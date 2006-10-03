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

/* $Rev$ $Date$ */

#include "commonj/sdo/Setting.h"
#include "commonj/sdo/TypeImpl.h"

namespace commonj{ 
namespace sdo {
    
    Setting::Setting(bool set, bool nul, void* invalue, unsigned int len, const Property& p, unsigned int inindex)
        : bisSet(set), bisNull(nul) , value(invalue),theProp(&p)
    {
        index = inindex;
        length  = len;
        strbuf = 0;
    }

    void Setting::copy(const Setting& s)
    {
        bisSet = s.isSet();
        bisNull = s.isNull();
        index = s.getIndex();
        length = s.getLength();
        theProp = &(s.getProperty());
        strbuf = 0;
        switch (theProp->getTypeEnum())
        {
            case Type::BooleanType:
                value = new long;
                *(long*)value = (long)s.getBooleanValue();
                break;
            case Type::ByteType:
                value = new long;
                *(long*)value = (long)s.getByteValue();
                break;
            case Type::CharacterType:
                value = new long;
                *(long*)value = (long)s.getCharacterValue();
                break;
            case Type::IntegerType: 
                value = new long;
                *(long*)value = (long)s.getIntegerValue();
                break;
            case Type::ShortType:
                value = new long;
                *(long*)value = (long)s.getShortValue();
                break;
            case Type::DoubleType:
                value = new long double;
                *(long double*)value = s.getDoubleValue();
                break;
            case Type::FloatType:
                value = new float;
                *(float*)value = s.getFloatValue();
                break;
            case Type::LongType:
                value = new int64_t;
                *(int64_t*)value = s.getLongValue();
                break;
            case Type::DateType:
                value = new long;
                *(long*)value = long(s.getDateValue().getTime());
                break;
            case Type::BigDecimalType: 
            case Type::BigIntegerType: 
            case Type::StringType: 
            case Type::UriType:
                value = new wchar_t[length];
                s.getStringValue((wchar_t*)value,length);
                break;
            case Type::BytesType:
                value = new char[length];
                s.getBytesValue((char*)value,length);
                break;
            case Type::OtherTypes:
            case Type::DataObjectType:
            case Type::ChangeSummaryType:
                value = s.getDataObjectValue();
            default:
                break;
        }
    }

    Setting& Setting::operator=(const Setting& s)
    {
        if (this == &s) return *this;
        copy(s);
        return *this;
    }

    Setting::Setting(const Setting& s)
    {
        copy(s);
    }

    Setting::~Setting()
    {
        // The value has been allocated for everything except data objects

        switch (theProp->getTypeEnum())
        {
            case Type::BooleanType:
            case Type::ByteType:
            case Type::CharacterType:
            case Type::IntegerType: 
            case Type::ShortType:
                delete (long*)value;
                break;
            case Type::DoubleType:
                delete (long double*)value;
                break;
            case Type::FloatType:
                delete (float*)value;
                break;
            case Type::LongType:
                delete (int64_t*)value;
                break;
            case Type::DateType:
                delete (long*)value;
                break;
            case Type::BigDecimalType: 
            case Type::BigIntegerType: 
            case Type::StringType: 
            case Type::UriType:
                delete (wchar_t*)value;
                break;
            case Type::BytesType:
                delete (char*)value;
                break;
            case Type::OtherTypes:
            case Type::DataObjectType:
            case Type::ChangeSummaryType:
            default:
                break;
        }
        if (strbuf != 0) delete strbuf;
    }

    const Property& Setting::getProperty() const
    {
      return *theProp;
    }

    const Type& Setting::getType() const
    {
      return theProp->getType();
    }

    bool Setting::getBooleanValue() const
    {
        TypeImpl* t = (TypeImpl*)&(getType());
        return t->convertToBoolean(value, length);
        //if (value == 0) return false;
        //if (*(long*)value != 0) {
        //    return true;
        //}
        //return false;
    }

    char Setting::getByteValue() const
    {
        TypeImpl* t = (TypeImpl*)&(getType());
        return t->convertToByte(value, length);
        //if (value == 0) return 0;
        //return (char)*(long*)value;
    }

    wchar_t Setting::getCharacterValue() const
    {
        TypeImpl* t = (TypeImpl*)&(getType());
        return t->convertToCharacter(value, length);
        //if (value == 0) return 0;
        //return (wchar_t)*(long*)value;
    }
    
    unsigned int Setting::getBytesValue(char* buffer, unsigned int max) const
    {
        if (buffer == 0 && max == 0) return length;

        TypeImpl* t = (TypeImpl*)&(getType());
        return t->convertToBytes(value, buffer,length, max);
        //int i;
        //char* c = (char*)value;
        //for (i=0;i < max && i < length;i++)
        //{
        //    buffer[i] = (char)*c;
        //    c++;
        //}
        //return i;
    }
    
    unsigned int Setting::getStringValue(wchar_t* buffer, unsigned int max) const
    {
        if (buffer == 0 && max == 0) return length;

        TypeImpl* t = (TypeImpl*)&(getType());
        return t->convertToString(value, buffer,length, max);
        //int i;
        //wchar_t* c = (wchar_t*)value;
        //for (i=0;i < max && i < length;i++)
        //{
        //    buffer[i] = (wchar_t)*c;
        //    c++;
        //}
        //return i;
    }

    short Setting::getShortValue() const
    {
        TypeImpl* t = (TypeImpl*)&(getType());
        return t->convertToShort(value,length);
        //return (short)*(long*)value;
    }

    long Setting::getIntegerValue() const
    {
        TypeImpl* t = (TypeImpl*)&(getType());
        return t->convertToInteger(value, length);
        //return *(long*)value;
    }


    int64_t Setting::getLongValue() const
    {
        TypeImpl* t = (TypeImpl*)&(getType());
        return t->convertToLong(value, length);
        //return (int64_t)*(int64_t*)value;
    }

    float Setting::getFloatValue() const
    {
        TypeImpl* t = (TypeImpl*)&(getType());
        return t->convertToFloat(value, length);
        //return (float)*(float*)value;
    }

    const SDODate Setting::getDateValue() const
    {
        TypeImpl* t = (TypeImpl*)&(getType());
        return t->convertToDate(value, length);
    }


    long double Setting::getDoubleValue() const
    {
        TypeImpl* t = (TypeImpl*)&(getType());
        return t->convertToDouble(value, length);
        //return (long double)*(long double*)value;
    }

    const char* Setting::getCStringValue()
    {
        TypeImpl* t = (TypeImpl*)&(getType());
        return t->convertToCString(value, &strbuf, length);
        //return (const char*)value;
    }

    RefCountingPointer<DataObject> Setting::getDataObjectValue() const
    {
        DataObject *dob = (DataObject*)value;
        return RefCountingPointer<DataObject>(dob);
    }


    unsigned int Setting::getLength() const
    {
        return length;
    }

    unsigned int Setting::getIndex() const
    {
        return index;
    }

    bool Setting::isSet() const
    {
        return bisSet;
    }

    bool Setting::isNull() const
    {
        return bisNull;
    }

};
};

