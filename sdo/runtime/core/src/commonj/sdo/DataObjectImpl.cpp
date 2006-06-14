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

#include "commonj/sdo/disable_warn.h"
#include "commonj/sdo/DataObjectImpl.h"


 
#include "commonj/sdo/SDORuntimeException.h"

#include "commonj/sdo/Property.h"
#include "commonj/sdo/Type.h"
#include "commonj/sdo/TypeList.h"
#include "commonj/sdo/Sequence.h"
#include "commonj/sdo/SequenceImpl.h"

#include "commonj/sdo/PropertyList.h"

#include "commonj/sdo/Logging.h"

#include "commonj/sdo/TypeImpl.h"

#include "commonj/sdo/ChangeSummaryImpl.h"
#include "commonj/sdo/DataFactoryImpl.h"

#include <string>
#include <stdio.h>
using std::string;




namespace commonj{
namespace sdo {

    /**
     * RDO is an internal class holding a property value
     */

    rdo::rdo(unsigned int infirst, DataObjectImpl* insecond)
        : first(infirst), second(insecond)
    {
    }

    rdo::rdo()
    {
        first = 0;
        second = 0;
    }

    rdo::rdo (const rdo& inrdo)
    {
        first = inrdo.first;
        second = inrdo.second;
    }

    rdo::~rdo()
    {
    }


/** @def getPrimitive
 *
 * A macro for the getting of primitive values from a data object
 */

#define getPrimitive(primval,retval,defval)\
    retval DataObjectImpl::get ##primval (unsigned int propertyIndex)\
    {\
        validateIndex(propertyIndex);\
        PropertyImpl* p = getPropertyImpl(propertyIndex);\
        if (p != 0 ) \
        {\
            if (p->isMany() || p->getTypeImpl()->isFromList())\
            {\
                string msg("Get value not available on many valued property:");\
                msg += p->getName();\
                SDO_THROW_EXCEPTION("get value", SDOUnsupportedOperationException,\
                    msg.c_str());\
            }\
             DataObjectImpl* d = getDataObjectImpl(propertyIndex);\
            if (d != 0) \
            {\
                if (d->isNull())return (retval)0;\
                return d->get ##primval ();\
            }\
            if (isSet(propertyIndex)) return (retval)0;\
            return p->get ##primval ##Default();\
        }\
        return (retval)0;\
    }

/** @def getCharsBasic
 *
 * A macro for the getting of primitive characters from a data object
 */

#define getCharsBasic(primval,retval,defval)\
    unsigned int DataObjectImpl::get ##primval (unsigned int propertyIndex, retval valptr , unsigned int max)\
    {\
        validateIndex(propertyIndex);\
          PropertyImpl* p = getPropertyImpl(propertyIndex);\
        if (p != 0) \
        {\
            if (p->isMany()|| p->getTypeImpl()->isFromList())\
            {\
                string msg("Get value not available on many valued property:");\
                msg += p->getName();\
                SDO_THROW_EXCEPTION("character getter", SDOUnsupportedOperationException,\
                    msg.c_str());\
            }\
             DataObjectImpl* d = getDataObjectImpl(propertyIndex);\
            if (d != 0) \
            { \
                if (d->isNull()) return 0;\
                return d->get ##primval ( valptr , max);\
            }\
            if (isSet(propertyIndex))return 0;\
            return p->get ##primval ##Default( valptr , max);\
        }\
        return 0;\
    }


/** @def setPrimitive
 *
 * A macro for the setting of primitive values in a data object
 */

#define setPrimitive(primval,primtype,primnam)\
    void DataObjectImpl::set ##primval (unsigned int propertyIndex, primtype value)\
    {\
        validateIndex(propertyIndex);\
        PropertyImpl* pl = getPropertyImpl(propertyIndex);\
        if (pl != 0) \
        {\
            if (pl->isMany()|| pl->getTypeImpl()->isFromList())\
            {\
                string msg("Set value not available on many valued property:");\
                msg += pl->getName();\
                SDO_THROW_EXCEPTION("set value",SDOUnsupportedOperationException,\
                msg.c_str());\
            }\
            PropertyValueMap::iterator i;\
            for (i = PropertyValues.begin(); i != PropertyValues.end();++i)\
            {\
                if ((*i).first == propertyIndex)\
                {\
                    logChange(propertyIndex);\
                    (*i).second->unsetNull();\
                    (*i).second->set ##primval (value);\
                    return;\
                }\
            }\
            DataFactory* df = getDataFactory();\
            DataObjectImpl* b = new DataObjectImpl(df, df->getType(Type::SDOTypeNamespaceURI, primnam));\
            b->setContainer(this);\
            b->setApplicableChangeSummary();\
            logChange(propertyIndex);\
            PropertyValues.insert(PropertyValues.end(), rdo(propertyIndex,b));\
             b->set ##primval (value);\
        }\
        return;\
    }


/** @def setCharsBasic
 *
 * A macro for the setting of primitive characters in a data object
 */

#define setCharsBasic(primval,primtype,primnam)\
    void DataObjectImpl::set ##primval (unsigned int propertyIndex, primtype value, unsigned int len)\
    {\
        validateIndex(propertyIndex);\
        PropertyImpl* pl = getPropertyImpl(propertyIndex);\
        if (pl != 0) \
        {\
            if (pl->isMany()|| pl->getTypeImpl()->isFromList())\
            {\
                string msg("Set value not available on many valued property:");\
                msg += pl->getName();\
                SDO_THROW_EXCEPTION("setter",SDOUnsupportedOperationException,\
                msg.c_str());\
            }\
            PropertyValueMap::iterator i;\
            for (i = PropertyValues.begin(); i != PropertyValues.end();++i)\
            {\
                if ((*i).first == propertyIndex)\
                {\
                    logChange(propertyIndex);\
                    (*i).second->unsetNull();\
                    (*i).second->set ##primval (value, len);\
                    return;\
                }\
            }\
            DataFactory* df = getDataFactory();\
            DataObjectImpl* b = new DataObjectImpl(df, df->getType(Type::SDOTypeNamespaceURI, primnam));\
            b->setContainer(this);\
            b->setApplicableChangeSummary();\
            logChange(propertyIndex);\
            PropertyValues.insert(PropertyValues.end(),rdo(propertyIndex,b));\
             b->set ##primval (value, len);\
        }\
        return;\
    }


/** @def getPrimitiveFromPath
 *
 * A macro for the getting of primitive values from a data object by path
 */

#define getPrimitiveFromPath(primval, retval, defval)\
    retval DataObjectImpl::get ##primval (const char* path)\
    {\
        DataObjectImpl* d;\
        char *spath = 0;\
        char* prop = 0;\
         try {\
            spath = DataObjectImpl::stripPath(path);\
              prop = findPropertyContainer(spath,&d);\
            if (spath){\
                delete spath;\
                spath = 0;\
            }\
            if (d != 0) {\
                if (prop == 0 || (strlen(prop) == 0)) {\
                    return d->get ##primval ();\
                }\
                else {\
                    PropertyImpl* p = d->getPropertyImpl(prop);\
                    if (p != 0) \
                    {\
                        if (p->isMany()|| p->getTypeImpl()->isFromList())\
                        {\
                            long l;\
                            DataObjectImpl* doi = d->findDataObject(prop,&l);\
                            delete prop;\
                            prop = 0;\
                            if (doi != 0)    {\
                                return doi->get ## primval();\
                            }\
                            string msg("Get value - index out of range:");\
                            msg += path;\
                            SDO_THROW_EXCEPTION("getter", SDOIndexOutOfRangeException,\
                            msg.c_str());\
                        }\
                        else\
                        {\
                            delete prop;\
                            prop = 0;\
                            if (!d->isSet(*p)) {\
                                    return p->get ##primval ##Default();\
                            }\
                            return d->get ##primval (*p);\
                        }\
                    }\
                }\
                if (prop) {\
                    delete prop;\
                    prop = 0;\
                }\
            }\
            string msg("Get value  - path not found");\
            SDO_THROW_EXCEPTION("getter", SDOPathNotFoundException,\
            msg.c_str());\
        }\
        catch (SDORuntimeException e) {\
           if (spath)delete spath;\
           if (prop) delete prop;\
           SDO_RETHROW_EXCEPTION("getter", e);\
        }\
    }

#define getPrimitiveFromPathUsingSDOString(primval, retval, defval)\
    retval DataObjectImpl::get ##primval (const SDOString& path)\
    {\
        DataObjectImpl* d;\
        SDOString spath;\
        SDOString prop;\
         try {\
            DataObjectImpl::stripPath(path, spath);\
            prop = findPropertyContainer(spath, &d);\
            if (d != 0) {\
                if (prop.empty()) {\
                    return d->get ##primval ();\
                }\
                else {\
                    PropertyImpl* p = d->getPropertyImpl(prop);\
                    if (p != 0) \
                    {\
                        if (p->isMany()|| p->getTypeImpl()->isFromList())\
                        {\
                            long l;\
                            DataObjectImpl* doi = d->findDataObject(prop,&l);\
                            if (doi != 0)    {\
                                return doi->get ## primval();\
                            }\
                            string msg("Get value - index out of range:");\
                            msg += path;\
                            SDO_THROW_EXCEPTION("getter", SDOIndexOutOfRangeException,\
                            msg.c_str());\
                        }\
                        else\
                        {\
                            if (!d->isSet(*p)) {\
                                    return p->get ##primval ##Default();\
                            }\
                            return d->get ##primval (*p);\
                        }\
                    }\
                }\
            }\
            string msg("Get value  - path not found");\
            SDO_THROW_EXCEPTION("getter", SDOPathNotFoundException,\
            msg.c_str());\
        }\
        catch (SDORuntimeException e) {\
           SDO_RETHROW_EXCEPTION("getter", e);\
        }\
    }

/** @def getCharsFromPath
 *
 * A macro for the getting of primitive characters from a data object by path
 */

#define getCharsFromPath(primval, retval, defval)\
    unsigned int DataObjectImpl::get ##primval (const char* path, retval valptr , unsigned int max)\
    {\
        DataObjectImpl* d;\
        char *spath = 0;\
        char* prop = 0;\
         try {\
            spath = DataObjectImpl::stripPath(path);\
              prop = findPropertyContainer(spath,&d);\
            if (spath){\
                delete spath;\
                spath = 0;\
            }\
            if (d != 0) {\
                if (prop == 0 || (strlen(prop) == 0)) {\
                    return d->get ##primval ( valptr , max);\
                }\
                else {\
                    PropertyImpl* p = d->getPropertyImpl(prop);\
                    if (p != 0)\
                    {\
                        if (p->isMany() || p->getTypeImpl()->isFromList())\
                        {\
                            long l;\
                            DataObjectImpl* doi = d->findDataObject(prop,&l);\
                            delete prop;\
                            prop = 0;\
                            if (doi != 0)    {\
                                return doi->get ## primval (valptr, max);\
                            }\
                            string msg("Get value - index out of range");\
                            msg += path;\
                            SDO_THROW_EXCEPTION("getChars", SDOIndexOutOfRangeException,\
                            msg.c_str());\
                        }\
                        else { \
                            delete prop;\
                            prop = 0;\
                            if (!d->isSet(*p)) {\
                                return p->get ##primval ##Default( valptr , max );\
                            }\
                            return d->get ##primval (*p, valptr , max);\
                        }\
                    }\
                }\
                if (prop) {\
                    delete prop;\
                    prop = 0;\
                }\
            }\
            string msg("Get value  - path not found");\
            SDO_THROW_EXCEPTION("getCString", SDOPathNotFoundException,\
            msg.c_str());\
        }\
        catch (SDORuntimeException e) {\
           if (spath)delete spath;\
           if (prop) delete prop;\
           SDO_RETHROW_EXCEPTION("character getter", e);\
        }\
    }



/** @def setPrimitiveFromPath
 *
 * A macro for the setting of primitive values in a data object by path
 */

#define setPrimitiveFromPath(primval,setval,platval)\
    void DataObjectImpl::set ##primval (const char* path, setval value)\
    {\
        DataObjectImpl *d;\
        char* spath = 0;\
        char* prop = 0;\
        try {\
            spath = DataObjectImpl::stripPath(path);\
              prop = findPropertyContainer(spath,&d);\
            if (spath) {\
                delete spath;\
                spath = 0;\
            }\
            if (d != 0)\
            {\
                if (prop == 0 || (strlen(prop) == 0)) {\
                    d->set ##primval (value);\
                }\
                else {\
                    const PropertyImpl* p = d->getPropertyImpl(prop);\
                    if (p == 0 && d->getType().isOpenType()) \
                    {\
                        p = d->define ##primval (prop);\
                    }\
                    if (p == 0)\
                    {\
                         string msg("Set value  - path not found");\
                         SDO_THROW_EXCEPTION("setter", SDOPathNotFoundException,\
                         msg.c_str());\
                    }\
                    if (p->isMany()|| p->getTypeImpl()->isFromList())\
                    {\
                        long l;\
                        DataObjectList& dol = d->getList((Property&)*p);\
                        DataObjectImpl* doi = d->findDataObject(prop,&l);\
                        delete prop;\
                        prop = 0;\
                        if (doi != 0)    {\
                             doi->set ## primval (value);\
                        }\
                        else {\
                            dol.append(( platval) value);\
                        }\
                    }\
                    else {\
                        delete prop;\
                        prop = 0;\
                        d->set ##primval ((Property&)*p,value);\
                    }\
                }\
            }\
            if (prop){\
                delete prop;\
                prop = 0;\
            }\
        }\
        catch (SDORuntimeException e) {\
            if (spath) delete spath;\
            if (prop) delete prop;\
            SDO_RETHROW_EXCEPTION("setter",e);\
        }\
    }

/** @def setCharsFromPath
 *
 * A macro for the setting of primitive characters in a data object by path
 */

#define setCharsFromPath(primval,setval)\
    void DataObjectImpl::set ##primval (const char* path, setval value, unsigned int len)\
    {\
        DataObjectImpl *d;\
        char* spath = 0;\
        char* prop = 0;\
        try {\
            spath = DataObjectImpl::stripPath(path);\
              prop = findPropertyContainer(spath,&d);\
            if (spath) {\
                delete spath;\
                spath = 0;\
            }\
            if (d != 0)\
            {\
                if (prop == 0 || (strlen(prop) == 0)) {\
                    d->set ##primval (value, len);\
                }\
                else {\
                    const PropertyImpl* p = d->getPropertyImpl(prop);\
                    if (p == 0 && d->getType().isOpenType())\
                    {\
                        p = d->define ##primval (prop);\
                    }\
                    if (p == 0)\
                    {\
                         string msg("Set value  - path not found");\
                         SDO_THROW_EXCEPTION("setter", SDOPathNotFoundException,\
                         msg.c_str());\
                    }\
                    if (p->isMany()|| p->getTypeImpl()->isFromList())\
                    {\
                        long l;\
                        DataObjectList& dol = d->getList((Property&)*p);\
                        DataObjectImpl* doi = d->findDataObject(prop,&l);\
                        delete prop;\
                        prop = 0;\
                        if (doi != 0)    {\
                            doi->set ## primval (value, len);\
                        }\
                        else {\
                            dol.append(value,len);\
                        }\
                    }\
                    else { \
                        delete prop;\
                        prop = 0;\
                        d->set ##primval ((Property&)*p,value, len);\
                    }\
                }\
            }\
            if (prop){\
                delete prop;\
                prop = 0;\
            }\
        }\
        catch (SDORuntimeException e) {\
            if (spath) delete spath;\
            if (prop) delete prop;\
            SDO_RETHROW_EXCEPTION("setter",e);\
        }\
    }




/** @def getPrimitiveFromProperty
 *
 * A macro for the getting of primitive values in a data object by property
 */

#define getPrimitiveFromProperty(primval,retval)\
    retval DataObjectImpl::get ##primval (const Property& property)\
    {\
        return get ##primval (getPropertyIndex(property));\
    }


/** @def getCharsFromProperty
 *
 * A macro for the getting of primitive characters in a data object by property
 */

#define getCharsFromProperty(primval,retval)\
    unsigned int DataObjectImpl::get ##primval (const Property& property, retval val, unsigned int max)\
    {\
        return get ##primval (getPropertyIndex(property), val, max);\
    }


/** @def setPrimitiveFromProperty
 *
 * A macro for the setting of primitive values in a data object by property
 */

#define setPrimitiveFromProperty(primval,primtype)\
  void DataObjectImpl::set ##primval (const Property& property, primtype value)\
  {\
      set ##primval (getPropertyIndex(property),value);\
  }

/** @def setCharsFromProperty
 *
 * A macro for the setting of primitive characters in a data object by property
 */

#define setCharsFromProperty(primval,primtype)\
  void DataObjectImpl::set ##primval (const Property& property, primtype value, unsigned int len)\
  {\
      set ##primval (getPropertyIndex(property),value, len);\
  }



