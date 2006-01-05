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

#ifndef _EQUALITYHELPER_H_
#define _EQUALITYHELPER_H_

#include "commonj/sdo/RefCountingPointer.h"



namespace commonj{
namespace sdo{

/**
 * EqualityHelper provides static helper methods for equals.
 * equalsShallow compares the DataType properties of the 
 * two data objects.
 * equals performs a check on all the memebers and verifies that
 * the same data objects are children.
 */

    class EqualityHelper
{
    public:

    /**
    *  Shallow compare of DataObjects
    *  
    *    Compares two DataObjects for each property where
    *        property.getType().isDataObjectType() is false.
    *    Compares dataObject1.get(property).equals(dataObject2.get(property))
    *    Properties where property.getType().isDataObjectType() is true
    *      are not compared.
    *    DataObjects must have the same type
    *      dataObject1.getType() == dataObject2.getType()
    *  Returns true if all values of all compared properties are equal.
    *  @param dataObject1 DataObject to be compared
    *  @param dataObject2 DataObject to be compared
    *  @return true if equal 
    */
    static SDO_API bool equalShallow(DataObjectPtr dataObject1, DataObjectPtr dataObject2);
    
    /**
    *  Deep compare of DataObjects
    *    Compares dataObject1 and dataObject2 and all their contained
    *      DataObjects (property.isContainment() is true) recursively.
    *    Values of properties are comapred as in isEqualShallow,
    *      and values of properties where 
    *        property.getType().isDataObjectType() is true
    *      are compared where each value copied must be a corresponding
    *      DataObject contained by the source dataObject.
    *    The dataObject trees must be closed. 
    *    If any DataObject referenced in either tree is not part of the
    *      containment trees an IllegalArgumentException is thrown.
    *    If a ChangeSummary is part of the tree they must contain an
    *      identical summary for their respective trees.
    *   
    *  @param dataObject1 DataObject to be compared
    *  @param dataObject2 DataObject to be compared
    *  @return true if all compared aspects of all compared objects are equal 
    *  @throws IllegalArgumentException if any referenced DataObject
    *    is not part of the containment trees.
    */
    static SDO_API bool equal(DataObjectPtr dataObject1, DataObjectPtr dataObject2);

    private: 

    static bool internalEqual(DataObjectPtr dataObject1,
                                                DataObjectPtr dataObject2,
                                            bool fullEqual);

    static bool comparelist(DataObjectList& to, DataObjectList& from, Type::Types t);

    static bool compareitem(DataObjectPtr to, DataObjectPtr from, const Property& p);


   
};
};
};
#endif //_EQUALITYHELPER_H_
