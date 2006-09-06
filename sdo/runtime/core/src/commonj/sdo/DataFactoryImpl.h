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

/* $Rev$ $Date: 2006/04/13 08:35:04 $ */

#ifndef _DataFactoryIMPL_H_
#define _DataFactoryIMPL_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/DataFactory.h"
#include "commonj/sdo/TypeImpl.h"
#include "commonj/sdo/RefCountingPointer.h"
#include "commonj/sdo/PropertyImpl.h"
#include "commonj/sdo/SDOXMLString.h"


#include <map>



namespace commonj{
namespace sdo{

    typedef std::map<std::string, PropertyImpl> propertyMap;

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
        bool isDataType = false
        );

    virtual void addType(const SDOString& uri, const SDOString& inTypeName,
        bool isSeqenced = false, 
        bool isOpen = false,
        bool isAbstract = false,
        bool isDataType = false
        );

    virtual void addType(const char* uri, const char* inTypeName,
        bool isSeqenced , 
        bool isOpen ,
        bool isAbstract ,
        bool isDataType ,
        bool isFromList );

    virtual void addType(const SDOString& uri, const SDOString& inTypeName,
        bool isSeqenced , 
        bool isOpen ,
        bool isAbstract ,
        bool isDataType ,
        bool isFromList );

    virtual void addPropertyToType(const char* uri, 
                                   const char* inTypeName,
                                   const char* propname,
                                   const char* propTypeUri, 
                                   const char* propTypeName,
                                   bool isMany = false);

    virtual void addPropertyToType(const SDOString& uri, 
                                   const SDOString& inTypeName,
                                   const SDOString& propname,
                                   const SDOString& propTypeUri, 
                                   const SDOString& propTypeName,
                                   bool isMany = false);

    virtual void addPropertyToType(const char* uri, 
                                   const char* inTypeName,
                                   const char* propname,
                                   const Type& propType,
                                   bool isMany = false);

    virtual void addPropertyToType(const SDOString& uri, 
                                   const SDOString& inTypeName,
                                   const SDOString& propname,
                                   const Type& propType,
                                   bool isMany = false);

    virtual void addPropertyToType(const Type& type, 
                                   const char* propname,
                                   const Type& propType,
                                   bool isMany = false);
    
    virtual void addPropertyToType(const Type& type, 
                                   const SDOString& propname,
                                   const Type& propType,
                                   bool isMany = false);

    virtual void addPropertyToType(const Type& type, 
                                   const char* propname,
                                   const char* propTypeUri, 
                                   const char* propTypeName,
                                   bool isMany = false);

    virtual void addPropertyToType(const Type& type, 
                                   const SDOString& propname,
                                   const SDOString& propTypeUri, 
                                   const SDOString& propTypeName,
                                   bool isMany = false);

    virtual void addPropertyToType(const char* uri, 
                                   const char* inTypeName,
                                   const char* propname,
                                   const char* propTypeUri, 
                                   const char* propTypeName,
                                   bool     isMany ,
                                   bool  isReadOnly,
                                   bool  isContainment);

    virtual void addPropertyToType(const SDOString& uri, 
                                   const SDOString& inTypeName,
                                   const SDOString& propname,
                                   const SDOString& propTypeUri, 
                                   const SDOString& propTypeName,
                                   bool isMany ,
                                   bool  isReadOnly,
                                   bool  isContainment);

    virtual void addPropertyToType(const char* uri, 
                                   const char* inTypeName,
                                   const char* propname,
                                   const Type& propType,
                                   bool  isMany,
                                   bool  isReadOnly,
                                   bool  isContainment);
    