 /**  DataObject
  *  DataObjects are the non-primitive members of a Data graph.
  *
  * A data object is a representation of some structured data. 
  * it is the fundamental component in the SDO (Service Data Objects) package.
  * Data objects support reflection, path-based accesss, convenience creation 
  * and deletion methods,and the ability to be part of a data graph.
  * Each data object holds its data as a series of properties. 
  * Properties can be accessed by name, property index, or using the property 
  * meta object itself. 
  * A data object can also contain references to other data objects, through 
  * reference-type properties.
  * A data object has a series of convenience accessors for its properties. 
  * These methods either use a path (String), a property index, 
  * or the property's meta object itself, to identify the property.
  * Some examples of the path-based accessors are as follows:
  * DataObjectPtr company = ...;
  * company->getString("name");                   
  * company->setString("name", "acme");
  * company->getString("department.0/name")       
  *                                        .n  indexes from 0.
  * company->getString("department[1]/name")      [] indexes from 1.
  * company->getDataObject("department[number=123]")  returns the department where number=123
  * company->getDataObject("..")                      returns the containing data object
  * company->getDataObject("/")                       returns the root containing data object
  * There are specific accessors for the primitive types and commonly used 
  * data types like String.
  */


void DataObjectImpl::handlePropertyNotSet(const char* name)
{
    // change of behaviour - no longer throw for this.

//    string msg("Get value on unset and undefaulted property:");
//    msg += name;
//    SDO_THROW_EXCEPTION("get value", SDOPropertyNotSetException,
//        msg.c_str());
}



    // setters and getters from a path specification 

    getCharsFromPath(String, wchar_t* , 0);
    getCharsFromPath(Bytes, char* , 0);
    setCharsFromPath(String, const wchar_t*);
    setCharsFromPath(Bytes, const char*);
    getCharsFromProperty(String,wchar_t*);
    getCharsFromProperty(Bytes,char*);
    setCharsFromProperty(String,const wchar_t*);
    setCharsFromProperty(Bytes,const char*);
    getCharsBasic(String,wchar_t*,0);
    getCharsBasic(Bytes,char*,0);
    setCharsBasic(String,const wchar_t*,"String");
    setCharsBasic(Bytes,const char*,"Bytes");


    // Convenience methods for string/bytes length

    unsigned int DataObjectImpl::getLength(const Property& p)
    {
        switch (p.getType().getTypeEnum()) {
        case Type::BooleanType:
            return BOOL_SIZE;
        case Type::CharacterType:
        case Type::ByteType:
            return BYTE_SIZE;
        case Type::ShortType:
        case Type::IntegerType:
        case Type::LongType:
            return MAX_LONG_SIZE;
        case Type::FloatType:
            return MAX_FLOAT_SIZE;
        case Type::DoubleType:
            return MAX_DOUBLE_SIZE;
        case Type::BigDecimalType:
        case Type::BigIntegerType:
        case Type::UriType:
        case Type::StringType:
            return getString(p,0,0);
        case Type::BytesType:
            return getBytes(p,0,0);
        default:
            return 0;
        }
    }

    unsigned int DataObjectImpl::getLength()
    {
        switch (getType().getTypeEnum()) {
        case Type::BooleanType:
            return BOOL_SIZE;
        case Type::CharacterType:
        case Type::ByteType:
            return BYTE_SIZE;
        case Type::ShortType:
        case Type::IntegerType:
        case Type::LongType:
            return MAX_LONG_SIZE;
        case Type::FloatType:
            return MAX_FLOAT_SIZE;
        case Type::DoubleType:
            return MAX_DOUBLE_SIZE;
        case Type::BigDecimalType:
        case Type::BigIntegerType:
        case Type::UriType:
        case Type::StringType:
            return getString(0,0);
        case Type::BytesType:
            return getBytes(0,0);
        default:
            return 0;
        }
    }

    unsigned int DataObjectImpl::getLength(const char* path)
    {
        DataObjectImpl* d;
        char * spath = DataObjectImpl::stripPath(path);
          char * prop = findPropertyContainer(spath,&d);
        if (spath)     delete spath;
        if (d != 0) {
            if (prop == 0 || (strlen(prop) == 0)) {
                return 0;
            }
            else 
            {
                const Property& p  = d->getProperty(prop);
                delete prop;
                return getLength(p);
            }
        }
        else 
        {
            if (prop)
            {
                const Property& p  = getProperty(prop);
                delete prop;
                
                return getLength(p);
            }
            else 
            {
                return 0;
            }
        }
    }

    unsigned int DataObjectImpl::getLength(unsigned int index)
    {
        return getLength(getProperty(index));
    }

    bool DataObjectImpl::getBoolean(const char* path)
    {
       return getBoolean(SDOString(path));
    }

   // getPrimitiveFromPath(Boolean,bool,false); // Provides DataObjectImpl::getBoolean(const char*)
    getPrimitiveFromPathUsingSDOString(Boolean,bool,false); // Provides DataObjectImpl::getBoolean(const SDOString&)
    getPrimitiveFromPath(Byte,char,0);
    getPrimitiveFromPath(Character,wchar_t,0);
    getPrimitiveFromPath(Short,short,0);
    getPrimitiveFromPath(Integer,long,0);
    getPrimitiveFromPath(Long,int64_t,0L);
    getPrimitiveFromPath(Double,long double,0.0);
    getPrimitiveFromPath(Float,float,0.0);
    getPrimitiveFromPath(Date,const SDODate,0);


    setPrimitiveFromPath(Boolean,bool, bool);
    setPrimitiveFromPath(Byte,char, char);
    setPrimitiveFromPath(Character,wchar_t, wchar_t);
    setPrimitiveFromPath(Short,short, short);
#if __WORDSIZE ==64
    setPrimitiveFromPath(Integer,long, int64_t);
#else
    setPrimitiveFromPath(Integer,long, long);
#endif
    setPrimitiveFromPath(Long,int64_t, int64_t);
    setPrimitiveFromPath(Float,float, float);
    setPrimitiveFromPath(Double,long double, long double);
    setPrimitiveFromPath(Date,const SDODate, const SDODate);


    getPrimitiveFromProperty(Boolean,bool);
    getPrimitiveFromProperty(Byte,char);
    getPrimitiveFromProperty(Character,wchar_t);
    getPrimitiveFromProperty(Short,short);
    getPrimitiveFromProperty(Integer,long);
    getPrimitiveFromProperty(Long,int64_t);
    getPrimitiveFromProperty(Double,long double);
    getPrimitiveFromProperty(Float,float);
    getPrimitiveFromProperty(Date,const SDODate);

    setPrimitiveFromProperty(Boolean,bool);
    setPrimitiveFromProperty(Byte,char);
    setPrimitiveFromProperty(Character,wchar_t);
    setPrimitiveFromProperty(Short,short);
    setPrimitiveFromProperty(Integer,long);
    setPrimitiveFromProperty(Long,int64_t);
    setPrimitiveFromProperty(Float,float);
    setPrimitiveFromProperty(Double,long double);
    setPrimitiveFromProperty(Date,const SDODate);

    getPrimitive(Boolean,bool,false);
    getPrimitive(Byte,char,0);
    getPrimitive(Character,wchar_t,0);
    getPrimitive(Short,short,0);
    getPrimitive(Integer,long,0);
    getPrimitive(Long,int64_t,0L);
    getPrimitive(Double,long double,0.0);
    getPrimitive(Float,float,0.0);
    getPrimitive(Date,const SDODate,0);

    setPrimitive(Boolean,bool,"Boolean");
    setPrimitive(Byte,char, "Byte");
    setPrimitive(Character,wchar_t,"Character");
    setPrimitive(Short,short,"Short");
    setPrimitive(Integer,long,"Integer");
    setPrimitive(Long,int64_t,"Long");
    setPrimitive(Float,float,"Float");
    setPrimitive(Double,long double,"Double");
    setPrimitive(Date,const SDODate,"Date");



    // open type support

    const PropertyImpl* DataObjectImpl::defineProperty(const char* propname, 
                 const Type& t)
    {
        openProperties.insert(
            openProperties.end(), PropertyImpl(getType(),propname,
            (TypeImpl&)t, false, false, true));
        DataFactory* df = factory;
        ((DataFactoryImpl*)df)->addOpenProperty(
                PropertyImpl(getType(),propname,
                (TypeImpl&)t, false, false, true));

        return getPropertyImpl(propname);
    }
   const PropertyImpl* DataObjectImpl::defineProperty(const SDOString& propname, 
                                                      const Type& t)
   {
      openProperties.insert(openProperties.end(),
                            PropertyImpl(getType(),
                                         propname,
                                         (TypeImpl&)t,
                                         false,
                                         false,
                                         true));
      DataFactory* df = factory;
      ((DataFactoryImpl*)df)->addOpenProperty(PropertyImpl(getType(),
                                                           propname,
                                                           (TypeImpl&)t,
                                                           false,
                                                           false,
                                                           true));

      return getPropertyImpl(propname);
   }

    void DataObjectImpl::undefineProperty(unsigned int index)
    {
        int point = index - openBase;
        if (point < 0 || point >= openProperties.size()) return;

        // downgrade all the property settings above this one

        PropertyValueMap::iterator pit;
        for (pit = PropertyValues.begin(); pit != PropertyValues.end();++pit)
        {
            if ((*pit).first > index)
            {
                if (getPropertyImpl((*pit).first)->isMany())
                {
                    DataObjectListImpl* dl = (*pit).second->getListImpl();
                    if (dl != 0) dl->decrementPindex();
                }
                (*pit).first-=1;
             }
        }

        // then remove this property from the list 

        std::list<PropertyImpl>::iterator it = 
            openProperties.begin();
        for (int i=0;i<point;i++)++it; /* there must be a better way */

        DataFactory* df = factory;
        ((DataFactoryImpl*)df)->removeOpenProperty((*it).getName());
        
        openProperties.erase(it);
        
        return;
    }

    const PropertyImpl* DataObjectImpl::defineList(const char* propname)
    {
        const Type& t = factory->getType(Type::SDOTypeNamespaceURI, "OpenDataObject");
        openProperties.insert(
            openProperties.end(), PropertyImpl(getType(),propname,
            (TypeImpl&)t, true, false, true));

        DataFactory* df = factory;
        ((DataFactoryImpl*)df)->addOpenProperty(PropertyImpl(getType(),propname,
            (TypeImpl&)t, true, false, true));

        return getPropertyImpl(propname);
    }

    const PropertyImpl* DataObjectImpl::defineBoolean(const char* propname)
    {
        const Type& t = factory->getType(Type::SDOTypeNamespaceURI, "Boolean");
        return defineProperty(propname,t);
    }

