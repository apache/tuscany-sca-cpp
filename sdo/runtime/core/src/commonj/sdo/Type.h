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

/* $Rev$ $Date: 2006/02/02 16:41:30 $ */

#ifndef _SDO_TYPE_H_
#define _SDO_TYPE_H_


#include "commonj/sdo/export.h"

#include "commonj/sdo/PropertyList.h"

namespace commonj{
namespace sdo{




class Property;

    /**  
     * 
     * Type is a representation of the type of a property of a data object.
     */

class Type
{

public:


/** \enum Types
 *
 * This is the enumerator for all DataType types. All DataObjectType types
 * appear as a Type::Types of DataObjectType.
 * otherTypes, ChangeSumamryType and TextType are not used by client code.
 */


   SDO_API enum Types
   {
    // Zero is a value for 'unknown type; - all data objects'
    OtherTypes = 0,
    BigDecimalType, 
    BigIntegerType, 
    BooleanType,   
    ByteType,
    BytesType,
    CharacterType,
    DateType,      
    DoubleType,    
    FloatType,    
    IntegerType, 
    LongType,      
    ShortType,     
    StringType,    
    UriType,
    DataObjectType,
    ChangeSummaryType,
    TextType,
    UndefinedOpenPropertyType,
    num_types
   };

    virtual SDO_API ~Type();

    /**  getName returns the name of the type
     *
     * This method returns a const char* name of the type.
     */

    virtual SDO_API const char* getName() const = 0;
  
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

    /**  getBaseType returns the base if there is one
     *
     * This method returns a const Type* corresponding to the
     * base Type for this type. The base type is the one which
     * this type inherits from.
     */
    
    virtual SDO_API const Type* getBaseType() const = 0;

    /**  getURI  returns the URI for this type
     *
     * This method returns the URI for this type. The URI may be
     * null.
     */

    virtual SDO_API const char* getURI() const = 0;



    /**  getProperties returns a list of properties for this type
     *
     * This method returns the list of properties for this type. 
     * Instances of open types may have more properties than appear
     * in this list.
     * See the propertylist API.
     */

    virtual SDO_API PropertyList  getProperties() const = 0;

 
    /**  getProperty returns a property for this type
     *
     * This method returns a property, by index or by name
     */

    virtual SDO_API const Property& getProperty(const char* propertyName)  const = 0;
    virtual SDO_API const Property& getProperty(unsigned int index)  const = 0;

    /**  getPropertyIndex returns an index 
     *
     * This method returns a property index for a named property
     */

    virtual SDO_API unsigned int getPropertyIndex(const char* propertyName) const  = 0;
 
    /**  isDataObjectType true if not a DataType 
     *
     * This method returns true if the type is not a DataType, and is therefore
     * a DataObjectType with properties.
     */

    virtual SDO_API bool isDataObjectType() const = 0;
  

    /**  isSequencedType true if the type is sequenced 
     *
     * This method returns true if the type is sequenced, and is therefore
     * objects of this type can be manipulate via their sequence interface.
     */

    virtual SDO_API bool isSequencedType() const = 0;


    /**  isOpenType true if the type is open 
     *
     * Normal types have a predefined list of properties. Trying to set 
     * properties which do not exist will cause an exception to be thrown.
     * Open types, on the other hand, may have properties added to their 
     * instances runtime. These properties get added silently when setting a
     * property value for a property which does not exist.
     * Different instances of these objects may have different lists of 
     * open properties.
     */

    virtual SDO_API bool isOpenType() const = 0;

    /**  isAbstractType true if the type is not instantiable. 
     *
     * An abstract type may not be instantiated. It is useful only as 
     * a base type to some other non-abstract type.
     */

    virtual SDO_API bool isAbstractType() const = 0;

    /**  isDataType true if the type is not an object. 
     *
     * A DataType is anything which is not a DataObjectType. This method
     * is the opposite of isDataObjectType().
     */

    virtual    SDO_API bool isDataType() const = 0;


    /**  isChangeSummaryType true if the type is a change summary. 
     *
     * There is only one type called ChangeSummary. This method serves
     * no purpose in the C++ implementation.
     */

    virtual    SDO_API bool isChangeSummaryType() const = 0;

    /**  getTypeEnum gets the enum for this type. 
     *
     * Each DataType has a defined value in the list of Types.
     */

    virtual    SDO_API Type::Types getTypeEnum() const = 0;

    /**  equals compares uri and name. 
     *
     * The types are equal if the URI and Name are equal.
     */

    virtual SDO_API bool equals(const Type& tother) const = 0;

    static SDO_API const char* SDOTypeNamespaceURI;
};

};
};
#endif //_SDO_TYPE_H_

