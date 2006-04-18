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

/* $Rev$ $Date: 2006/04/06 12:58:18 $ */

#ifndef _PROPERTY_H_
#define _PROPERTY_H_


#include "commonj/sdo/export.h"

#include "commonj/sdo/Type.h"
#include "commonj/sdo/SDODate.h"

namespace commonj{
namespace sdo{

class Type;
class TypeImpl;
class DataObject;

/**  
 *
 * A representation of a property in the type of a data object.
 */

class Property
{
    public:
    
    /**  getName gets the name of the property
     *
     * Returns the name of the property.
     */

     virtual const SDO_API char* getName() const = 0;
  
    /**  getAlias returns the n'th alias
     *
     * This method returns a const char* corresponding to the
     * alias at index n of the list of aliases. Use getAliasCount to 
     * discover the size of the list.
     */

    virtual SDO_API const char* getAlias(unsigned int index = 0) const = 0;
    
    /**  getAliasCount  returns the number of aliases
     *
     * This method returns the number of aliases for this type
     */

    virtual SDO_API unsigned int getAliasCount() const = 0;

    /**  getType returns the type of this property
     *
     * This method returns the type, which may be a DataType or a
     * DataObjectType
     */

    virtual SDO_API const Type& getType() const = 0;

    /**  getTypeEnum gets the enum for this type. 
     *
     * Each DataType has a defined value in the list of Types.
     */

    virtual SDO_API const Type::Types getTypeEnum() const = 0;

     /**  isMany is true if the property is a list 
     *
     * IsMany returns true if this property represents a list of
     * values, and should be accessed via the getList DataObjectAPI.
     */

    virtual bool SDO_API isMany() const = 0;
  
     /**  isContainment is true if the property value is contained 
     *
     * IsContainment returns true if this property represents a DataObjectType,
     * and that DataObjectType is contained. I.E the property value is not a pointer
     * to a DataObject somewhere else in the graph, it is an actual value.
     */

    virtual bool SDO_API isContainment() const = 0;

     /**  isReference is true if the property value is not contained 
     *
     * IsReference returns true if this property represents a DataObjectType,
     * and that DataObjectType is not contained. I.E the property value is a pointer
     * to a DataObject somewhere else in the graph not an actual value.
     */

    virtual bool SDO_API isReference() const = 0;
  
    /**  getContainingType give the type which holds this property.
     *
     * Although many types may have a property of the same name, any given 
     * instance of a property belongs to only one type.
     * This method returns the type which holds this proeprty.
     */

    virtual SDO_API const Type& getContainingType() const = 0;
  


    /**  isReadOnly returns true if the property is unmodifiable.
     *
     * NOT IMPLEMENTED
     * Returns true if values for this Property cannot be modified using the SDO APIs.
     * When true, DataObject.set(Property property, Object value) throws an exception.
     * Values may change due to other factors, such as services operating on DataObjects.
     */

    virtual bool SDO_API isReadOnly() const = 0;

    /**  getOpposite  returns the opposite property or zero.
     *
     * NOT IMPLEMENTED
     */

    virtual SDO_API const Property* getOpposite() const = 0;

    /**  isDefaulted is true if a default has been set.
     *
     * A property value may be set or unset. If unset, requests to the
     * data object for the value will return a default if there is one.
     * If the property is not defaulted, an un specified value will be
     * returned. (Thism value will probably be zero).
     */

    virtual SDO_API bool isDefaulted() const = 0 ;

    /**  setDefault sets the right sort of default.
     *
     * The many overrides of this method allow the setting
     * of a default value for any DataType property.
     */

    virtual SDO_API void setDefault(bool b ) = 0;
    virtual SDO_API void setDefault(char c) = 0;
    virtual SDO_API void setDefault(wchar_t c) = 0;
    virtual SDO_API void setDefault(char* c) = 0;
    virtual SDO_API void setDefault(short s) = 0;
#if __WORDSIZE !=64
    virtual SDO_API void setDefault(long l) = 0;
#endif
    virtual SDO_API void setDefault(int64_t i) = 0;
    virtual SDO_API void setDefault(float f) = 0;
    virtual SDO_API void setDefault(long double d) = 0;
    virtual SDO_API void setDefault(const SDODate d) = 0;
    virtual SDO_API void setDefault(const wchar_t* c, unsigned int len) = 0;
    virtual SDO_API void setDefault(const char* c, unsigned int len) = 0;

    /**  getDefault gets the right sort of default.
     *
     * The many overrides of this method allow the getting
     * of a default value for any DataType property.
     */

    virtual SDO_API unsigned int     getStringDefault(wchar_t* val, unsigned int max) const = 0;
    virtual SDO_API unsigned int    getBytesDefault(char* val, unsigned int max) const = 0;
    virtual SDO_API bool        getBooleanDefault() const = 0;
    virtual SDO_API char        getByteDefault() const = 0;
    virtual SDO_API wchar_t     getCharacterDefault() const = 0;
    virtual SDO_API short       getShortDefault() const = 0;
    virtual SDO_API long        getIntegerDefault() const = 0;
    virtual SDO_API int64_t     getLongDefault() const = 0;
    virtual SDO_API float       getFloatDefault() const = 0;
    virtual SDO_API long double getDoubleDefault() const = 0;
    virtual SDO_API const SDODate  getDateDefault() const = 0;
    virtual SDO_API const char*   getCStringDefault() const = 0;
    virtual SDO_API unsigned int getDefaultLength() const = 0;


};


};
};

#endif //_PROPERTY_H_