    const PropertyImpl* DataObjectImpl::defineByte(const char* propname)
    {
        const Type& t = factory->getType(Type::SDOTypeNamespaceURI, "Byte");
        return defineProperty(propname,t);
    }
    const PropertyImpl* DataObjectImpl::defineCharacter(const char* propname)
    {
        const Type& t = factory->getType(Type::SDOTypeNamespaceURI, "Character");
        return defineProperty(propname,t);
    }
    const PropertyImpl* DataObjectImpl::defineString(const char* propname)
    {
        const Type& t = factory->getType(Type::SDOTypeNamespaceURI, "String");
        return defineProperty(propname,t);
    }
    const PropertyImpl* DataObjectImpl::defineBytes(const char* propname)
    {
        const Type& t = factory->getType(Type::SDOTypeNamespaceURI, "Bytes");
        return defineProperty(propname,t);
    }
    const PropertyImpl* DataObjectImpl::defineShort(const char* propname)
    {
        const Type& t = factory->getType(Type::SDOTypeNamespaceURI, "Short");
        return defineProperty(propname,t);
    }
    const PropertyImpl* DataObjectImpl::defineInteger(const char* propname)
    {
        const Type& t = factory->getType(Type::SDOTypeNamespaceURI, "Integer");
        return defineProperty(propname,t);
    }
    const PropertyImpl* DataObjectImpl::defineLong(const char* propname)
    {
        const Type& t = factory->getType(Type::SDOTypeNamespaceURI, "Long");
        return defineProperty(propname,t);
    }
    const PropertyImpl* DataObjectImpl::defineFloat(const char* propname)
    {
        const Type& t = factory->getType(Type::SDOTypeNamespaceURI, "Float");
        return defineProperty(propname,t);
    }
    const PropertyImpl* DataObjectImpl::defineDouble(const char* propname)
    {
        const Type& t = factory->getType(Type::SDOTypeNamespaceURI, "Double");
        return defineProperty(propname,t);
    }
    const PropertyImpl* DataObjectImpl::defineDate(const char* propname)
    {
        const Type& t = factory->getType(Type::SDOTypeNamespaceURI, "Date");
        return defineProperty(propname,t);
    }
    const PropertyImpl* DataObjectImpl::defineCString(const char* propname)
    {
        const Type& t = factory->getType(Type::SDOTypeNamespaceURI, "Bytes");
        return defineProperty(propname,t);
    }
    const PropertyImpl* DataObjectImpl::defineDataObject(const char* propname,
        const Type& t)
    {
        return defineProperty(propname,t);
    }
    const PropertyImpl* DataObjectImpl::defineDataObject(const SDOString& propname,
        const Type& t)
    {
        return defineProperty(propname,t);
    }
    const PropertyImpl* DataObjectImpl::defineDataObject(const char* propname,
        const char* typeURI, const char* typeName)
    {
        const Type& t = factory->getType(typeURI, typeName);
        return defineProperty(propname,t);
    }

    // Used to return empty values - remove when defaults are there.
    const char* DataObjectImpl::emptyString = "";

    // Useful for debug, so not included in the macros above - but
    // could be.
    // getters and setters for strings 

    const char* DataObjectImpl::getCString(unsigned int propertyIndex)
    {
        validateIndex(propertyIndex);
        if ((getProperty(propertyIndex).isMany())
            || getPropertyImpl(propertyIndex)->getTypeImpl()->isFromList())
        {

            string msg("Get value not available on many valued property:");
            msg += getProperty(propertyIndex).getName();
            SDO_THROW_EXCEPTION("getCString", SDOUnsupportedOperationException,
                msg.c_str());
        }
          DataObjectImpl* d = getDataObjectImpl(propertyIndex);
        if (d != 0) {
            if (!d->isNull()) return d->getCString ();
            return 0;
        }
        PropertyImpl* p = (PropertyImpl*)getPropertyImpl(propertyIndex);
        if (p != 0) 
        {
            if (isSet(propertyIndex)) return 0;
            //if (p->isDefaulted())
            //{
                return p->getCStringDefault();
            //}
            //else
            //{
            //    handlePropertyNotSet(p->getName());
            //}
        }
        return 0;
    }


    void DataObjectImpl::setCString (unsigned int propertyIndex, const char* value)
    {
        validateIndex(propertyIndex);
        PropertyValueMap::iterator i;
        if ((getProperty(propertyIndex).isMany())
            || getPropertyImpl(propertyIndex)->getTypeImpl()->isFromList())
        {
            string msg("Set value not available on many valued property:");
            msg += getProperty(propertyIndex).getName();
            SDO_THROW_EXCEPTION("setString", SDOUnsupportedOperationException,
                msg.c_str());
        }
        for (i = PropertyValues.begin(); i != PropertyValues.end();++i)
            {
            if ((*i).first == propertyIndex)
            {
                logChange(propertyIndex);
                (*i).second->unsetNull();
                (*i).second->setCString(value);
                return;
            }
        }
        DataFactory* df = getDataFactory();
        DataObjectImpl* b = new DataObjectImpl(df, df->getType(Type::SDOTypeNamespaceURI,"String"));
        b->setContainer(this);
        b->setApplicableChangeSummary();
        logChange(propertyIndex);
        PropertyValues.insert(PropertyValues.end(),rdo(propertyIndex,b));
        b->setCString(value);
          return;
    }


    const char* DataObjectImpl::getCString (const char* path)
    {
        DataObjectImpl* d = 0;
        char* spath = 0;
        char *prop = 0;
        try {
            spath = DataObjectImpl::stripPath(path);
              prop = findPropertyContainer(spath,&d);
            if (spath) {
                delete spath;
                spath = 0;
            }
            if (d != 0) {
                if (prop == 0 || (strlen(prop) == 0)) {
                    return d->getCString();
                }
                else {
                    PropertyImpl* p  = d->getPropertyImpl(prop);
                    if (p != 0) 
                    {
                        if (p->isMany()|| p->getTypeImpl()->isFromList())
                        {
                            long l;
                            DataObjectImpl* doi = d->findDataObject(prop,&l);
                            delete prop;
                            prop = 0;
                            if (doi != 0)    {
                                return doi->getCString();
                            }
                            string msg("Get CString - index out of range");
                            msg += path;
                            SDO_THROW_EXCEPTION("getter", SDOIndexOutOfRangeException,
                            msg.c_str());
                        }
                        else {
                            delete prop;
                            prop = 0;
                            if (!d->isSet(*p)) {
                                //if (p->isDefaulted())
                                //{
                                    return p->getCStringDefault();
                                //}
                                //else
                                //{
                                //    handlePropertyNotSet(p->getName());
                                //}
                            }
                            return d->getCString(*p);
                        }
                    }
                }
            }
            if (prop){
                delete prop;
                prop = 0;
            }
            string msg("Get CString  - object not found");
            SDO_THROW_EXCEPTION("getCString", SDOPathNotFoundException,
            msg.c_str());
        }
        catch (SDORuntimeException e) {
            if (spath) delete spath;
            if (prop) delete prop;
            SDO_RETHROW_EXCEPTION("getCString",e);
        }
    }
    


    void DataObjectImpl::setCString(const char* path, const char* value)
    {
        DataObjectImpl *d = 0;
        char* spath = 0;
        char* prop = 0;
        try {
            spath = DataObjectImpl::stripPath(path);
             prop = findPropertyContainer(spath,&d);
            if (spath) {
                delete spath;
                spath = 0;
            }
            if (d != 0) {
                if (prop == 0 || (strlen(prop) == 0)) {
                    d->setCString(value);
                }
                else { 
                    const PropertyImpl* p = d->getPropertyImpl(prop);
                    if (p == 0 && d->getType().isOpenType())
                    {
                        p = d->defineBytes(prop);
                    }
                    if (p != 0)
                    {
                        if (p->isMany()|| p->getTypeImpl()->isFromList()) {
                            long l;
                            DataObjectList& dol = d->getList((Property&)*p);
                            DataObjectImpl* doi = d->findDataObject(prop,&l);
                            if (doi != 0)
                            {
                                doi->setCString(value);
                            }
                            else 
                            {
                                dol.append(value);
                            }
                        }
                        else {
                            d->setCString((Property&)*p,value);
                        }
                    }
                    delete prop;
                    prop = 0;
                }
            }
            if (prop) {
                delete prop;
                prop = 0;
            }
        }
        catch (SDORuntimeException e) {
            if (spath) delete spath;
            if (prop) delete prop;
            SDO_RETHROW_EXCEPTION("setCString",e);
        }
    }



    const char* DataObjectImpl::getCString (const Property& property)
    {
        return getCString(getPropertyIndex(property));
    }


    void DataObjectImpl::setCString(const Property& property, const char* value)
    {
        setCString(getPropertyIndex(property),value);
    }

    // null support

    bool DataObjectImpl::isNull(const unsigned int propertyIndex)
    {
        validateIndex(propertyIndex);
        if ((getProperty(propertyIndex).isMany()))
        {
            return false;
        }

        PropertyValueMap::iterator i;
        for (i = PropertyValues.begin(); i != PropertyValues.end();++i)
            {
            if ((*i).first == propertyIndex)
            {
                return (*i).second->isNull();
            }
        }
        return false;
    }

    bool DataObjectImpl::isNull(const Property& property)
    {
        return isNull(getPropertyIndex(property));
    }

    bool DataObjectImpl::isNull(const char* path)
    {
        DataObjectImpl *d = 0;
        char* spath = 0;
        char* prop = 0;
        try {
            spath = DataObjectImpl::stripPath(path);
             prop = findPropertyContainer(spath,&d);
            if (spath) {
                delete spath;
                spath = 0;
            }
            if (d != 0) {
                if (prop == 0 || (strlen(prop) == 0)) {
                    return d->isNull();
                }
                else {
                    const Property& p = d->getProperty(prop);
                    delete prop;
                    return d->isNull(p);
                }
            }
            if (prop) {
                delete prop;
                prop = 0;
            }
            return false;
        }
        catch (SDORuntimeException e) {
            if (spath) delete spath;
            if (prop) delete prop;
            SDO_RETHROW_EXCEPTION("isNull",e);
        }

    }
    void DataObjectImpl::setNull(const unsigned int propertyIndex)
    {
        validateIndex(propertyIndex);
        if ((getProperty(propertyIndex).isMany()))
        {
            string msg("Setting a list to null is not supported:");
            msg += getProperty(propertyIndex).getName();
            SDO_THROW_EXCEPTION("setNull", SDOUnsupportedOperationException,
                msg.c_str());
        }

        PropertyValueMap::iterator i;
        for (i = PropertyValues.begin(); i != PropertyValues.end();++i)
            {
            if ((*i).first == propertyIndex)
            {
                logChange(propertyIndex);
                (*i).second->setNull();
                return;
            }
        }
        // The property was not set yet...
        logChange(propertyIndex);
        DataFactory* df = getDataFactory();
        DataObjectImpl* b = new DataObjectImpl(df, 
            getProperty(propertyIndex).getType());
        b->setContainer(this);
        b->setApplicableChangeSummary();
        PropertyValues.insert(PropertyValues.end(),rdo(propertyIndex,b));
        b->setNull();


    }
    void DataObjectImpl::setNull(const Property& property)
    {
        setNull(getPropertyIndex(property));
    }

    void DataObjectImpl::setNull(const char* path)
    {
        DataObjectImpl *d = 0;
        char* spath = 0;
        char* prop = 0, *pc = 0;
        try {
            spath = DataObjectImpl::stripPath(path);
             prop = findPropertyContainer(spath,&d);
            if (spath) {
                delete spath;
                spath = 0;
            }
            if (d != 0) {
                if (prop == 0 || (strlen(prop) == 0)) {
                    try {
                        DataObjectImpl* cont = d->getContainerImpl();
                        if (cont != 0)
                        {
                            pc = strrchr(path,'/');
                            if (pc != 0)pc++;
                        }
                        const Property& pcont = cont->getProperty(pc);
                        cont->logChange(pcont);
                    }
                    catch (SDORuntimeException&)
                    {
                    }
                    d->setNull();
                }
                else {
                    const PropertyImpl* p = d->getPropertyImpl(prop);
                    if (p == 0)
                    {
                        if(d->getType().isOpenType())
                        {
                            string msg("Cannot set unassigned open property to null:");
                            msg += prop;
                            SDO_THROW_EXCEPTION("setNull", SDOUnsupportedOperationException,
                            msg.c_str());
                        }
                        else
                        {
                            string msg("Property Not Found:");
                            msg += prop;
                            SDO_THROW_EXCEPTION("setNull", SDOPropertyNotFoundException,
                            msg.c_str());
                        }
                    }
                    delete prop;
                    d->setNull((Property&)*p);
                    return;
                }
            }
            if (prop) {
                delete prop;
                prop = 0;
            }
            return;
        }
        catch (SDORuntimeException e) {
            if (spath) delete spath;
            if (prop) delete prop;
            SDO_RETHROW_EXCEPTION("setNull",e);
        }

    }



    // getters and setters for a List data object 

    DataObjectList& DataObjectImpl::getList(const char* path)
    {
        DataObjectImpl *d;
        char* spath = DataObjectImpl::stripPath(path);
         char* prop = findPropertyContainer(spath,&d);
        if (spath) delete spath;  
        if (d != 0) {
            if (prop == 0 || (strlen(prop) == 0)) {
                return d->getList();
            }
            else {
                const PropertyImpl* p = d->getPropertyImpl(prop);
                if (p == 0 && d->getType().isOpenType())
                {
                    p = d->defineList(prop);
                }
                if (p != 0)
                {
                    delete prop;
                    return d->getList((Property&)*p);
                }
            }
        }
        if (prop) delete prop;

        string msg("Invalid path:");
        msg += path;
        SDO_THROW_EXCEPTION("getList",SDOPathNotFoundException, msg.c_str());

    }



    DataObjectList& DataObjectImpl::getList(unsigned int propIndex)
    {
        if (!(getProperty(propIndex).isMany()))
        {
            if (!getPropertyImpl(propIndex)->getTypeImpl()->isFromList())
            {
                string msg("Get list not available on single valued property:");
                msg += getProperty(propIndex).getName();
                SDO_THROW_EXCEPTION("getList", SDOUnsupportedOperationException,
                    msg.c_str());
            }
        }
        DataObjectImpl* d = getDataObjectImpl(propIndex);
        if (d == 0)
        {
            string msg("Property Not Found:");
            msg += getType().getProperty(propIndex).getName();
            SDO_THROW_EXCEPTION("getList", SDOPropertyNotFoundException,
            msg.c_str());
        }
        return d->getList();
    }

    DataObjectList& DataObjectImpl::getList(const Property& p)
    {
        if (!p.isMany())
        {
            PropertyImpl* pi = (PropertyImpl*)&p;
            if (!pi->getTypeImpl()->isFromList())
            {
                string msg("Get list not available on single valued property:");
                msg += p.getName();
                SDO_THROW_EXCEPTION("getList", SDOUnsupportedOperationException,
                msg.c_str());
            }
        }

        int propIndex = getPropertyIndex(p);
        DataObjectImpl* d = getDataObjectImpl(propIndex);
        if (d == 0) {
            // There is no list yet, so we need to create an 
            // empty data object to hold the list
            DataFactory* df = getDataFactory();
            d = new DataObjectImpl(df, df->getType(Type::SDOTypeNamespaceURI,"DataObject"));
            PropertyValues.insert(PropertyValues.end(),rdo(propIndex,d));
            d->setContainer(this);
            d->setApplicableChangeSummary();
            
            DataObjectListImpl* list = new DataObjectListImpl(df,this,
                propIndex,p.getType().getURI(),p.getType().getName());
            d->setList(list); 

        }
        return d->getList();
    }    