    virtual void addPropertyToType(const SDOString& uri, 
                                   const SDOString& inTypeName,
                                   const SDOString& propname,
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
                                   const SDOString& propname,
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

    virtual void addPropertyToType(const Type& type, 
                                   const SDOString& propname,
                                   const SDOString& propTypeUri, 
                                   const SDOString& propTypeName,
                                   bool  isMany,
                                   bool  isReadOnly,
                                   bool  isContainment);


    virtual void setBaseType( const Type& type,
                              const Type& base,
                              bool isRestriction = false);

    virtual void setBaseType( const char* typeuri,
                              const char* typenam,
                              const char* baseuri,
                              const char* basename,
                              bool isRestriction = false);

    virtual void setBaseType( const SDOString& typeuri,
                              const SDOString& typenam,
                              const SDOString& baseuri,
                              const SDOString& basename,
                              bool isRestriction = false);

    virtual const Type& getType(const char* uri, const char* inTypeName) const;

    virtual const Type& getType(const SDOString& uri, const SDOString& inTypeName) const;

    virtual TypeList getTypes() const;

    virtual DataObjectPtr create(const char* uri, const char* typeName) ;

    virtual DataObjectPtr create(const SDOString& uri, const SDOString& typeName) ;

    virtual DataObjectPtr create(const Type& type) ;

    virtual void setAlias(const char* typeuri,
                                  const char* typenam,
                                  const char* alias) ;

    virtual void setAlias(const SDOString& typeuri,
                                  const SDOString& typenam,
                                  const SDOString& alias) ;

    virtual void setAlias(const char* typeuri, 
                                  const char* typname, 
                                  const char* propname,
                                  const char* alias) ;

	virtual void setAlias(const SDOString& typeuri, 
                                  const SDOString& typname, 
                                  const SDOString& propname,
                                  const SDOString& alias) ;

    virtual void setOpposite (const Type& type,
                                  const char* propname,
                              const Type& opptype,
                              const char* opppropname) ;

    virtual void setOpposite (const Type& type,
                              const SDOString& propname,
                              const Type& opptype,
                              const SDOString& opppropname) ;


  virtual void setPropertySubstitute(
            const char* uri, 
            const char* inTypeName,
            const char* propname,
            const char* subname,
            const char* subTypeUri, 
            const char* subTypeName);
        
  virtual void setPropertySubstitute(
            const SDOString& uri, 
            const SDOString& inTypeName,
            const SDOString& propname,
            const SDOString& subname,
            const SDOString& subTypeUri, 
            const SDOString& subTypeName);

    virtual void setPropertySubstitute(
            const Type& containertype,
            const char* propname,
            const char* subname,
            const Type& subtype);

    virtual void setPropertySubstitute(
            const Type& containertype,
            const SDOString& propname,
            const SDOString& subname,
            const Type& subtype);

    ////////////////////////////////////////////////////////
    virtual void setDefault(
        const Type& t, 
        const char* propname, 
        bool b ) ;

    virtual void setDefault(
        const Type& t, 
        const SDOString& propname, 
        bool b ) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        char c) ;

    virtual void setDefault(
        const Type& t, 
        const SDOString& propname , 
        char c) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        wchar_t c) ;

    virtual void setDefault(
        const Type& t, 
        const SDOString& propname , 
        wchar_t c) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        char* c) ;

    virtual void setDefault(
        const Type& t, 
        const SDOString& propname , 
        SDOString& outstr);

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        short s) ;

    virtual void setDefault(
        const Type& t, 
        const SDOString& propname , 
        short s) ;



#if __WORDSIZE !=64
    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        long l) ;

    virtual void setDefault(
        const Type& t, 
        const SDOString& propname , 
        long l) ;

#endif

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        int64_t i) ;

    virtual void setDefault(
        const Type& t, 
        const SDOString& propname , 
        int64_t i) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        float f) ;

    virtual void setDefault(
        const Type& t, 
        const SDOString& propname , 
        float f) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname ,
        long double d) ;

    virtual void setDefault(
        const Type& t, 
        const SDOString& propname ,
        long double d) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        const SDODate d) ;

    virtual void setDefault(
        const Type& t, 
        const SDOString& propname , 
        const SDODate d) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        const wchar_t* c, 
        unsigned int len) ;

    virtual void setDefault(
        const Type& t, 
        const SDOString& propname , 
        const wchar_t* c, 
        unsigned int len) ;

    virtual void setDefault(
        const Type& t, 
        const char* propname , 
        const char* c, 
        unsigned int len) ;

    virtual void setDefault(
        const Type& t, 
        const SDOString& propname , 
        const SDOString& c, 
        unsigned int len) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname, 
        bool b ) ;

    virtual void setDefault(
        const SDOString& typuri, 
        const SDOString& typnam, 
        const SDOString& propname, 
        bool b ) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        char c) ;

    virtual void setDefault(
        const SDOString& typuri, 
        const SDOString& typnam, 
        const SDOString& propname , 
        char c) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        wchar_t c) ;

    virtual void setDefault(
        const SDOString& typuri, 
        const SDOString& typnam, 
        const SDOString& propname , 
        wchar_t c) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname ,
        char* c) ;

    virtual void setDefault(
        const SDOString& typuri, 
        const SDOString& typnam, 
        const SDOString& propname ,
        SDOString& c) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        short s) ;

    virtual void setDefault(
        const SDOString& typuri, 
        const SDOString& typnam, 
        const SDOString& propname , 
        short s) ;

#if __WORDSIZE !=64
    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        long l) ;

    virtual void setDefault(
        const SDOString& typuri, 
        const SDOString& typnam, 
        const SDOString& propname , 
        long l) ;

