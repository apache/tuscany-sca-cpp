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

/* $Rev$ $Date: 2006/04/07 13:35:33 $ */

#ifndef _DATAFACTORY_H_
#define _DATAFACTORY_H_

#include "commonj/sdo/export.h"

#include "commonj/sdo/TypeList.h"

#include "commonj/sdo/RefCountingObject.h"
#include "commonj/sdo/RefCountingPointer.h"
#include "commonj/sdo/SDODate.h"
#include "commonj/sdo/DASValue.h"
#include "commonj/sdo/SDOString.h"


namespace commonj{
namespace sdo{

class DataObject;
class Type;

/**  
 *    DataFactory holds metadata and creates DataObjects.
 *
 *    The DataFactory holds properties and types loaded from XSD, or
 *    created using the DataFactory API.
 */

class DataFactory : public RefCountingObject
{
    public:


        /**
         *  getDataFactory is static and gets a data factory
         *
         * Static method to get a new data factory, with a set of base Types defined. 
         * With the data factory returned, either load Type definitions 
         * from XSD, or use the DataFactory APIs to define the types.
         */

        static SDO_API DataFactoryPtr getDataFactory();
        
        virtual SDO_API ~DataFactory();


        /**
         *  DataFactory::clone copies a data factory
         *
         * Copy the data factory, and return a new data factory which 
         * has the same properties and types, but is still able to have
         * new types added to it.
         */

        SDO_API virtual DataFactoryPtr clone();

        /**
         *  DataFactory::create creates a data object.
         *
         * Create a data object based on the type specified as a parameter
         * Once a data object has been created by this factory, the metadata
         * (types and properties) may no longer be altered.
         */

        SDO_API virtual DataObjectPtr create(const char* uri, const char* typeName)  = 0;

        SDO_API virtual DataObjectPtr create(const SDOString& uri, const SDOString& typeName)  = 0;

        SDO_API virtual DataObjectPtr create(const Type& type)  = 0;

        /**
         *  DataFactory::getType gets a type back from the factory.
         *
         * Get a type as specified in the data factory. Useful for creating
         * data objects or querying properties.
         */

        virtual const Type& getType(const char* uri, const char* inTypeName) const = 0;

        virtual const Type& getType(const SDOString& uri, const SDOString& inTypeName) const = 0;


        /**
         *  DataFactory::getTypes gets a list of types back from the factory.
         *
         * Get all the types available within this data factory. Useful for 
         * validating whether a data object is of the correct type to be 
         * usable.
         */

        virtual TypeList getTypes() const = 0;

        /**
         *  DataFactory::addType adds a type definition.
         *
         * Add a type defintion to the factory. (Properties may be added later).
         * The type is defined by its uri and name.
         * -# The type may be sequenced - and therefore work with a sequence API.
         * -# The type may be open, indicating that it may have extra properties
         * added at runtime.
         * -# The type may be abstract, so the data factory will not permit creation
         * of instances.
         * -# The type may be a data type, indicating that is will not have properties.
         * The Type may inherit from another type, but that is specified later with a 
         * call to setBaseType().
         */

        
        virtual SDO_API void addType(const char* uri, const char* inTypeName,
            bool isSequenced = false, 
            bool isOpen      = false,
            bool isAbstract  = false,
            bool isDataType  = false) = 0;

		virtual SDO_API void addType(const SDOString& uri, const SDOString& inTypeName,
            bool isSequenced = false, 
            bool isOpen      = false,
            bool isAbstract  = false,
            bool isDataType  = false) = 0;
        

        /**
         *  DataFactory::setBaseType allows inheritance
         *
         * The type specified second in the parameters becomes the basetype
         * of the first parameter.
         * The second type will have all the properties of its baser type, followed
         * by any additional properties of its own. The property indices of the properties
         * of the subclass will begin at one more than the total number of
         * properties of the supertype
         * This relationship is fixed when the first data object of any type is
         * created by the factory. Up to that point the base type may be changed.
         * 
         */