    DataObjectList& DataObjectImpl::getList()
    {
        if (getTypeImpl().isFromList())
        {
            return getList("values");
        }
        return *listValue;
    }

    DataObjectListImpl* DataObjectImpl::getListImpl()
    {
        if (getTypeImpl().isFromList())
        {
            DataObjectList& dl = getList("values");
            return (DataObjectListImpl*)&dl;
        }
        return listValue;
    }



  /////////////////////////////////////////////////////////////////////////////
  // Utilities 
  /////////////////////////////////////////////////////////////////////////////
  

    // get an index, or throw if the prop is not part of this DO 

    unsigned int DataObjectImpl::getPropertyIndex(const Property& p)
    {
        PropertyList props = getType().getProperties(); 

        for (int i = 0; i < props.size() ; ++i)
        {
            if (!strcmp(props[i].getName(),p.getName()) )
            {
                return i;
            }
        }
        if (getType().isOpenType())
        {
            std::list<PropertyImpl>::iterator j;
            int count = 0;
            for (j = openProperties.begin() ; 
                 j != openProperties.end() ; ++j)
             {
                if (!strcmp((*j).getName(),p.getName()))
                {
                    return count+openBase;
                }
                count++;
            }
        }
        string msg("Cannot find property:");
        msg += p.getName();
        SDO_THROW_EXCEPTION("getPropertyIndex", SDOPropertyNotFoundException,
            msg.c_str());
    }

    const Property& DataObjectImpl::getProperty(unsigned int index)
    {
        PropertyImpl* pi = getPropertyImpl(index);
        if (pi == 0)
        {
            string msg("Index out of range");
            SDO_THROW_EXCEPTION("getProperty", SDOIndexOutOfRangeException,
            msg.c_str());
        }
        return (Property&)*pi;
    }

    /**
     * See if the property currently exists
     */

    bool DataObjectImpl::hasProperty(const char* name)
    {
        PropertyImpl* pi = getPropertyImpl(name);
        if (pi == 0) return false;
        return true;
    }

    bool DataObjectImpl::hasProperty(const SDOString& name)
    {
        PropertyImpl* pi = getPropertyImpl(name);
        if (pi == 0) return false;
        return true;
    }


    PropertyImpl* DataObjectImpl::getPropertyImpl(unsigned int index)
    {
        PropertyList props = getType().getProperties();  
        if (index < props.size())
        {
            return (PropertyImpl*)&props[index];
        }

        if (getType().isOpenType())
        {
            if (index >= openBase && index - openBase  < openProperties.size())
            {
                std::list<PropertyImpl>::iterator j;
                int val = 0;
                j = openProperties.begin();
                while (val < index-openBase && j != openProperties.end())
                {
                    val++;
                    j++;
                }
                if (j != openProperties.end()) return &(*j);
            }
        }
        return 0;
    }


    //////////////////////////////////////////////////////////////////////
    // TODO - this is rubbish, but gets us by until XPATH is done
    // trip the path down to characters which I am going to
    // recognise later (a-z, A-Z _ [ ] .)
    //////////////////////////////////////////////////////////////////////

    const char* DataObjectImpl::templateString = 
    " /abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890=[]._#";

    char* DataObjectImpl::stripPath(const char* path)
    {
        int pos = 0;
        char* s = 0;
        if (path == 0) return s;

        s = new char[strlen(path)+1];

        for (int i=0;i < strlen(path); i++) 
        {
            if (strchr(templateString,path[i]) != 0) {
                s[pos++] = path[i];
            }
        }
        s[pos++] = 0;
        return s;
    }

    void DataObjectImpl::stripPath(const SDOString& path, SDOString& result)
    {
       result.erase();
       result.reserve(path.length());
       
       size_t start = 0;
       size_t position = path.find_first_not_of(templateString);

       while (position != string::npos)
       {
          result.append(path, start, (position - start));
          start = ++position;
          position = path.find_first_not_of(templateString, position);
       }

       result.append(path, start, string::npos);

       return;
    }

    //////////////////////////////////////////////////////////////////////
    // Find a data object or return 0 if not found
    //////////////////////////////////////////////////////////////////////

    DataObjectImpl* DataObjectImpl::findDataObject(char* token, long* index)
    {
        // name , name[int], name[x=y] name.int 
        char c = 0;
           char* beginbrace = strchr(token,'[');
        char* endbrace   = strchr(token,']');
        char* dot = strchr(token,'.');
        char* breaker = 0;


        if (dot != 0)
        {
            if (beginbrace != 0) 
            {
                breaker = (beginbrace < dot)? beginbrace:dot;
            }
            else 
            {
                breaker = dot;
            }
        }
        else 
        {
            breaker = beginbrace;
        }

        if (breaker == 0){
            // its this object, and a property thereof 
            *index = -1;
            const Property& p = getProperty(token);
            return getDataObjectImpl(p);
        
        }

        c = *breaker;
        *breaker = 0;
        const Property& p = getProperty(token);
        *breaker = c;

    
        breaker++;

        if (endbrace != 0)
        {
            *endbrace = 0;
        }

        char* eq = strchr(breaker,'=');

        if (eq == 0)
        {
            int val = atoi(breaker);
            DataObjectList& list = getList(p);

            // The spec says that depts[1] is the first element,
            //   as is depts.0 
            
              if (beginbrace != 0)val--;

            if (endbrace != 0)*endbrace = ']';

            if (val >=0 && val < list.size())
            {
                DataObject* dob =  list[val];
                *index = val;
                return (DataObjectImpl*)dob;
            }
            *index = -1;
            return 0;
        }
         *eq = 0;
        // breaker is now propname
        eq++;
        // eq is now propval

        DataObjectList& list = getList(p);
        for (int li = 0 ; li < list.size() ; ++li)
        {
            // TODO  comparison for double not ok

            const Type & t = list[li]->getType();
            const Property& p  = list[li]->getProperty(breaker);
            int ok = 0;

            switch (p.getTypeEnum())
            {
            case  Type::BooleanType:
                {
                    // getCString will return "true" or "false"
                    if (!strcmp(eq,list[li]->getCString(p))) ok = 1;
                }
                break;

            case  Type::ByteType:
                {
                    char cc =  (char)eq[0];
                    // getByte return a char
                    if (cc == list[li]->getByte(p)) ok = 1;
                }
                break;

            case  Type::CharacterType:
                {
                    wchar_t wc =  (wchar_t)((wchar_t*)eq)[0];
                    // TODO - this is not a very accesible way of storing a wchar
                    if (wc == list[li]->getCharacter(p)) ok = 1;
                }
                break;

            case  Type::IntegerType:
                {
                    long  ic =  atol(eq);
                    if (ic == list[li]->getInteger(p)) ok = 1;
                }
                break;

            case  Type::DateType: 
                {
                    long  dc =  atol(eq);
                    if (dc == (long)(list[li]->getDate(p).getTime())) ok = 1;
                }
                break;
                
            case  Type::DoubleType:
                {
                    // TODO - double needs a bigger size than float
                    long double  ldc =  (long double)atof(eq);
                    if (ldc == list[li]->getDouble(p)) ok = 1;
                }
                break;

            case  Type::FloatType:
                {
                    float  fc =  atof(eq);
                    if (fc == list[li]->getFloat(p)) ok = 1;
                }
                break;

            case  Type::LongType:
                {
#if defined(WIN32)  || defined (_WINDOWS)
                                        int64_t lic = (int64_t)_atoi64(eq);
#else
                                        int64_t lic = (int64_t)strtoll(eq, NULL, 0);
#endif

                    if (lic == list[li]->getLong(p)) ok = 1;
                }
                break;

            case  Type::ShortType:
                {
                    short  sic =  atoi(eq);
                    if (sic == list[li]->getShort(p)) ok = 1;
                }
                break;


            case  Type::BytesType:
            case  Type::BigDecimalType:
            case  Type::BigIntegerType:
            case  Type::StringType:
            case  Type::UriType:
                {

                    if (!strcmp(eq, list[li]->getCString(p))) ok = 1;
                    // try with quotes too
                    char *firstquote = strchr(eq,'"');
                    char *firstsingle = strchr(eq,'\'');
                    char searchchar = 0;

                    if (firstsingle == 0)
                    {
                        if (firstquote != 0)
                        {
                            searchchar = '"';
                        }
                    }
                    else
                    {
                        if (firstquote != 0 && firstquote < firstsingle)
                        {
                            searchchar = '"';
                        }
                        else
                        {
                            searchchar = '\'';
                            firstquote = firstsingle;
                        }
                    }

                    if (searchchar != 0)
                    {
                        char* ender = strchr(firstquote+1,searchchar);
                        if (ender != 0)
                        {
                            *ender = 0;
                            if (!strcmp(firstquote+1, list[li]->getCString(p))) ok = 1;
                            *ender = searchchar;
                        }
                    }
                }
                break;

            case Type::DataObjectType:
                break;

            default:
                break;
            }

            if (ok == 1)
            {
                *--eq='=';
                if (endbrace != 0)*endbrace = ']';
                DataObject* dob = list[li];
                *index = li;
                return (DataObjectImpl*)dob;
            }
        }
        if (endbrace != 0)*endbrace = ']';
        *--eq='=';
        return 0;
    }
DataObjectImpl* DataObjectImpl::findDataObject(const SDOString& token, long* index)
{
   // name , name[int], name[x=y] name.int 
   // char c = 0;
   size_t beginbrace = token.find('[');
   size_t dot = token.find('.');
   size_t breaker = 0;

//    char* beginbrace = strchr(token,'[');
//    char* endbrace   = strchr(token,']');
//    char* dot = strchr(token,'.');
//    char* breaker = 0;

   if (dot != string::npos)
   {
      if (beginbrace != string::npos)
      {
         breaker = (beginbrace < dot) ? beginbrace : dot;
      }
      else 
      {
         breaker = dot;
      }
   }
   else 
   {
      breaker = beginbrace;
   }

   if (breaker == string::npos){
      // its this object, and a property thereof
      *index = -1;
      const Property& p = getProperty(token);
      return getDataObjectImpl(p);
   }

   // We did find a breaker character.
   const Property& p = getProperty(SDOString(token, 0, breaker));

   breaker++;

   size_t endbrace = token.find(']');
   SDOString breakerStr = token.substr(breaker, (endbrace - breaker));
   // Search for the first occurence of an = sign starting at the previously
   // identified "breaker" character and ending at the endbrace just found. We
   // need to make a new SDOString to contain that substring.

   size_t eq = breakerStr.find('=');

   if (eq == string::npos)
   {
      // There is no "=" sign
      int val = atoi(breakerStr.c_str());
      DataObjectList& list = getList(p);

      // The spec says that depts[1] is the first element, as is depts.0
      if (beginbrace != string::npos) val--;

      if (val >=0 && val < list.size())
      {
         DataObject* dob = list[val];
         *index = val;
         return (DataObjectImpl*)dob;
      }
      *index = -1;
      return 0;
   }

   // We did find an "=" sign.
   // *eq = 0;
   SDOString PropertyName = token.substr(breaker, (eq - breaker));
   // breaker is now propname
   eq++;
   SDOString PropertyValue = token.substr(eq, (endbrace - eq));
   // eq is now propval

   DataObjectList& list = getList(p);
   for (int li = 0 ; li < list.size() ; ++li)
   {
      // TODO  comparison for double not ok

      const Type & t = list[li]->getType();
      const Property& p  = list[li]->getProperty(PropertyName);
      int ok = 0;

      switch (p.getTypeEnum())
      {
         case Type::BooleanType:
         {
            // getCString will return "true" or "false"
            if (!strcmp(PropertyValue.c_str(), list[li]->getCString(p))) ok = 1;
         }
         break;

         case  Type::ByteType:
         {
            char cc = PropertyValue[0];
            // getByte return a char
            if (cc == list[li]->getByte(p)) ok = 1;
         }
         break;

         case  Type::CharacterType:
         {
            wchar_t wc = *((wchar_t*) PropertyValue.c_str());
            // wchar_t wc =  (wchar_t)((wchar_t*)eq)[0];
            // TODO - this is not a very accesible way of storing a wchar
            if (wc == list[li]->getCharacter(p)) ok = 1;
         }
         break;

         case  Type::IntegerType:
         {
            long  ic =  atol(PropertyValue.c_str());
            if (ic == list[li]->getInteger(p)) ok = 1;
         }
         break;

         case  Type::DateType: 
         {
            long  dc =  atol(PropertyValue.c_str());
            if (dc == (long)(list[li]->getDate(p).getTime())) ok = 1;
         }
         break;
                
         case  Type::DoubleType:
         {
            // TODO - double needs a bigger size than float
            long double  ldc =  (long double)atof(PropertyValue.c_str());
            if (ldc == list[li]->getDouble(p)) ok = 1;
         }
         break;

         case  Type::FloatType:
         {
            float  fc =  atof(PropertyValue.c_str());
            if (fc == list[li]->getFloat(p)) ok = 1;
         }
         break;

         case  Type::LongType:
         {
#if defined(WIN32)  || defined (_WINDOWS)
            int64_t lic = (int64_t)_atoi64(PropertyValue.c_str());
#else
            int64_t lic = (int64_t)strtoll(PropertyValue.c_str(), NULL, 0);
#endif

            if (lic == list[li]->getLong(p)) ok = 1;
         }
         break;

         case  Type::ShortType:
         {
            short sic = atoi(PropertyValue.c_str());
            if (sic == list[li]->getShort(p)) ok = 1;
         }
         break;

         case  Type::BytesType:
         case  Type::BigDecimalType:
         case  Type::BigIntegerType:
         case  Type::StringType:
         case  Type::UriType:
         {

            if (!strcmp(PropertyValue.c_str(), list[li]->getCString(p))) ok = 1;
            // try with quotes too
            size_t firstquote = PropertyValue.find('"');
            size_t firstsingle = PropertyValue.find('\'');

            char searchchar = 0;

            if (firstsingle == string::npos)
            {
               if (firstquote != string::npos)
               {
                  searchchar = '"';
               }
            }
            else
            {
               if (firstquote != string::npos && firstquote < firstsingle)
               {
                  searchchar = '"';
               }
               else
               {
                  searchchar = '\'';
                  firstquote = firstsingle;
               }
            }

            if (searchchar != 0)
            {
               size_t ender = PropertyValue.find(searchchar, firstquote + 1);
               if (ender != string::npos)
               {
                  if (!strcmp(PropertyValue.substr(firstquote + 1, ender - firstquote).c_str(), list[li]->getCString(p)))
                     ok = 1;
               }
            }
         }
         break;

         case Type::DataObjectType:
            break;

         default:
            break;
      }

      if (ok == 1)
      {
         DataObject* dob = list[li];
         *index = li;
         return (DataObjectImpl*)dob;
      }
   }
   return 0;
}


