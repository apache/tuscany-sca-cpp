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

#ifndef _DATAGRAPH_H_
#define _DATAGRAPH_H_

#include "commonj/sdo/RefCountingObject.h"
#include "commonj/sdo/Type.h"
#include "commonj/sdo/export.h"
#include "commonj/sdo/DataObject.h"


namespace commonj{
namespace sdo{
    
/** 
 * DataGraph holds a whole graph of data objects, starting from
 * a single root. This class is not currently used in the C++
 * implementation, but will be required by DAS implementations.
 */

class DataGraph : public RefCountingObject
{
     public:
        virtual ~DataGraph();
    
    /**
     *
     * Gets the root data object of this graph.
     */

 
    virtual SDO_API DataObjectPtr getRootObject() = 0;
    
   /**
     *
     * gets the name of the element from which the root
     * object was loaded (The XML definition).
     */
    virtual SDO_API const char*  getRootElementName() = 0;

   /**
     * Sets the Root data object.
     *
     */
    virtual SDO_API void setRootObject(DataObjectPtr dob) = 0;

   /**
     * Creates a root data object - used by DAS implementations.
     *
     */

    virtual SDO_API DataObjectPtr createRootObject(const char* uri,
                        const char* name) = 0;
    virtual SDO_API DataObjectPtr createRootObject(const SDOString& uri,
                        const SDOString& name) = 0;

   /**
     * Creates a root data object - used by DAS implementations.
     *
     */

    virtual SDO_API DataObjectPtr createRootObject(const Type& t) = 0;

   /**
     * Gets the change summary if there is one.
     *
     */

    virtual SDO_API ChangeSummaryPtr getChangeSummary() = 0;

   /**
     * Gets a type from the data factory.
     *
     */

    virtual SDO_API const Type& getType(const char* uri,
                            const char* name) = 0;
    virtual SDO_API const Type& getType(const SDOString& uri,
                            const SDOString& name) = 0;


};
};
};
 
#endif //_DATAGRAPH_H_
