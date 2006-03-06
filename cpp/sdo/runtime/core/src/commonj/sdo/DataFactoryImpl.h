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

/* $Rev$ $Date: 2005/12/22 16:54:14 $ */

#ifndef _DataFactoryIMPL_H_
#define _DataFactoryIMPL_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/DataFactory.h"
#include "commonj/sdo/TypeImpl.h"
#include "commonj/sdo/RefCountingPointer.h"
#include "commonj/sdo/PropertyImpl.h"
#include "commonj/sdo/SDOXMLString.h"


#include <map>
using namespace std;


namespace commonj{
namespace sdo{

typedef map<string, PropertyImpl> propertyMap;

/**  
 *    DataFactoryImpl implements the abstract class DataFactory.
 *    DataFactoryImpl holds metadata and creates DataObjects.
 *
 *    The DataFactory holds properties and types loaded from XSD, or
 *    created using the DataFactory API.
 */
class  DataFactoryImpl  : public DataFactory
{
public:
    DataFactoryImpl();
    virtual ~DataFactoryImpl();

    DataFactoryImpl(const DataFactoryImpl& inmdg);
    DataFactoryImpl& operator=(const DataFactoryImpl& inmdg);

    virtual void addType(const char* uri, const char* inTypeName,
        bool isSeqenced = false, 
        bool isOpen = false,
        bool isAbstract = false,
        bool isDataType = false);


    virtual void addPropertyToType(const char* uri, 
                                   const char* inTypeName,
                                   const char* propname,
                                   const char* propTypeUri, 
                                   const char* propTypeName,
                                   bool isMany = false);


    virtual void addPropertyToType(const char* uri, 
                                   const char* inTypeName,
                                   const char* propname,
                                   const Type& propType,
                                   bool isMany = false);

    virtual void addPropertyToType(const Type& type, 
                                   const char* propname,
                                   const Type& propType,
                                   bool isMany = false);
    
    virtual void addPropertyToType(const Type& type, 
                                   const char* propname,
                                   const char* propTypeUri, 
                                   const char* propTypeName,
                                   bool isMany = false);

    virtual void addPropertyToType(const char* uri, 
                                   const char* inTypeName,
                                   const char* propname,
                                   const char* propTypeUri, 
                                   const char* propTypeName,
                                   bool     isMany ,
                                   bool  isReadOnly,
                                   bool  isContainment);

    virtual void addPropertyToType(const char* uri, 
                                   const char* inTypeName,
                                   const char* propname,
                                   const Type& propType,
                                   bool  isMany,
                                   bool  isReadOnly,
                                   bool  isContainment);
    
    virtual void addPropertyToType(const Type& type, 
                                   const char* propname,
                                   const Type& propType,
                                   bool  isMany,
                                   bool  isReadOnly,
                                   bool  isContainment);

    virtual void addPropertyToType(const Type& type, 
                                   const char* propname,
                                   const char* propTypeUri, 
                                   const char* propTypeName,
                                   bool  isMany,
                                   bool  isReadOnly,
                                   bool  isContainment);


    virtual void setBaseType( const Type& type,
                              const Type& base);

    virtual void setBaseType( const char* typeuri,
                              const char* typenam,
                              const char* baseuri,
                              const char* basename);

    virtual const Type& getType(const char* uri, const char* inTypeName) const;

    virtual TypeList getTypes() const;

    virtual DataObjectPtr create(const char* uri, const char* typeName) ;

    virtual DataObjectPtr create(const Type& type) ;

    virtual void setAlias(const char* typeuri,
                                  const char* typenam,
                                  const char* alias) ;

    virtual void setAlias(const char* typeuri, 
                                  const char* typname, 
                                  const char* propname,
                                  const char* alias) ;

    virtual void setOpposite (const Type& type,
                                  const char* propname,
                              const Type& opptype,
                              const char* opppropname) ;


    virtual void setPropertySubstitute(
            const char* uri, 
            const char* inTypeName,
            const char* propname,
            const char* subname,
            const char* subTypeUri, 
            const char* subTypeName);
        

    virtual void setPropertySubstitute(
            const Type& containertype,
            const char* propname,
            const char* subname,
            const Type& subtype);

    ////////////////////////////////////////////////////////
    virtual void setDefault(
        const Type& t, 
        const char* propname, 
        bool b ) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        char c) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        wchar_t c) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        char* c) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        short s) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        long l) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        int64_t i) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        float f) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname ,
        long double d) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        const SDODate d) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        const wchar_t* c, 
        unsigned int len) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        const char* c, 
        unsigned int len) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname, 
        bool b ) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        char c) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        wchar_t c) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname ,
        char* c) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        short s) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        long l) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname ,
        int64_t i) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        float f) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        long double d) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        const SDODate d) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        const wchar_t* c, 
        unsigned int len) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        const char* c, 
        unsigned int len) ;

    virtual void setDASValue( 
            const Type& type,
                const char* name,
                DASValue* value);

    virtual void setDASValue(
                const char* typeuri,
                const char* typenam,
                const char* name,
                DASValue* value);

    virtual DASValue* getDASValue(
                const Type& type,
                const char* name) const;

    virtual DASValue* getDASValue(
                const char* typeuri,
                const char* typenam, 
                const char* name) const;

    virtual void setDASValue( 
                const Type& type,
                const char* propertyName,
                const char* name,
                DASValue* value);

    virtual void setDASValue( 
                const char* typeuri,
                const char* typenam,
                const char* propertyName,
                const char* name,
                DASValue* value);

    virtual DASValue* getDASValue( 
                const Type& type,
                const char* propertyName,
                const char* name) const;

    virtual DASValue* getDASValue(
                const char* typeuri,
                const char* typenam,
                const char* propertyName, 
                const char* name) const;

    virtual    void resolve();

    const Type*    findType  (const char* uri, const char* inTypeName) const;
    const TypeImpl*    findTypeImpl  (const char* uri, const char* inTypeName) const;

    void addOpenProperty(const PropertyImpl& prop);
    void removeOpenProperty(const char* name);
    const propertyMap& getOpenProperties();
    virtual const TypeImpl& getTypeImpl(const char* uri, const char* inTypeName) const;
    virtual const char* getRootElementName() const;
    virtual void setRootElementName(const char* ren);


private:
    typedef map<string, TypeImpl*> TYPES_MAP;
    TYPES_MAP    types;

    char * rootElementName;

    propertyMap openProperties;

    // Need to validate and 'lock' the data model for base types to
    // work properly.

    bool isResolved;
    
    bool recursiveCheck(TypeImpl* cs, TypeImpl* t);
    bool checkForValidChangeSummary(TypeImpl* t);

    typedef vector<TypeImpl*> TYPES_VECTOR;
    TYPES_VECTOR cstypes;
    
    char*        getFullTypeName(const char* uri, const char* inTypeName) const;
    char*        getAliasTypeName(const char* uri, const char* inTypeName) const;

    void        copyTypes(const DataFactoryImpl& inmdg);
};
};
};
#endif // _DataFactoryIMPL_H_