    //////////////////////////////////////////////////////////////////////
    // Find a data object and a property name within it.
    //////////////////////////////////////////////////////////////////////

    char* DataObjectImpl::findPropertyContainer(const char* path, DataObjectImpl** din)
    {

        // initially check for "#/" which indicates that we need to find the root
        // object first 
        if (path == 0) return 0;

        if (strlen(path) <= 2)
        {
            if (path[0] == '#')
            {
                DataObjectImpl* root = this;
                while (root->getContainerImpl() != 0)
                {
                    root = root->getContainerImpl();
                }
                *din = root;
                return 0;
            }
        }
        
        if (path[0] == '#' && path[1] == '/')
        {
            DataObjectImpl* root = this;
            while (root->getContainerImpl() != 0)
            {
                root = root->getContainerImpl();
            }
            return root->findPropertyContainer(&path[2],din);
        }

        DataObjectImpl* d;
        char*  i = strchr(path,'/');
        char* remaining = 0;
        char* token     = 0;

        if (i != 0) 
        {
            int j = strlen(path) - strlen(i);
            if (j > 0) 
            {
                token = new char[j + 1];
                strncpy(token,path, j);
                token[j] = 0;
            }
            if (strlen(i) > 1) 
            {
                remaining = new char[strlen(i)];
                strcpy(remaining, i+1);
            }
        }
        else 
        {
            remaining = new char[strlen(path) + 1];
            strcpy(remaining,path);
        }
        
        if (token == 0) 
        {
            if (remaining != 0 && !strcmp(remaining,"..")) 
            {
                /* Its the container itself */
                *din = getContainerImpl();
                delete remaining;
                return 0;
            }

            /* Its this data object - property could be empty or
               valid or invalid - user must check */
            *din = this;
            return remaining;
        }

        if (!strcmp(token,"..")) {
            /* Its derived from the container */
            d = getContainerImpl();
            /* carry on trying to find a property */
            if (d != 0) {
                char* ret = d->findPropertyContainer(remaining, din);
                delete token;
                if (remaining) delete remaining;
                return ret;
            }
            /* Give up - no container */
            delete token;
            if (remaining) delete remaining;
            *din = 0;
            return 0;
        }


        /* Try to find a property ....*/
        long l;
        d = findDataObject(token,&l);
        if (d != 0) {
            char* ret = d->findPropertyContainer(remaining,din);
            delete token;
            if (remaining) delete remaining;
            return ret;
        }

        /* Give up its not in the tree */

        delete token;
        if (remaining) delete remaining;
        *din = 0;
        return 0;
    }
  SDOString DataObjectImpl::findPropertyContainer(const SDOString& path, DataObjectImpl** din)
  {
    // initially check for "#/" which indicates that we need to find the root object first 
    if (path.empty()) return 0;

    if (path.length() <= 2)
    {
      if (path[0] == '#')
      {
        DataObjectImpl* root = this;
        while (root->getContainerImpl() != 0)
        {
          root = root->getContainerImpl();
        }
        *din = root;
        return SDOString();
      }
    }

    if (path[0] == '#' && path[1] == '/')
    {
      DataObjectImpl* root = this;
      while (root->getContainerImpl() != 0)
      {
        root = root->getContainerImpl();
      }
      return root->findPropertyContainer(SDOString(path, 2), din);
    }

    DataObjectImpl* d;
    size_t slashPos = path.find('/');  // i is the subscript of the found character
    SDOString remaining;
    SDOString token;

    if (slashPos != string::npos)      // If we found a slash character
    {
      if (slashPos > 0)              // If there is something before the slash
      {
        token.assign(path, 0, slashPos);
      }
      if ((path.length() - slashPos) > 1) // If there is something after the slash
      {
        remaining.assign(path, slashPos + 1, string::npos);
      }
    }
    else
    {
      remaining = path;
    }

    if (token.empty()) 
    {
      if (remaining == "..") 
      {
        /* Its the container itself */
        *din = getContainerImpl();
        return SDOString();
      }

      /* Its this data object - property could be empty or
         valid or invalid - user must check */
      *din = this;
      return remaining;
    }

    if (token == "..") {
      /* Its derived from the container */
      d = getContainerImpl();
      /* carry on trying to find a property */
      if (d != 0) {
        return d->findPropertyContainer(remaining, din);
      }
      /* Give up - no container */
      *din = 0;
      return 0;
    }

    /* Try to find a property ....*/
    long l;
    d = findDataObject(token, &l);
    if (d != 0) {
      return d->findPropertyContainer(remaining, din);
    }

    /* Give up its not in the tree */
    *din = 0;
    return 0;
  }




   // Returns a read-only List of the Properties currently used in thIs DataObject.
   // ThIs list will contain all of the properties in getType().getProperties()
   // and any properties where isSet(property) is true.
   // For example, properties resulting from the use of
   // open or mixed XML content are present if allowed by the Type.
   // The list does not contain duplicates. 
   // The order of the properties in the list begins with getType().getProperties()
   // and the order of the remaining properties is determined by the implementation.
   // The same list will be returned unless the DataObject is updated so that 
   // the contents of the list change
   // @return the list of Properties currently used in thIs DataObject.
   
    PropertyList /* Property */ DataObjectImpl::getInstanceProperties()
    {
        std::vector<PropertyImpl*> theVec;
        PropertyList propList = getType().getProperties();
        for (int i = 0 ; i < propList.size() ; ++i)
        {
            Property& p = propList[i];
            theVec.insert(theVec.end(),(PropertyImpl*)&p);
        }
        std::list<PropertyImpl>::iterator j;
        for (j = openProperties.begin() ;
             j != openProperties.end() ; ++j)
        {
            theVec.insert(theVec.end(),&(*j));
        }
        return PropertyList(theVec);
    }
  
    void DataObjectImpl::setInstancePropertyType(unsigned int index,
        const Type* t)
    {
        if (index >= openBase && index - openBase  < openProperties.size())
        {
            std::list<PropertyImpl>::iterator j;
            unsigned int count = openBase;
            for (j = openProperties.begin() ;
                 j != openProperties.end() ; ++j)
            {
                if (count == index)
                {
                    openProperties.insert(j,
                        PropertyImpl(getType(),
                        (*j).getName(),
                        (TypeImpl&)*t,
                        (*j).isMany(),
                        (*j).isReadOnly(),
                        (*j).isContainment()));

                    DataFactory* df = factory;
                    ((DataFactoryImpl*)df)->addOpenProperty(
                        PropertyImpl(getType(),
                        (*j).getName(),
                        (TypeImpl&)*t,
                        (*j).isMany(),
                        (*j).isReadOnly(),
                        (*j).isContainment()));

                    openProperties.erase(j);
                    
                    return;
                }
                count++;
            }
        }
        return;
    }
  
   // Returns the Sequence for thIs DataObject.
   // When Type.isSequencedType() == true,
   // the Sequence of a DataObject corresponds to the
   // XML elements representing the values of its properties.
   // Updates through DataObject and the Lists or Sequences returned
   // from DataObject operate on the same data.
   // When Type.isSequencedType() == false, null is returned.  
   // @return the <code>Sequence</code> or null.

    SequenceImpl* DataObjectImpl::getSequenceImpl()
    {

         return sequence;
    }

    SequencePtr DataObjectImpl::getSequence()
    {
        return (SequencePtr)sequence;
    }

    SequencePtr DataObjectImpl::getSequence(const char* path)
    {
        DataObject* d = (DataObject*)getDataObject(path);
         if (d) return d->getSequence();
        return 0;
    }

    SequencePtr DataObjectImpl::getSequence(unsigned int propertyIndex)
    {
        DataObject* d = (DataObject*)getDataObject(propertyIndex);
         if (d) return d->getSequence();
        return 0;
    }

    SequencePtr DataObjectImpl::getSequence(const Property& property)
    {
        DataObject* d = (DataObject*)getDataObject(property);
         if (d) return d->getSequence();
        return 0;
    }

  

    ChangeSummaryPtr DataObjectImpl::getChangeSummary(const char* path)
    {
        DataObjectImpl* d = getDataObjectImpl(path);
        return d->getChangeSummary();
    }

    ChangeSummaryPtr DataObjectImpl::getChangeSummary(unsigned int propIndex)
    {
        DataObjectImpl* d = getDataObjectImpl(propIndex);
        return d->getChangeSummary();
    }

    ChangeSummaryPtr DataObjectImpl::getChangeSummary(const Property& prop)
    {
        DataObjectImpl* d = getDataObjectImpl(prop);
        return d->getChangeSummary();

    }

    ChangeSummaryPtr DataObjectImpl::getChangeSummary()
    {
        if (getType().isChangeSummaryType())
        {
            return (ChangeSummaryPtr)localCS;
        }
        // The changesummaryobject MUST be a change summary type
        // but as an additional check against looping, I will use
        // a redundent getSummary() method.
        // TODO - remove this.
        if (changesummaryobject != 0) return 
            (ChangeSummaryPtr)(changesummaryobject->getSummary());
        return 0;
    }


    ChangeSummaryImpl* DataObjectImpl::getChangeSummaryImpl()
    {
        if (getType().isChangeSummaryType())
        {
            return localCS;
        }
        // The changesummaryobject MUST be a change summary type
        // but as an additional check against looping, I will use
        // a redundent getSummary() method.
        // TODO - remove this.
        if (changesummaryobject != 0) return changesummaryobject->getSummary();
        return 0;
    }

    ChangeSummaryImpl* DataObjectImpl::getSummary()
    {
        return localCS;
    }

    // sets a property of either this object or an object reachable from it, 
    // as identified by the specified path,
    // to the specified value.
    // @param path the path to a valid object and property.
    // @param value the new value for the property.

    void DataObjectImpl::setDataObject(const char* path, DataObjectPtr value)
    {
        DataObjectImpl* d;
        char* prop = findPropertyContainer(path, &d);
        if (d != 0)
        {
            if (prop != 0) {
                const PropertyImpl* p = d->getPropertyImpl(prop);
                if (p == 0 && d->getType().isOpenType())
                {
                    if (value != 0)
                    {
                        p = d->defineDataObject(prop, value->getType());
                    }
                }
                if (p != 0)
                {
                    if (p->isMany())
                    {
                        DataObjectList& dol = d->getList((Property&)*p);
                        long index;
                        DataObjectImpl* dx = d->findDataObject(prop,&index);
                        if (index >= 0)
                        {
                            if(index < dol.size())
                            {
                                dol.setDataObject((unsigned int)index,value);
                            }
                            else 
                            {
                                dol.append(value);
                            }
                            delete prop;
                            return;
                        }
                        string msg("Set of data object on many valued item");
                        msg += path;
                        SDO_THROW_EXCEPTION("setDataObject", SDOUnsupportedOperationException,
                        msg.c_str());
                    }
                    else 
                    {
                        d->setDataObject((Property&)*p,value);
                        delete(prop);
                        return;
                    }
                }
            }
        }
        if (prop != 0)delete prop;

        string msg("Path not valid:");
        msg += path;
        SDO_THROW_EXCEPTION("setDataObject", SDOPathNotFoundException,
            msg.c_str());
    }

void DataObjectImpl::setDataObject(const SDOString& path, DataObjectPtr value)
{
  DataObjectImpl* d;
  SDOString prop = findPropertyContainer(path, &d);
  if (d != 0)
  {
     if (!prop.empty()) {
        const PropertyImpl* p = d->getPropertyImpl(prop);
        if (p == 0 && d->getType().isOpenType())
        {
           if (value != 0)
           {
              p = d->defineDataObject(prop, value->getType());
           }
        }
        if (p != 0)
        {
           if (p->isMany())
           {
              DataObjectList& dol = d->getList((Property&)*p);
              long index;
              DataObjectImpl* dx = d->findDataObject(prop,&index);
              if (index >= 0)
                  {
                    if(index < dol.size())
                      {
                        dol.setDataObject((unsigned int)index,value);
                      }
                    else 
                      {
                        dol.append(value);
                      }
                    return;
                  }
                string msg("Set of data object on many valued item");
                msg += path;
                SDO_THROW_EXCEPTION("setDataObject", SDOUnsupportedOperationException,
                                    msg.c_str());
              }
            else 
              {
                d->setDataObject((Property&)*p,value);
                return;
              }
          }
      }
    }

  string msg("Path not valid:");
  msg += path;
  SDO_THROW_EXCEPTION("setDataObject", SDOPathNotFoundException,
                      msg.c_str());
}

    void DataObjectImpl::validateIndex(unsigned int index)
    {
        PropertyList pl = getType().getProperties();

        if (index >= pl.size()) {

            // open type support
            if (getType().isOpenType())
            {
                if (index < openBase + openProperties.size())
                {
                    return;
                }
            }

            string msg("Index of property out of range:");
            msg += index;
            SDO_THROW_EXCEPTION("Index Validation", SDOIndexOutOfRangeException,
                msg.c_str());
        }
    }

    void DataObjectImpl::transferChildren(DataObject* d, DataFactory* f)
    {
        bool isData = false;
        PropertyList pl = d->getInstanceProperties();
        for (int i=0;i<pl.size();i++)
        {
            // even primitives need their DF fixed up
            if (pl[i].getType().isDataType())
            {
                 isData = true;
            }

            if (!d->isSet(pl[i]) || d->isNull(pl[i]))
            {
                continue;
            }
            if (pl[i].isMany())
            {
                DataObjectList& dl = d->getList(pl[i]);
                for (int j=0;j<dl.size();j++)
                {
                    
                    DataObject* d2 = dl[j];
                    if (d2) {
                        ((DataObjectImpl*)d2)->setDataFactory(f);
                        if (!isData)transferChildren(d2,f);
                    }
                }
            }
            else
            {
                DataObject* d3 = d->getDataObject(pl[i]);
                if (d3)
                { 
                    ((DataObjectImpl*)d3)->setDataFactory(f);
                    if (!isData)transferChildren(d3,f);
                }
            }
        }
    }



