/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 *   
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/* $Rev$ $Date$ */

#ifndef _DATAGRAPHIMPL_H_
#define _DATAGRAPHIMPL_H_
#include "commonj/sdo/export.h"
#include "commonj/sdo/DataGraph.h"
#include "commonj/sdo/disable_warn.h"


namespace commonj{
namespace sdo{
    
/** 
 * DataGraphImpl implements the abstract class DataGraph.
 * DataGraph holds a whole graph of data objects, starting from
 * a single root. This class is not currently used in the C++
 * implementation, but will be required by DAS implementations.
 */

class DataGraphImpl : public DataGraph
{
     public:
    virtual ~DataGraphImpl();

    SDO_API DataGraphImpl(DataFactoryPtr fac);

    /**
     * Sets the root data object.
     * 
     */

    virtual SDO_API void setRootObject(DataObjectPtr dob);
 
    /**
     * Gets the root data object.
     *
     */
    
    virtual DataObjectPtr getRootObject();

    /**
     * Gets the name of the root data element (from the XML
     * description).
     *
     */

    virtual const char* getRootElementName();

    /**
     * Creates the root data object - for use by DAS implementations
     *
     */

    virtual DataObjectPtr createRootObject(const char* uri,
                        const char* name);
    virtual DataObjectPtr createRootObject(const SDOString& uri,
                        const SDOString& name);

    /**
     * Creates the root data object - for use by DAS implementations
     *
     */


    virtual DataObjectPtr createRootObject(const Type& t);

    /**
     *
     * Gets the change summary if there is one.
     */

    virtual ChangeSummaryPtr getChangeSummary();

    /**
     *
     * Gets a type from the data factory.
     */

    virtual const Type& getType(const char* uri,
                            const char* name);
    virtual const Type& getType(const SDOString& uri,
                            const SDOString& name);

    private:

        DataFactoryPtr factory;
        DataObjectPtr root;

};
};
};
 
#endif //_DATAGRAPHIMPL_H_