#endif

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname ,
        int64_t i) ;

    virtual void setDefault(
        const SDOString& typuri, 
        const SDOString& typnam, 
        const SDOString& propname ,
        int64_t i) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        float f) ;

    virtual void setDefault(
        const SDOString& typuri, 
        const SDOString& typnam, 
        const SDOString& propname , 
        float f) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        long double d) ;

    virtual void setDefault(
        const SDOString& typuri, 
        const SDOString& typnam, 
        const SDOString& propname , 
        long double d) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        const SDODate d) ;

    virtual void setDefault(
        const SDOString& typuri, 
        const SDOString& typnam, 
        const SDOString& propname , 
        const SDODate d) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        const wchar_t* c, 
        unsigned int len) ;

    virtual void setDefault(
        const SDOString& typuri, 
        const SDOString& typnam, 
        const SDOString& propname , 
        const wchar_t* c, 
        unsigned int len) ;

    virtual void setDefault(
        const char* typuri, 
        const char* typnam, 
        const char* propname , 
        const char* c, 
        unsigned int len) ;

    virtual void setDefault(
        const SDOString& typuri, 
        const SDOString& typnam, 
        const SDOString& propname , 
        const SDOString& c, 
        unsigned int len) ;

    virtual void setDASValue( 
            const Type& type,
                const char* name,
                DASValue* value);

    virtual void setDASValue( 
            const Type& type,
            const SDOString& name,
            DASValue* value);

    virtual void setDASValue(
                const char* typeuri,
                const char* typenam,
                const char* name,
                DASValue* value);

    virtual void setDASValue(
                const SDOString& typeuri,
                const SDOString& typenam,
                const SDOString& name,
                DASValue* value);

    virtual DASValue* getDASValue(
                const Type& type,
                const char* name) const;

    virtual DASValue* getDASValue(
                const Type& type,
                const SDOString& name) const;

    virtual DASValue* getDASValue(
                const char* typeuri,
                const char* typenam, 
                const char* name) const;

    virtual DASValue* getDASValue(
                const SDOString& typeuri,
                const SDOString& typenam, 
                const SDOString& name) const;

    virtual void setDASValue( 
                const Type& type,
                const char* propertyName,
                const char* name,
                DASValue* value);

    virtual void setDASValue( 
                const Type& type,
                const SDOString& propertyName,
                const SDOString& name,
                DASValue* value);

    virtual void setDASValue( 
                const char* typeuri,
                const char* typenam,
                const char* propertyName,
                const char* name,
                DASValue* value);

    virtual void setDASValue( 
                const SDOString& typeuri,
                const SDOString& typenam,
                const SDOString& propertyName,
                const SDOString& name,
                DASValue* value);

    virtual DASValue* getDASValue( 
                const Type& type,
                const char* propertyName,
                const char* name) const;

    virtual DASValue* getDASValue( 
                const Type& type,
                const SDOString& propertyName,
                const SDOString& name) const;

    virtual DASValue* getDASValue(
                const char* typeuri,
                const char* typenam,
                const char* propertyName, 
                const char* name) const;

    virtual DASValue* getDASValue(
                const SDOString& typeuri,
                const SDOString& typenam,
                const SDOString& propertyName, 
                const SDOString& name) const;

    virtual    void resolve();

// TODO: Remove char* form of method since it is not exposed to users
    const Type*    findType  (const char* uri, const char* inTypeName) const;
    const Type* findType(const SDOString uri, const SDOString inTypeName) const;

// TODO: Remove char* form of method since it is not exposed to users
    const TypeImpl*    findTypeImpl  (const char* uri, const char* inTypeName) const;
    const TypeImpl* findTypeImpl(const SDOString& uri, const SDOString& inTypeName) const;

    void addOpenProperty(const PropertyImpl& prop);
// TODO: Remove char* form of method since it is not exposed to users
    void removeOpenProperty(const char* name);
    void removeOpenProperty(const SDOString& name);
    const propertyMap& getOpenProperties();
// TODO: Remove char* form of method since it is not exposed to users
    virtual const TypeImpl& getTypeImpl(const char* uri, const char* inTypeName) const;
    virtual const TypeImpl& getTypeImpl(const SDOString& uri, const SDOString& inTypeName) const;
    virtual const SDOString& getRootElementName() const;
// TODO: Remove char* form of method since it is not exposed to users
    virtual void setRootElementName(const char* ren);
    virtual void setRootElementName(const SDOString& ren);
    virtual bool checkType(const Type& t);
    virtual bool isCompatible(DataFactory* df, DataObject* d);
    virtual bool compareTypes(const TypeImpl* t1, const Type& t2);

   virtual bool generateInterface(const char* fileroot, const char* factoryname);

   virtual bool generateInterface(const SDOString& fileroot, const SDOString& factoryname);

    virtual std::ostream& printSelf(std::ostream &os);

private:
    typedef std::map<std::string, TypeImpl*> TYPES_MAP;
    TYPES_MAP    types;

    std::vector<DataFactory*> compatibleFactories;

	SDOString rootElementName;

    propertyMap openProperties;

    // Need to validate and 'lock' the data model for base types to
    // work properly.

    bool isResolved;
    
    bool recursiveCheck(TypeImpl* cs, TypeImpl* t);
    bool checkForValidChangeSummary(TypeImpl* t);

    typedef std::vector<TypeImpl*> TYPES_VECTOR;
    TYPES_VECTOR cstypes;
    
// TODO: Remove char* form of method since it is not exposed to users
    char*        getFullTypeName(const char* uri, const char* inTypeName) const;
    char*        getFullTypeName(const SDOString& uri, const SDOString& inTypeName) const;
// TODO: Remove char* form of method since it is not exposed to users
    char*        getAliasTypeName(const char* uri, const char* inTypeName) const;
    char*        getAliasTypeName(const SDOString& uri, const SDOString& inTypeName) const;

    void        copyTypes(const DataFactoryImpl& inmdg);
};
};
};
#endif // _DataFactoryIMPL_H_