    void DataObjectImpl::checkFactory(DataObjectPtr dob,
        unsigned int propertyIndex)
    {
        
        DataObjectImpl* d = (DataObjectImpl*)(DataObject*)dob;

        if (d->getDataFactory() == getDataFactory()) return;

        // Temporary alteration to test the feasibility and
        // problems associated with allowing data object 'migration'
        // lets move this one and all its children to the factory of the
        // new parent if the factories are compatible.
        
        DataFactoryImpl* f = (DataFactoryImpl*)getDataFactory();

        if (d->getContainer() != 0)
        {
            string msg("Insertion of object from another factory is only allowed if the parent is null: ");
            const Type& t = d->getType();
            msg += t.getURI();
            msg += "#";
            msg += t.getName();
            msg += " into property ";
            msg += getProperty(propertyIndex).getName();
            msg += " of ";
            msg += getType().getURI();
            msg += "#";
            msg += getType().getName();
            SDO_THROW_EXCEPTION("checkFactory", SDOInvalidConversionException,
                msg.c_str());
        }

        if (f->isCompatible(d->getDataFactory(),d))
        {
            d->setDataFactory(getDataFactory());
            transferChildren(d,getDataFactory());
            return;
        }


        // This is the default position....
        
        string msg("Insertion from incompatible factory ");
        const Type& t = d->getType();
        msg += t.getURI();
        msg += "#";
        msg += t.getName();
        msg += " into property ";
        msg += getProperty(propertyIndex).getName();
        msg += " of ";
        msg += getType().getURI();
        msg += "#";
        msg += getType().getName();
        SDO_THROW_EXCEPTION("checkFactory", SDOInvalidConversionException,
            msg.c_str());
    }


    void DataObjectImpl::checkType(    const Property& prop,
                                    const Type& objectType)
    {
        const Type& propType = prop.getType();
        if (propType.equals(objectType)) return;

        DataFactory* df = (DataFactory*)factory;

        const TypeImpl* ti = ((DataFactoryImpl*)df)->findTypeImpl
            (objectType.getURI(),objectType.getName());
        if (ti != 0)
        {
            do 
            {
                ti = (const TypeImpl*)ti->getBaseType();
                if (ti == 0) break;
                if (propType.equals(*ti)) return;
            } while (ti != 0);

            // allow types of any substitutes
            const PropertyImpl* pi = 
                getPropertyImpl(getPropertyIndex(prop));
            if (pi != 0) 
            {
                unsigned int subcount = pi->getSubstitutionCount();
                for (int i=0;i<subcount;i++)
                {
                    const Type* tsub = pi->getSubstitutionType(i);
                    if (tsub != 0 && tsub->equals(objectType)) return;
                }
            }
        }

        // no match..
        string msg("Insertion of object of incompatible type ");
        msg += objectType.getURI();
        msg += "#";
        msg += objectType.getName();
        msg += " into property of type ";
        msg += propType.getURI();
        msg += "#";
        msg += propType.getName();
        SDO_THROW_EXCEPTION("TypeCheck", SDOInvalidConversionException,
            msg.c_str());
    }

    void DataObjectImpl::setDataObject(unsigned int propertyIndex, DataObjectPtr value)
    {
        setDataObject(getProperty(propertyIndex), value);
    }

    void DataObjectImpl::setDataObject(const Property& prop, DataObjectPtr value)
    {
        unsigned int propertyIndex = getPropertyIndex(prop);

       
        if (value != 0)
        {
            checkFactory(value, propertyIndex);
            checkType(prop,value->getType());
        }

        validateIndex(propertyIndex);

        if (prop.isReference() && value != 0)
        {
            // just need to make sure that the object is already part of our
            // tree.
            DataObjectImpl* r1 = this;
            while (r1->getContainerImpl() != 0)
            {
                r1 = r1->getContainerImpl();
            }
            DataObjectImpl* r2 = (DataObjectImpl*)(DataObject*)value;
            while (r2->getContainerImpl() != 0)
            {
                r2 = r2->getContainerImpl();
            }
            if (r1 != r2)
            {
                string msg("Set of a reference to an object outside the graph");
                SDO_THROW_EXCEPTION("setDataObject", SDOUnsupportedOperationException,
                msg.c_str());
            }
        }

        if ((prop.isMany()))
        {
            string msg("Set operation on a many valued property:");
            msg += prop.getName();
            SDO_THROW_EXCEPTION("setDataObject", SDOUnsupportedOperationException,
                msg.c_str());
        }


        if (value == 0) 
        {
            PropertyValueMap::iterator j;
            for (j = PropertyValues.begin(); j != PropertyValues.end(); ++j)
            {
                if ((*j).first == propertyIndex) {
                    if (prop.isReference())
                    {
                        ((*j).second)->unsetReference(this, prop);
                    }
                    else
                    {
                        // log both deletion and change - change is not 
                        // automatically recorded by deletion.
                        ((*j).second)->logDeletion();
                    }
                    logChange(prop);
                    (*j).second = RefCountingPointer<DataObjectImpl>(0);
            
                    return;
                }
            }
            logChange(prop);
            PropertyValues.insert(PropertyValues.end(),rdo(propertyIndex,(DataObjectImpl*)0));
            return;
        }

        DataObject* dob = value;
        PropertyValueMap::iterator i;
        for (i = PropertyValues.begin(); i != PropertyValues.end(); ++i)
        {
            if ((*i).first == propertyIndex) {

                if (prop.isReference())
                {
                    ((*i).second)->unsetReference(this, prop);
                }
                else
                {
                    // log both deletion and change - change is not 
                    // automatically recorded by deletion.
                    ((*i).second)->logDeletion();
                }
                logChange(prop);

                (*i).second = RefCountingPointer<DataObjectImpl>((DataObjectImpl*)dob);

            
                if (prop.isReference())
                {
                    ((DataObjectImpl*)dob)->setReference(this, prop);
                }
                else
                {
                    logCreation((*i).second, this, prop);
                }
                return;
            }
        }
        if (prop.isReference())
        {
            ((DataObjectImpl*)dob)->setReference(this, prop);
        }
        else
        {
            ((DataObjectImpl*)dob)->setContainer(this);
            ((DataObjectImpl*)dob)->setApplicableChangeSummary();
            // log creation before putting into property values.
            // also log change - not done by logCreation
            logCreation((DataObjectImpl*)dob, this, prop);
        }

        logChange(prop);

        PropertyValues.insert(PropertyValues.end(),rdo(propertyIndex,(DataObjectImpl*)dob));
        return;
    }


    bool DataObjectImpl::isValid(const char* path)
    {
        DataObjectImpl* d;
        char* prop = findPropertyContainer(path,&d);
        if (d != 0) {
            if (prop != 0) {
                const Property& p = d->getProperty(prop);
                delete prop;
                return d->isValid(p);
            }
        }
        if (prop != 0)delete prop;
        string msg("Invalid path:");
        msg += path;
        SDO_THROW_EXCEPTION("isSet" ,SDOPathNotFoundException,
            msg.c_str());
    }

   
    // Returns whether a property of either this object or an object reachable 
    // from it, as identified by the specified path,
    // is considered to be set.
    // @param path the path to a valid Object* and property.
    
    bool DataObjectImpl::isSet(const char* path)
    {
        DataObjectImpl* d;
        char* prop = findPropertyContainer(path,&d);
        if (d != 0) {
            if (prop != 0) {
                const Property& p = d->getProperty(prop);
                delete prop;
                return d->isSet(p);
            }
        }
        if (prop != 0)delete prop;
        string msg("Invalid path:");
        msg += path;
        SDO_THROW_EXCEPTION("isSet" ,SDOPathNotFoundException,
            msg.c_str());
    }

    bool DataObjectImpl::isValid(unsigned int propertyIndex)
    {
        return isValid(getProperty(propertyIndex));
    }

    bool DataObjectImpl::isValid(const Property& p)
    {
        if (p.isDefaulted()) return true;
        if (isSet(p))return true;
        return false;
    }

    bool DataObjectImpl::isSet(unsigned int propertyIndex)
    {
        return isSet(getProperty(propertyIndex), propertyIndex);
    }

    bool DataObjectImpl::isSet(const Property& property)
    {
        return isSet(property, getPropertyIndex(property));
    }