        virtual SDO_API void setBaseType( 
            const Type& type,
            const Type& base,
            bool isRestriction = false) = 0;
        
        virtual SDO_API void setBaseType( 
            const char* typeuri,
            const char* typenam,
            const char* baseuri,
            const char* basename,
            bool isRestriction = false) = 0;

        virtual SDO_API void setBaseType( 
            const SDOString& typeuri,
            const SDOString& typenam,
            const SDOString& baseuri,
            const SDOString& basename,
            bool isRestriction = false) = 0;


        /**
         * Generate a typesafe interface for a given data factory
         * test code.
         */

        virtual SDO_API bool generateInterface(const char* fileroot,
                              const char *factoryname) = 0;

        virtual SDO_API bool generateInterface(const SDOString& fileroot,
                              const SDOString& factoryname) = 0;



        /**
         *  DataFactory::setAlias sets an alternative name
         *
         * A Type may be known to the data factory by several different names.
         * This method adds a new name for an existing property.
         * 
         */

        virtual SDO_API void setAlias(const char* typeuri,
            const char* typenam,
            const char* alias) = 0;

        virtual SDO_API void setAlias(const SDOString& typeuri,
            const SDOString& typenam,
            const SDOString& alias) = 0;

        /**
         *  DataFactory::addPropertyToType adds properties
         *
         * The various addPropertyToType methods add a property to an 
         * existing type in the factory, specifying the name of the new property,
         * and the type of the new property - which must also be an existing type
         * in this factory.
         * -# The new property may be many-valued - so it will be a list of values.
         * -# The property may be read-only , and may not be altered by user code - 
         * However the value may be changed by data access service code.
         * -# The property may be containment.
         * The type of a property may be DataType, or DataObjectType (see Types).
         * If the property is a DataType, then the actual value of the property is held
         * within the data object containing the property.
         * If the property is a DataObjectType, it may be containment, or reference. 
         * Containment indicates that the value of the property is contained in the 
         * data object, whilst reference indicates that the property is only a pointer to
         * a value somewhere else in the data graph.
         * 
         */


        virtual SDO_API void addPropertyToType(const char* uri,
            const char* inTypeName,
            const char* propname,
            const char* propTypeUri,
            const char* propTypeName,
            bool isMany,
            bool isReadOnly ,
            bool isContainment ) = 0;

        virtual SDO_API void addPropertyToType(const SDOString& uri,
            const SDOString& inTypeName,
            const SDOString& propname,
            const SDOString& propTypeUri,
            const SDOString& propTypeName,
            bool  isMany  ,
            bool  isReadOnly ,
            bool  isContainment ) = 0;

        virtual SDO_API void addPropertyToType(const char* uri, 
            const char* inTypeName,
            const char* propname,
            const Type& propType,
            bool  isMany   ,
            bool  isReadOnly ,
            bool  isContainment ) = 0;
        
        virtual SDO_API void addPropertyToType(const SDOString& uri, 
            const SDOString& inTypeName,
            const SDOString& propname,
            const Type& propType,
            bool  isMany   ,
            bool  isReadOnly ,
            bool  isContainment ) = 0;

        virtual SDO_API void addPropertyToType(const Type& type, 
            const char* propname,
            const Type& propType,
            bool  isMany   ,
            bool  isReadOnly ,
            bool  isContainment ) = 0;
        
        virtual SDO_API void addPropertyToType(const Type& type, 
            const SDOString& propname,
            const Type& propType,
            bool  isMany   ,
            bool  isReadOnly ,
            bool  isContainment ) = 0;

        virtual SDO_API void addPropertyToType(const Type& type, 
            const char* propname,
            const char* propTypeUri, 
            const char* propTypeName,
            bool  isMany   ,
            bool  isReadOnly ,
            bool  isContainment ) = 0;

        virtual SDO_API void addPropertyToType(const Type& type, 
            const SDOString& propname,
            const SDOString& propTypeUri, 
            const SDOString& propTypeName,
            bool  isMany   ,
            bool  isReadOnly ,
            bool  isContainment ) = 0;

