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

#include "commonj/sdo/DataGraphImpl.h"
#include "commonj/sdo/DataObjectImpl.h"
#include "commonj/sdo/SDORuntimeException.h"



namespace commonj{
    namespace sdo {

/** 
 * DataGraphImpl implements DataGraph.
 * DataGraph holds a whole graph of data objects, starting from
 * a single root. This class is not currently used in the C++
 * implementation, but will be required by DAS implementations.
 */
        DataGraphImpl::~DataGraphImpl()
    {
    }

    DataGraphImpl::DataGraphImpl(DataFactoryPtr fac)
    {
        factory = fac;
        root = 0;
    }


    DataObjectPtr DataGraphImpl::getRootObject()
    {
        return root;
    }

    const char* DataGraphImpl::getRootElementName()
    {
        if (root == 0) return 0; 
        return root->getInstanceProperties()[0].getName();
    }

    void DataGraphImpl::setRootObject(DataObjectPtr dob)
    {
        if (root != 0)
        {
            SDO_THROW_EXCEPTION("setRootObject",SDOUnsupportedOperationException,
                "Root data object already exists");
        }
        root = dob;
    }


//     DataObjectPtr DataGraphImpl::createRootObject(const char* uri,
//                         const char* name)
//     {
//         if (root != 0)
//         {
//             SDO_THROW_EXCEPTION("createRootObject",SDOUnsupportedOperationException,
//                 "Root data object already exists");
//         }
//         root = factory->create(uri,name);
//         if (root->getInstanceProperties()[0].getType().isDataType())
//         {
//             SDO_THROW_EXCEPTION("createRootObject",SDOUnsupportedOperationException,
//                 "Root data object cannot have a datatype property at element 0");
//         }
//         return root;
//     }

    DataObjectPtr DataGraphImpl::createRootObject(const char* uri,
                        const char* name)
    {
      return createRootObject(SDOString(uri), SDOString(name));
    }

    DataObjectPtr DataGraphImpl::createRootObject(const SDOString& uri,
                        const SDOString& name)
    {
        if (root != 0)
        {
            SDO_THROW_EXCEPTION("createRootObject",SDOUnsupportedOperationException,
                "Root data object already exists");
        }
        root = factory->create(uri,name);
        if (root->getInstanceProperties()[0].getType().isDataType())
        {
            SDO_THROW_EXCEPTION("createRootObject",SDOUnsupportedOperationException,
                "Root data object cannot have a datatype property at element 0");
        }
        return root;
    }


    DataObjectPtr DataGraphImpl::createRootObject(const Type& t)
    {
        if (root != 0)
        {
            SDO_THROW_EXCEPTION("createRootObject",SDOUnsupportedOperationException,
                "Root data object already exists");
        }
        root = factory->create(t);
        if (root->getInstanceProperties()[0].getType().isDataType())
        {
            SDO_THROW_EXCEPTION("createRootObject",SDOUnsupportedOperationException,
                "Root data object cannot have a datatype property at element 0");
        }
        return root;
    }


    ChangeSummaryPtr DataGraphImpl::getChangeSummary()
    {
        if (!root) return 0;
        return root->getChangeSummary();
    }


    const Type& DataGraphImpl::getType(const char* uri,
                            const char* name)
    {
        return factory->getType(uri,name);
    }
    const Type& DataGraphImpl::getType(const SDOString& uri,
                            const SDOString& name)
    {
        return factory->getType(uri,name);
    }

    };
};