    bool DataObjectImpl::isSet(const Property& prop, unsigned int propertyIndex)
    {
        PropertyValueMap::iterator i;
        for (i = PropertyValues.begin(); i != PropertyValues.end(); ++i)
        {
            if ((*i).first == propertyIndex) {
                if (prop.isMany())
                {
                    DataObjectImpl* dol = (*i).second;
                    if (dol != 0 && dol->getList().size() == 0)
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
    }


    // unSets a property of either this Object or an Object reachable from it, 
    // as identified by the specified path.
    // @param path the path to a valid Object and property.
    // @see #unSet(Property)

    void DataObjectImpl::unset(const char* path)
    {
        
        DataObjectImpl* d;
        char* prop = findPropertyContainer(path,&d);
        if (d != 0)
        {
            if (prop != 0){
                const Property& p = d->getProperty(prop);
                if (p.isMany())
                {
                    char *c;
                    if ((c = strchr(prop,'[')) != 0)
                    {
                        char *c1 = strchr(c,']');
                        if (c1 != 0)*c1 = 0;
                        unsigned int i = atoi(++c);
                        if (i > 0){
                            i--;
                            DataObjectList& li = d->getList(p);
                            li.remove(i);
                        }
                        delete prop;
                        return;
                    }
                    if ((c = strchr(prop,'.')) != 0)
                    {
                        unsigned int i = atoi(++c);
                        DataObjectList& li = d->getList(p);
                        li.remove(i);
                        delete prop;
                        return;
                    }
                }
                delete prop;
                d->unset(p);
                return;
            }
        }
        if (prop != 0) delete prop;

        string msg("Invalid path:");
        msg += path;
        SDO_THROW_EXCEPTION("unset", SDOPathNotFoundException,
            msg.c_str());
    }

    void DataObjectImpl::unset(unsigned int propertyIndex)
    {
        unset(getProperty(propertyIndex));
    }

    void DataObjectImpl::unset(const Property& p)
    {
        PropertyValueMap::iterator i;
        unsigned int index = getPropertyIndex(p);

        if (getType().isSequencedType())
        {
            Sequence* sq = getSequence();
            sq->removeAll(p);
        }

        for (i = PropertyValues.begin(); i != PropertyValues.end(); ++i)
        {
            if ((*i).first == index) {
                DataObjectImplPtr dol = (*i).second;
                if (p.getType().isDataType())
                {
                    dol->clearReferences();
                    logChange(index);
                    if (p.isMany()) {
                        DataObjectList& dl = dol->getList();
                        while (dl.size() > 0) 
                        {
                            RefCountingPointer<DataObject> dli = dl.remove(0);
                        }
                    }
                    else
                    {
                        PropertyValues.erase(i);
                    }
                }
                else {
                    // if its a reference, we dont want to delete anything
                    if (!p.isReference())
                    {
                        if (dol) { 
                            dol->clearReferences();
                            if (p.isMany()) {
                                DataObjectList& dl = dol->getList();
                                while (dl.size() > 0) 
                                {
                                    if (p.getType().isDataObjectType())
                                    {
                                        DataObject* dob = dl[0];
                                        ((DataObjectImpl*)dob)->logDeletion();
                                    }
                                    // the remove will record a change
                                    // remove will also clear the container.
                                    RefCountingPointer<DataObject> dli = dl.remove(0);
                                }
                            }
                            else 
                            {
                                PropertyValues.erase(i);
                                dol->logDeletion();
                                logChange(index);
                            }
                        }
                        else
                        {
                        logChange(index);
                        PropertyValues.erase(i);
                        }
                    }
                    else {
                        logChange(index);
                        PropertyValues.erase(i);
                    }
                }
                if (getType().isOpenType() && index >= openBase)
                {
                    if (p.isMany())
                    {
                        PropertyValues.erase(i);
                    }
                    undefineProperty(index);
                }
                return;
            }
        }
        return;
    }

    

    // Returns the value of a DataObject property identified by 
    // the specified path.
    // @param path the path to a valid object and property.
    // @return the DataObject value of the specified property.

    RefCountingPointer<DataObject> DataObjectImpl::getDataObject(const char* path)
    {
        DataObjectImpl* ptr = getDataObjectImpl(path);
        return RefCountingPointer<DataObject> ((DataObject*)ptr);
     }

  RefCountingPointer<DataObject> DataObjectImpl::getDataObject(const SDOString& path)
  {
    DataObjectImpl* ptr = getDataObjectImpl(path);
    return RefCountingPointer<DataObject> ((DataObject*)ptr);
  }

  DataObjectImpl* DataObjectImpl::getDataObjectImpl(const char* path)
    {
        
          DataObjectImpl* d = 0;
        char* prop = findPropertyContainer(path,&d);
        if (d != 0) {
            if (prop != 0) {
                if (strchr(prop,'[') || strchr(prop,'.')) {
                    /* Its a multlvalued property */
                    long l;
                    DataObjectImpl* theob = d->findDataObject(prop,&l);
                    delete prop;
                    if (theob == 0) {
                        string msg("Get DataObject - index out of range:");
                        msg += path;
                        SDO_THROW_EXCEPTION("getDataObject" ,SDOIndexOutOfRangeException,
                        msg.c_str());
                    }
                    return theob;
                }
                else 
                {
                    if (strlen(prop) == 0) 
                    {
                        delete prop;
                        prop = 0;
                        return d;
                    }
                    const Property& p = d->getProperty(prop);
                    delete prop;
                    prop = 0;
                    return d->getDataObjectImpl(p);
                }
            }
            else {
                return d;
            }
        }
        if (prop != 0)delete prop;

        string msg("Invalid path:");
        msg += path;
        SDO_THROW_EXCEPTION("getDataObject" ,SDOPathNotFoundException,
            msg.c_str());
    }

  // +++

  DataObjectImpl* DataObjectImpl::getDataObjectImpl(const SDOString& path)
  {
        
    DataObjectImpl* d = 0;
    SDOString prop = findPropertyContainer(path,&d);
    // char* prop = findPropertyContainer(path,&d);
    if (d != 0) {
      if (!prop.empty()) {
        if (prop.find_first_of("[.") != string::npos) {
          /* Its a multlvalued property */
          long l;
          DataObjectImpl* theob = d->findDataObject(prop,&l);
          if (theob == 0) {
            string msg("Get DataObject - index out of range:");
            msg += path;
            SDO_THROW_EXCEPTION("getDataObject" ,SDOIndexOutOfRangeException,
                                msg.c_str());
          }
          return theob;
        }
        else 
          {
            if (prop.length() == 0) 
              {
                return d;
              }
            const Property& p = d->getProperty(prop);
            return d->getDataObjectImpl(p);
          }
      }
      else {
        return d;
      }
    }

    string msg("Invalid path:");
    msg += path;
    SDO_THROW_EXCEPTION("getDataObject" ,SDOPathNotFoundException,
                        msg.c_str());
  }

  // ---

    RefCountingPointer<DataObject> DataObjectImpl::getDataObject(unsigned int propertyIndex)
    {
        if ((getProperty(propertyIndex).isMany()))
        {
            string msg("get operation on a many valued property:");
            msg += getProperty(propertyIndex).getName();
            SDO_THROW_EXCEPTION("getDataObject", SDOUnsupportedOperationException,
                msg.c_str());
        }
        DataObjectImpl* ptr = getDataObjectImpl(propertyIndex);;
        return RefCountingPointer<DataObject>((DataObject*)ptr);
    }

    DataObjectImpl* DataObjectImpl::getDataObjectImpl(unsigned int propertyIndex)
    {
        PropertyValueMap::iterator i;
        for (i = PropertyValues.begin(); i != PropertyValues.end(); ++i)
        {
            if ((*i).first == propertyIndex)
            {
                DataObject* dob = (*i).second;
                if (dob == 0 || ((DataObjectImpl*)dob)->isNull())return 0;
                return (DataObjectImpl*)dob;
            }
        }
        return 0;
    }

  
    RefCountingPointer<DataObject> DataObjectImpl::getDataObject(const Property& property)
    {
        DataObjectImpl* ptr = getDataObjectImpl(property);
        return RefCountingPointer<DataObject>((DataObject*)ptr);
    }

    DataObjectImpl* DataObjectImpl::getDataObjectImpl(const Property& property)
    {
        return getDataObjectImpl(getPropertyIndex(property));
    }





   // Returns a new DataObject contained by this Object using the specified property,
   // which must be a containment property.
   // The type of the created Object is the declared type of the specified property.

    RefCountingPointer<DataObject> DataObjectImpl::createDataObject(const char* propertyName)
    {
        // Throws runtime exception for type or property not found 

        const Property& p  = getProperty(propertyName);
        return createDataObject(p);
    }

    // Returns a new DataObject contained by this Object using the specified property,
    // which must be a containment property.
    // The type of the created Object is the declared type of the specified property.

    RefCountingPointer<DataObject> DataObjectImpl::createDataObject(unsigned int propertyIndex)
    {
        const Property& p  = getProperty(propertyIndex);
         return createDataObject(p);
    }

    // Returns a new DataObject contained by this Object using the specified property,
    // which must be a containment property.
    // The type of the created Object is the declared type of the specified property.
    
    RefCountingPointer<DataObject> DataObjectImpl::createDataObject(const Property& property)
    {
        const Type& tp = property.getType();
        return createDataObject(property,tp.getURI(), tp.getName());
    }


    // Returns a new DataObject contained by this Object using the specified property,
    // which must be a containment property.
    // The type of the created Object is the declared type of the specified property.

    RefCountingPointer<DataObject> DataObjectImpl::createDataObject(const Property& property, const char* namespaceURI, 
                                       const char* typeName)
    {
        if (!property.isContainment())
        {
            string msg("Create data object on non-containment property:");
            msg += property.getName();
            SDO_THROW_EXCEPTION("createDataObject", SDOUnsupportedOperationException,
            msg.c_str());
        }

        DataFactory* df = getDataFactory();
        if (property.isMany()) {
            /* add to the list */
            RefCountingPointer<DataObject> ptr = df->create(namespaceURI, typeName);
            DataObject* dob = ptr;
            ((DataObjectImpl*)dob)->setContainer(this);
            ((DataObjectImpl*)dob)->setApplicableChangeSummary();
            
            // log creation before adding to list - the change must record the old state 
            // of the list
            logCreation(((DataObjectImpl*)dob), this, property);
            //logChange(property);

            DataObjectImpl* theDO = getDataObjectImpl(property);
            if ( theDO == 0) { /* No value set yet */
                unsigned int ind = getPropertyIndex(property);
                RefCountingPointer<DataObject> listptr = 
                    df->create(Type::SDOTypeNamespaceURI,"DataObject");

                DataObject* doptr = listptr;
                PropertyValues.insert(PropertyValues.end(),rdo(ind,(DataObjectImpl*)doptr));
                ((DataObjectImpl*)doptr)->setContainer(this);
                ((DataObjectImpl*)doptr)->setApplicableChangeSummary();

                DataObjectListImpl* list = new DataObjectListImpl(df,
                    this, ind, namespaceURI,typeName);

                ((DataObjectImpl*)doptr)->setList(list);
                // the append will log a change to the property.
                list->append(ptr);

                // now done by list append
                //if (getType().isSequencedType())
                //{
                //    SequenceImpl* sq = getSequenceImpl();
                //    sq->push(property,0);
                //}
            }
            else 
            {
                DataObjectList& list =    theDO->getList();
                // the append will log a change to the property, and update the 
                // sequence
                list.append(ptr);
                //if (getType().isSequencedType())
                //{
                //    SequenceImpl* sq = getSequenceImpl();
                //    sq->push(property,list.size()-1);
                //}

            }
            return ptr;

        }
        else {
            unset(property);
            DataObjectImpl* ditem = 
              new DataObjectImpl(df, df->getType(namespaceURI, typeName));
            ditem->setContainer(this);
            ditem->setApplicableChangeSummary();

            // log both creation and change - creations no longer log
            // changes automatically.

            logCreation(ditem, this, property);
            logChange(property);

            PropertyValues.insert(PropertyValues.end(),
                                  rdo(getPropertyIndex(property),ditem));
            if (getType().isSequencedType())
            {
                SequenceImpl* sq = getSequenceImpl();
                sq->push(property,0);
            }
            return RefCountingPointer<DataObject>((DataObject*)ditem);
        }
        return 0;
    }

    void DataObjectImpl::setList( DataObjectList* theList)
    {
        listValue = (DataObjectListImpl*)theList;
    }


    bool DataObjectImpl::remove(DataObjectImpl* indol)
    {
        PropertyValueMap::iterator i;
        for (i = PropertyValues.begin(); i != PropertyValues.end(); ++i)
        {
            const Property& prop = getProperty((*i).first);
            if (prop.isMany())
            {
                DataObjectList& dol = ((*i).second)->getList();
                for (int j=0;j< dol.size(); j++)
                {
                    if (dol[j] == indol)
                    {
                        indol->logDeletion();
                        logChange(prop);
                        indol->setContainer(0);
                        dol.remove(j);
                        return true;
                    }
                }
            }
            DataObjectImpl* tmp = (*i).second;
            if (tmp == indol) {
                indol->logDeletion();
                logChange(prop);
                indol->setContainer(0);
                PropertyValues.erase(i);
                return true;
            }
        }
        return false;
     }

    // remove this Object from its container and dont unSet all its properties.

    void DataObjectImpl::detach()
    {
        // remove this data object from its tree
        clearReferences();
        if (container == 0) return; 
        container->remove(this);
        return ;
    }

    void DataObjectImpl::clear()
    {
        // clear this objects state
        PropertyValueMap::iterator i = PropertyValues.begin();

        while (i != PropertyValues.end()) 
        {
            unset((*i).first);
            i = PropertyValues.begin();
        }
        return ;
    }

    // Returns the containing Object
    // of 0 if there is no container.

    RefCountingPointer<DataObject> DataObjectImpl::getContainer()
    {
        DataObject* dob = (DataObject*)container;
          return RefCountingPointer<DataObject> (dob);
    }

    DataObjectImpl* DataObjectImpl::getContainerImpl()
    {
          return container;
    }

    void DataObjectImpl::setContainer(DataObjectImpl* d)
    {
          container = d;
    }

    const Property* DataObjectImpl::findInProperties(DataObject* ob)
    {
        PropertyValueMap::iterator i;
        for (i = PropertyValues.begin() ;i != PropertyValues.end() ; ++i)
        {
            if (getProperty((*i).first).isReference()) continue;
            if (getProperty((*i).first).isMany())
            {
                DataObjectList& dl = ((*i).second)->getList();
                for (int j = 0 ; j < dl.size(); j++)
                {
                    if (dl[j] == ob)
                    {
                        return &(getProperty((*i).first));
                    }
                }
            }
            else 
            {
                if ((*i).second == ob) 
                {
                    return &(getProperty((*i).first));
                }
            }
        }
        return 0; // this can happen if the object has been detached 

        //string msg("Object cannot find its containing property");
        //SDO_THROW_EXCEPTION("FindInProperties" ,SDOPropertyNotFoundException,
        //    msg.c_str());
    }

    // Return the Property of the data Object containing this data Object
    // or 0 if there is no container.

    const Property& DataObjectImpl::getContainmentProperty()
    {
        if (container != 0) {
            const Property* p = container->findInProperties(this);
            if (p != 0)return *p;
        }
        SDO_THROW_EXCEPTION("getContainmentProperty" ,SDOPropertyNotFoundException,
            "Object cannot find its containment property");
    }


    // Returns the data Object's type.
    // The type defines the properties available for reflective access.

    const Type& DataObjectImpl::getType()
    {
          return (const Type&)(*ObjectType);
    }

    const Type::Types DataObjectImpl::getTypeEnum()
    {
           return ObjectType->getTypeEnum();
    }

    const TypeImpl& DataObjectImpl::getTypeImpl()
    {
          return (const TypeImpl&)*ObjectType;
    }


    // open type support

    const Property& DataObjectImpl::getProperty(const char* prop)
    {
        PropertyImpl* pi = getPropertyImpl(prop);
        if (pi == 0)
        {
            string msg("Cannot find property:");
            msg += prop;
            SDO_THROW_EXCEPTION("getProperty", SDOPropertyNotFoundException,
            msg.c_str());

        }
        return (Property&)*pi;
     }
  const Property& DataObjectImpl::getProperty(const SDOString& prop)
  {
    PropertyImpl* pi = getPropertyImpl(prop);
    if (pi == 0)
      {
        string msg("Cannot find property:");
        msg += prop;
        SDO_THROW_EXCEPTION("getProperty", SDOPropertyNotFoundException,
                            msg.c_str());

      }
    return (Property&)*pi;
  }

    PropertyImpl* DataObjectImpl::getPropertyImpl(const char* prop)
    {
        PropertyImpl* pi = getTypeImpl().getPropertyImpl(prop);
        if (pi != 0) return pi;

        if (getType().isOpenType())
        {
            std::list<PropertyImpl>::iterator j;
            for (j=openProperties.begin(); 
                 j != openProperties.end(); ++j)
            {
                if (!strcmp((*j).getName(), prop))
                {
                    return (PropertyImpl*)&(*j);
                }
            }
        }
        return 0;
     }
  PropertyImpl* DataObjectImpl::getPropertyImpl(const SDOString& prop)
  {
    PropertyImpl* pi = getTypeImpl().getPropertyImpl(prop);
    if (pi != 0) return pi;

    if (getType().isOpenType())
      {
        std::list<PropertyImpl>::iterator j;
        for (j=openProperties.begin(); 
             j != openProperties.end(); ++j)
          {
            if (!strcmp((*j).getName(), prop.c_str()))
              {
                return (PropertyImpl*)&(*j);
              }
          }
      }
    return 0;
  }

    DataFactory* DataObjectImpl::getDataFactory()
    {
        return factory;
    }

    void DataObjectImpl::setDataFactory(DataFactory* df)
    {
        ObjectType = (TypeImpl*)&(df->getType(ObjectType->getURI(),
                        ObjectType->getName()));
        factory = df;
    }

    ///////////////////////////////////////////////////////////////////////////
    // These finally are the setters/getters for primitives given
    // that the data object is a primitive type.
    ///////////////////////////////////////////////////////////////////////////
    

    bool DataObjectImpl::getBoolean()
    {
        return getTypeImpl().convertToBoolean(value, valuelength);
    }


    char DataObjectImpl::getByte()
    {
        return getTypeImpl().convertToByte(value,valuelength);

    }


    wchar_t DataObjectImpl::getCharacter()
    {
        return getTypeImpl().convertToCharacter(value,valuelength);

    }

    long DataObjectImpl::getInteger() 
    {
        return getTypeImpl().convertToInteger(value,valuelength);

    }


    long double DataObjectImpl::getDouble()
    {
        return getTypeImpl().convertToDouble(value,valuelength);
    }


    float DataObjectImpl::getFloat()
    {
        return getTypeImpl().convertToFloat(value,valuelength);

    }



    int64_t DataObjectImpl::getLong()
    {
        return getTypeImpl().convertToLong(value,valuelength);

    }


    short DataObjectImpl::getShort()
    {
        return getTypeImpl().convertToShort(value,valuelength);

    }

    unsigned int DataObjectImpl::getString( wchar_t* outptr, unsigned int max)
    {
        if (outptr == 0 || max == 0) return valuelength;
        return getTypeImpl().convertToString(value, outptr, valuelength, max);

    }
    unsigned int DataObjectImpl::getBytes( char* outptr, unsigned int max)
    {
        if (outptr == 0 || max == 0) return valuelength;
        return getTypeImpl().convertToBytes(value, outptr, valuelength, max);

    }

    const char* DataObjectImpl::getCString()
    {
        return getTypeImpl().convertToCString(value, &asStringBuffer, valuelength);

    }

    const SDODate DataObjectImpl::getDate()
    {
        return getTypeImpl().convertToDate(value, valuelength); /* time_t == long*/

    }

    DataObjectImpl* DataObjectImpl::getDataObject()
    {
        return (DataObjectImpl*)getTypeImpl().convertToDataObject(value, valuelength);

    }


    void DataObjectImpl::setBoolean(bool invalue)
    {
        valuelength = getTypeImpl().convert(&value,invalue);
        return;
    }


    void DataObjectImpl::setByte(char invalue)
    {
        valuelength = getTypeImpl().convert(&value,invalue);
        return;

    }


    void DataObjectImpl::setCharacter(wchar_t invalue)
    {
        valuelength = getTypeImpl().convert(&value,invalue);
        return;
    }

    void DataObjectImpl::setString(const wchar_t* invalue, unsigned int len)
    {
        valuelength = getTypeImpl().convert(&value,invalue, len);
        return;
    }

    void DataObjectImpl::setBytes(const char* invalue, unsigned int len)
    {
        valuelength = getTypeImpl().convert(&value,invalue, len);
        return;
    }

   void DataObjectImpl::setInteger(long invalue) 
    {
#if __WORDSIZE ==64
        valuelength = getTypeImpl().convert(&value,(int64_t)invalue);
#else
        valuelength = getTypeImpl().convert(&value,invalue);
#endif
        return;
    }


    void DataObjectImpl::setDouble(long double invalue)
    {
        valuelength = getTypeImpl().convert(&value,invalue);
        return;
    }

    void DataObjectImpl::setFloat(float invalue)
    {
        valuelength = getTypeImpl().convert(&value,invalue);
        return;

    }


    void DataObjectImpl::setLong(int64_t invalue)
    {
        valuelength = getTypeImpl().convert(&value,invalue);
        return;
    }


    void DataObjectImpl::setShort(short invalue)
    {
        valuelength = getTypeImpl().convert(&value,invalue);
        return;

    }

    void DataObjectImpl::setCString(const char* invalue)
    {
        valuelength = getTypeImpl().convert(&value,invalue);
        return;
    }

    void DataObjectImpl::setDate(const SDODate invalue)
    {
        valuelength = getTypeImpl().convertDate(&value,invalue); /* time_t == long*/
        return;
    }

    void DataObjectImpl::setDataObject(DataObject* invalue)
    {
        valuelength = getTypeImpl().convert(&value,invalue);
        return;
    }

    void DataObjectImpl::setNull()
    {
        isnull = true;
    }

    bool DataObjectImpl::isNull()
    {
        return isnull;
    }

    void DataObjectImpl::unsetNull()
    {
        isnull = false;
    }


    DataObjectImpl::DataObjectImpl(const TypeImpl& t) 
    {
        ObjectType = (TypeImpl*)&t;
        container = 0;
        value = 0; /* Will be initialized when used */
        valuelength = 0;
        asStringBuffer = 0;
//        asXPathBuffer = 0;
        isnull = false;
        
        // open type support
        openBase = t.getPropertiesSize() ;

        userdata = (void*)0xFFFFFFFF;

        if (t.isChangeSummaryType())
        {
            changesummaryobject = 0;
            localCS = new ChangeSummaryImpl();
        }
        else 
        {
            changesummaryobject = 0;
            localCS = 0;
        }

        if (getType().isSequencedType()) sequence = new SequenceImpl(this);
        else sequence = 0;
    }



    DataObjectImpl::DataObjectImpl(DataFactory* df, const Type& t) : ObjectType((TypeImpl*)&t),
        factory(df)
    {
        container = 0;
        value = 0;
        valuelength = 0;
        asStringBuffer = 0;
//        asXPathBuffer = 0;
        isnull = false;

        // open type support
        openBase = ObjectType->getPropertiesSize() ;

        userdata = (void*)0xFFFFFFFF;

        if (ObjectType->isChangeSummaryType())
        {
            changesummaryobject = 0;
            localCS = new ChangeSummaryImpl();
        }
        else 
        {
            changesummaryobject = 0;
            localCS = 0;
        }

        if (getType().isSequencedType()) 
        {
            sequence = new SequenceImpl(this);
        }
        else 
        {
            sequence = 0;
        }
    }

    void DataObjectImpl::deleteValue()
    {
        switch (getTypeEnum())
        {
        case  Type::BooleanType:
        case  Type::ByteType:
        case  Type::CharacterType:
        case  Type::IntegerType: 
        case  Type::DateType:   
        case  Type::DoubleType:
        case  Type::FloatType:
        case  Type::LongType:
        case  Type::ShortType:
        case  Type::BytesType:
            delete (char*)value;
            return;

        case  Type::BigDecimalType:
        case  Type::BigIntegerType:
        case  Type::StringType:
        case  Type::UriType:
            delete (wchar_t*)value;
            return;

        case Type::DataObjectType:
            return;

        default:
            return;
        }
    }

    DataObjectImpl::~DataObjectImpl()
    {
        // We do not want to log changes to our own deletion
        // if this DO owns the ChangeSummary. Do not delete
        // it here as contained DOs still have a reference to it.

        if (getTypeImpl().isChangeSummaryType())
        {
            ChangeSummaryPtr c = getChangeSummary();
            if (c) {
                if (c->isLogging())
                {
                    c->endLogging();
                }
            }
        }


        clearReferences();
         PropertyValueMap::iterator i = PropertyValues.begin();
        while (i != PropertyValues.end()) 
        {
            unset((*i).first);
            if (i == PropertyValues.begin())
            {
                // unset has not removed the item from the list - do it 
                // here instead
                PropertyValues.erase(i);
            }
            i = PropertyValues.begin();
        }

        // Theory: A DO cant get here if its still attached to anything,
        //so we dont need to detach....
        //detach();

        
        if (asStringBuffer != 0) delete asStringBuffer;
//        if (asXPathBuffer != 0) delete asXPathBuffer;

        if (value != 0) 
        {
            if (getType().isDataType())deleteValue();
        }
        
        

        if (getType().isSequencedType()) 
        {
            if (sequence != 0) delete sequence;
        }


        if (getTypeImpl().isChangeSummaryType()    )
        {
            if (getChangeSummary() != 0) 
            {
                delete localCS; 
                localCS = 0;
            }
        }
    }

    void DataObjectImpl::setApplicableChangeSummary()
    {
        changesummaryobject = 0;
        if (getType().isChangeSummaryType())
        {
            changesummaryobject = 0;
            return;
        }
        else {
            DataObjectImpl* dob = getContainerImpl();
            while (dob != 0) {
                if (dob->getType().isChangeSummaryType())
                {
                    changesummaryobject = dob;
                    return;
                }
                dob = dob->getContainerImpl();
            }
        }

    }

    void DataObjectImpl::logCreation(DataObjectImpl* dol, DataObjectImpl* cont,
        const Property& theprop)
    {
        if (getChangeSummaryImpl() != 0 && getChangeSummaryImpl()->isLogging())
        {
            getChangeSummaryImpl()->logCreation(dol,cont,theprop); 
        }
    }

    void DataObjectImpl::logDeletion()
    {
        // Only log if ChangeSummary is inherited from container

        if (getChangeSummaryImpl() != 0 && getChangeSummaryImpl()->isLogging() && !getType().isChangeSummaryType())
        {
            DataObjectImpl* cont = getContainerImpl();
            if (cont != 0)    // log if there is a container. If there is not, then
                            // this can only be the object with the CS, so logging
                            // would not make sense.
            {
                const Property* p = cont->findInProperties(this);
                if ( p != 0)    // if the object is not in the properties, then its been
                                // detached, and has already been logged as deleted
                {
                    getChangeSummaryImpl()->logDeletion(this,cont,*p,
                        objectToXPath(), true);
                }
            }
        }
    }

    void DataObjectImpl::logChange(const Property& prop)
    {
        if (getChangeSummaryImpl() != 0 && getChangeSummaryImpl()->isLogging())
        {
            getChangeSummaryImpl()->logChange(this,prop);
        }
    }

    void DataObjectImpl::logChange(unsigned int propIndex)
    {
        if (getChangeSummaryImpl() != 0 && getChangeSummaryImpl()->isLogging())
        {
            getChangeSummaryImpl()->logChange(this,getProperty(propIndex));
        }
    }
    // reference support

    void DataObjectImpl::setReference(DataObject* dol, const Property& prop)
    {
        LOGINFO_1(INFO,"ChangeSummary:Setting a reference to %s",prop.getName());

        refs.push_back(new Reference(dol,prop));
    }
    void DataObjectImpl::unsetReference(DataObject* dol, const Property& prop)
    {
        LOGINFO_1(INFO,"ChangeSummary:Unsetting a reference to %s",prop.getName());

        for (int i=0;i< refs.size();i++)
        {
            if (refs[i]->getDataObject() == dol)
            {
                if (!strcmp(refs[i]->getProperty().getName(),
                    prop.getName()))
                {
                    delete refs[i];
                    refs.erase(refs.begin() + i);
                }
            }
        }
    }


    void DataObjectImpl::clearReferences()
    {
        for (int i=0;i<refs.size();i++)
        {
            // Note - no loop as the referer must be of type reference
            refs[i]->getDataObject()->unset(refs[i]->getProperty());
            delete refs[i];
        }
        refs.clear();
    }

    const char* DataObjectImpl::objectToXPath()
    {
        asXPathBuffer = "";

        DataObjectImpl* dob = getContainerImpl();
        DataObject*thisob = this;
        while (dob != 0){
            const Property& p = thisob->getContainmentProperty();
            if (asXPathBuffer != "")
            {
                asXPathBuffer = "/" + asXPathBuffer;
            }

            if (p.isMany()) {
                DataObjectList& dol = dob->getList(p);
                for (int i=0;i<dol.size();i++)
                {
                    if (dol[i] == thisob)
                    {
                        char index[64];
                        sprintf(index,"%d",i);
                        asXPathBuffer = index + asXPathBuffer;
                        asXPathBuffer = "." + asXPathBuffer;
                        break;
                    }
                }
            }
            asXPathBuffer = p.getName() + asXPathBuffer;

            thisob = dob;
            dob = dob->getContainerImpl();
        }

        asXPathBuffer = "#/" + asXPathBuffer;

        return asXPathBuffer.c_str();
/*
        char* temp1;
        char* temp2;

        if (asXPathBuffer == 0)
        {
            asXPathBuffer = new char[2];
            sprintf(asXPathBuffer,"#");
        }

        DataObjectImpl* dob = getContainerImpl();
        DataObject*thisob = this;
        while (dob != 0){
            const Property& p = thisob->getContainmentProperty();
            const char* name = p.getName();
            temp1 = new char[strlen(name) + 34];
            temp1[0] = 0;

            
            if (p.isMany()) {
                DataObjectList& dol = dob->getList(p);
                for (int i=0;i<dol.size();i++)
                {
                    if (dol[i] == thisob)
                    {
                        sprintf(temp1,"#/%s.%d",name,i);
                        break;
                    }
                }
            }
            else {
                sprintf(temp1,"#/%s",name);
            }
            if (asXPathBuffer != 0) {
                temp2 = new char[strlen(asXPathBuffer) + strlen(temp1)  + 1];
                sprintf(temp2,"%s%s", temp1, asXPathBuffer+1 );
                delete asXPathBuffer;
            }
            else {
                temp2 = new char[strlen(temp1)  + 1];
                sprintf(temp2,"%s", temp1);
            }
            delete temp1;
            asXPathBuffer = temp2;
            thisob = dob;
            dob = dob->getContainerImpl();
        }
        return asXPathBuffer; */
    }

    // user data support...
    void* DataObjectImpl::getUserData(const char* path)
    {
        DataObjectImpl *d;
        void* v = 0;
        char *spath = 0;
        char* prop = 0;
        try {
            spath = DataObjectImpl::stripPath(path);
              prop = findPropertyContainer(spath,&d);
            if (spath)
            {
                delete spath;
                spath = 0;
            }
            if (d != 0) 
            {
                if (prop != 0)
                {
                    const Property& p = d->getProperty(prop);
                    if (p.getType().isDataType()) return 0;
                    if (p.isMany())
                    {
                        DataObjectImpl* d2 = d->getDataObjectImpl(prop);
                        if (d2) v = d2->getUserData();
                        delete prop;
                        prop = 0;
                        return v;
                    }
                    v = d->getUserData(p);
                    delete prop;
                    prop = 0;
                    return v;
                }
                return d->getUserData();
            }
            return 0;
        }
        catch (SDORuntimeException e)
        {
            if (prop)  delete prop;
            if (spath) delete spath;
            return 0;
        }
                
    }

    void* DataObjectImpl::getUserData(unsigned int propertyIndex)
    {
        if ((getProperty(propertyIndex).isMany()))
        {
            return 0;
        }
        if ((getProperty(propertyIndex).getType().isDataType()))
        {
            return 0;
        }
        DataObjectImpl* ptr = getDataObjectImpl(propertyIndex);
        if (ptr) return ptr->getUserData();
        return 0;
    }

    void* DataObjectImpl::getUserData(const Property& property)
    {
        if (property.isMany())
        {
            return 0;
        }
        if (property.getType().isDataType())
        {
            return 0;
        }
        DataObjectImpl* ptr = getDataObjectImpl(property);
        if (ptr) return ptr->getUserData();
        return 0;
    }

    void* DataObjectImpl::getUserData()
    {
        return userdata;
    }

    void DataObjectImpl::setUserData(const char* path, void* value)
    {
        char *spath = 0;
        char* prop = 0;
         DataObjectImpl *d;
        try {
            spath = DataObjectImpl::stripPath(path);
              prop = findPropertyContainer(spath,&d);
            if (spath)
            {
                delete spath;
                spath = 0;
            }
            if (d != 0) 
            {
                if (prop != 0)
                {
                    const Property& p = d->getProperty(prop);
                    if (p.getType().isDataType()) return;
                    if (p.isMany())
                    {
                        DataObjectImpl* d2 = d->getDataObjectImpl(prop);
                        if (d2) d2->setUserData(value);
                        delete prop;
                        prop = 0;
                        return;
                    }
                    d->setUserData(p,value);
                    delete prop;
                    prop = 0;
                    return;
                }
                d->setUserData(value);
                return;
            }
        }
        catch (SDORuntimeException e)
        {
            if (prop)  delete prop;
            if (spath) delete spath;
            return;
        }
                
    }

    void DataObjectImpl::setUserData(unsigned int propertyIndex, void* value)
    {
        if ((getProperty(propertyIndex).isMany()))
        {
            return;
        }
        if ((getProperty(propertyIndex).getType().isDataType()))
        {
            return;
        }
        DataObjectImpl* ptr = getDataObjectImpl(propertyIndex);
        if (ptr) ptr->setUserData(value);
        return;
    }

    void DataObjectImpl::setUserData(const Property& property, void* value)
    {
        if (property.isMany())
        {
            return;
        }
        if (property.getType().isDataType())
        {
            return;
        }
        DataObjectImpl* ptr = getDataObjectImpl(property);
        if (ptr) ptr->setUserData(value);
        return;
    }

    void DataObjectImpl::setUserData(void* value)
    {
        userdata = value;
    }

};
};