        virtual SDO_API void addPropertyToType(const char* uri, 
            const char* inTypeName,
            const char* propname,
            const char* propTypeUri, 
            const char* propTypeName,
            bool  isMany   = false) =0;
        
        virtual SDO_API void addPropertyToType(const SDOString& uri, 
            const SDOString& inTypeName,
            const SDOString& propname,
            const SDOString& propTypeUri, 
            const SDOString& propTypeName,
            bool  isMany   = false) =0;

        virtual SDO_API void addPropertyToType(const char* uri, 
            const char* inTypeName,
            const char* propname,
            const Type& propType,
            bool  isMany   = false) =0;
        
        virtual SDO_API void addPropertyToType(const SDOString& uri, 
            const SDOString& inTypeName,
            const SDOString& propname,
            const Type& propType,
            bool  isMany   = false) =0;

        virtual SDO_API void addPropertyToType(const Type& type, 
            const char* propname,
            const Type& propType,
            bool  isMany   = false) =0;
        
        virtual SDO_API void addPropertyToType(const Type& type, 
            const SDOString& propname,
            const Type& propType,
            bool  isMany   = false) =0;

        virtual SDO_API void addPropertyToType(const Type& type, 
            const char* propname,
            const char* propTypeUri, 
            const char* propTypeName,
            bool  isMany   = false) =0;

        virtual SDO_API void addPropertyToType(const Type& type, 
            const SDOString& propname,
            const SDOString& propTypeUri, 
            const SDOString& propTypeName,
            bool  isMany   = false) =0;

        /**
         *  DataFactory::setOpposite sets an opposite name
         *
         * NOT IMPLEMENTED.
         * 
         */

        virtual SDO_API void setOpposite( 
            const Type& type,
            const char* propName,
            const Type& oppositetype,
            const char* oppositePropName) = 0;
        
        virtual SDO_API void setOpposite( 
            const Type& type,
            const SDOString& propName,
            const Type& oppositetype,
            const SDOString& oppositePropName) = 0;
        
        /**
         *  DataFactory::setAlias sets a property alias name
         *
         * A property, like a type, may be known to the factory by several
         * names.
         */

        virtual SDO_API void setAlias(const char* typeuri, 
            const char* typname, 
            const char* propname,
            const char* alias) = 0;

        virtual SDO_API void setAlias(const SDOString& typeuri, 
            const SDOString& typname, 
            const SDOString& propname,
            const SDOString& alias) = 0;

        /**
         *  DataFactory::setPropertySubstitute sets a substitute for a property
         *
         * A property is of a specified type. However this method specifies another type
         * which will be accepted by the data factory at runtime as a substitute for this
         * property, and will be inserted into the parent data object as though it was
         * a value for this property. The substitute name is then used for this property
         * as a sort of alias.
         * names.
         */

        virtual SDO_API void setPropertySubstitute(
            const char* uri, 
            const char* inTypeName,
            const char* propname,
            const char* subname,
            const char* subTypeUri, 
            const char* subTypeName) = 0;

        virtual SDO_API void setPropertySubstitute(
            const SDOString& uri, 
            const SDOString& inTypeName,
            const SDOString& propname,
            const SDOString& subname,
            const SDOString& subTypeUri, 
            const SDOString& subTypeName) = 0;

        virtual SDO_API void setPropertySubstitute(
            const Type& containertype,
            const char* propname,
            const char* subname,
            const Type& subtype) = 0;

        virtual SDO_API void setPropertySubstitute(
            const Type& containertype,
            const SDOString& propname,
            const SDOString& subname,
            const Type& subtype) = 0;


        /**
         *  DataFactory::setDefault sets a default value for a property
         *
         * When querying the value of a property of a data object, that property
         * may be set - indicating that a value has been assigned, or unset - indicating that
         * no value has yet been assigned.
         * If there is no value assigned, then a default value may be returned for that
         * property. 
         * This method allows the data factory to define that default value. 
         * There is no default value for a DataObjectType. There are overloads for 
         * each of the primitive DataTypes.
         */

        virtual SDO_API void setDefault(
            const Type& t, 
            const char* propname, 
            bool b ) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const SDOString& propname, 
            bool b ) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const char* propname , 
            char c) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const SDOString& propname , 
            char c) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const char* propname , 
            wchar_t c) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const SDOString& propname , 
            wchar_t c) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const char* propname , 
            char* c) = 0;

// #pragma message( "GMW: Unimplemented method, writable string parameter" )
//         virtual SDO_API void setDefault(
//             const Type& t, 
//             const SDOString& propname , 
//             SDOString& c) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const char* propname , 
            short s) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const SDOString& propname , 
            short s) = 0;


#if __WORDSIZE !=64
        virtual SDO_API void setDefault(
            const Type& t, 
            const char* propname , 
            long l) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const SDOString& propname , 
            long l) = 0;
#endif

        virtual SDO_API void setDefault(
            const Type& t, 
            const char* propname , 
            int64_t i) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const SDOString& propname , 
            int64_t i) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const char* propname , 
            float f) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const SDOString& propname , 
            float f) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const char* propname , 
            long double d) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const SDOString& propname , 
            long double d) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const char* propname , 
            const wchar_t* c, 
            unsigned int len) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const SDOString& propname , 
            const wchar_t* c, 
            unsigned int len) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const char* propname , 
            const char* c, 
            unsigned int len) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const SDOString& propname , 
            const SDOString& c, 
            unsigned int len) = 0;


        virtual SDO_API void setDefault(
            const Type& t, 
            const char* propname , 
            const SDODate dat) = 0;

        virtual SDO_API void setDefault(
            const Type& t, 
            const SDOString& propname , 
            const SDODate dat) = 0;

        virtual SDO_API void setDefault(
            const char* typuri, 
            const char* typnam, 
            const char* propname, 
            bool b ) = 0;

        virtual SDO_API void setDefault(
            const SDOString& typuri, 
            const SDOString& typnam, 
            const SDOString& propname, 
            bool b ) = 0;

        virtual SDO_API void setDefault(
            const char* typuri, 
            const char* typnam, 
            const char* propname , 
            char c) = 0;

        virtual SDO_API void setDefault(
            const SDOString& typuri, 
            const SDOString& typnam, 
            const SDOString& propname , 
            char c) = 0;

        virtual SDO_API void setDefault(
            const char* typuri, 
            const char* typnam, 
            const char* propname ,
            wchar_t c) = 0;

        virtual SDO_API void setDefault(
            const SDOString& typuri, 
            const SDOString& typnam, 
            const SDOString& propname ,
            wchar_t c) = 0;

        virtual SDO_API void setDefault(
            const char* typuri, 
            const char* typnam, 
            const char* propname , 
            char* c) = 0;

// #pragma message( "GMW: Unimplemented method, writable string parameter" )
//         virtual SDO_API void setDefault(
//             const SDOString& typuri, 
//             const SDOString& typnam, 
//             const SDOString& propname , 
//             SDOString& c) = 0;

        virtual SDO_API void setDefault(
            const char* typuri, 
            const char* typnam, 
            const char* propname , 
            short s) = 0;

        virtual SDO_API void setDefault(
            const SDOString& typuri, 
            const SDOString& typnam, 
            const SDOString& propname , 
            short s) = 0;


#if __WORDSIZE !=64
        virtual SDO_API void setDefault(
            const char* typuri, 
            const char* typnam, 
            const char* propname ,
            long l) = 0;

        virtual SDO_API void setDefault(
            const SDOString& typuri, 
            const SDOString& typnam, 
            const SDOString& propname ,
            long l) = 0;

#endif

        virtual SDO_API void setDefault(
            const char* typuri, 
            const char* typnam, 
            const char* propname , 
            int64_t i) = 0;

        virtual SDO_API void setDefault(
            const SDOString& typuri, 
            const SDOString& typnam, 
            const SDOString& propname , 
            int64_t i) = 0;

        virtual SDO_API void setDefault(
            const char* typuri, 
            const char* typnam, 
            const char* propname , 
            float f) = 0;

        virtual SDO_API void setDefault(
            const SDOString& typuri, 
            const SDOString& typnam, 
            const SDOString& propname , 
            float f) = 0;

        virtual SDO_API void setDefault(
            const char* typuri, 
            const char* typnam, 
            const char* propname , 
            long double d) = 0;

        virtual SDO_API void setDefault(
            const SDOString& typuri, 
            const SDOString& typnam, 
            const SDOString& propname , 
            long double d) = 0;

        virtual SDO_API void setDefault(
            const char* typuri, 
            const char* typnam, 
            const char* propname ,
            const wchar_t* c, 
            unsigned int len) = 0;

        virtual SDO_API void setDefault(
            const SDOString& typuri, 
            const SDOString& typnam, 
            const SDOString& propname ,
            const wchar_t* c, 
            unsigned int len) = 0;

        virtual SDO_API void setDefault(
            const char* typuri, 
            const char* typnam, 
            const char* propname , 
            const char* c, 
            unsigned int len) = 0;

        virtual SDO_API void setDefault(
            const SDOString& typuri, 
            const SDOString& typnam, 
            const SDOString& propname , 
            const SDOString& c, 
            unsigned int len) = 0;

        virtual SDO_API void setDefault(
            const char* typuri, 
            const char* typnam, 
            const char* propname , 
            const SDODate dat) = 0;

        virtual SDO_API void setDefault(
            const SDOString& typuri, 
            const SDOString& typnam, 
            const SDOString& propname , 
            const SDODate dat) = 0;


        /**
         * DAS values are used by a DAS implementation
         */

        virtual SDO_API void setDASValue( 
            const Type& type,
            const char* name,
            DASValue* value) = 0;

        virtual SDO_API void setDASValue( 
            const Type& type,
            const SDOString& name,
            DASValue* value) = 0;

        virtual SDO_API void setDASValue(
            const char* typeuri,
            const char* typenam,
            const char* name,
            DASValue* value) = 0;

        virtual SDO_API void setDASValue(
            const SDOString& typeuri,
            const SDOString& typenam,
            const SDOString& name,
            DASValue* value) = 0;

        virtual SDO_API DASValue* getDASValue( 
            const Type& type,
            const char* name) const = 0;

        virtual SDO_API DASValue* getDASValue( 
            const Type& type,
            const SDOString& name) const = 0;

        virtual SDO_API DASValue* getDASValue(
            const char* typeuri,
            const char* typenam, 
            const char* name) const = 0;

        virtual SDO_API DASValue* getDASValue(
            const SDOString& typeuri,
            const SDOString& typenam, 
            const SDOString& name) const = 0;

        virtual SDO_API void setDASValue( 
            const Type& type,
            const char* propertyName,
            const char* name,
            DASValue* value) = 0;

        virtual SDO_API void setDASValue( 
            const Type& type,
            const SDOString& propertyName,
            const SDOString& name,
            DASValue* value) = 0;

        virtual SDO_API void setDASValue( 
            const char* typeuri,
            const char* typenam,
            const char* propertyName,
            const char* name,
            DASValue* value) = 0;

        virtual SDO_API void setDASValue( 
            const SDOString& typeuri,
            const SDOString& typenam,
            const SDOString& propertyName,
            const SDOString& name,
            DASValue* value) = 0;

        virtual SDO_API DASValue* getDASValue( 
            const Type& type,
            const char* propertyName,
            const char* name) const = 0;

        virtual SDO_API DASValue* getDASValue( 
            const Type& type,
            const SDOString& propertyName,
            const SDOString& name) const = 0;

        virtual SDO_API DASValue* getDASValue(
            const char* typeuri,
            const char* typenam,
            const char* propertyName, 
            const char* name) const = 0;

        virtual SDO_API DASValue* getDASValue(
            const SDOString& typeuri,
            const SDOString& typenam,
            const SDOString& propertyName, 
            const SDOString& name) const = 0;

        virtual void resolve() = 0;

    };
};
};
#endif //_DATAFACTORY_H_
