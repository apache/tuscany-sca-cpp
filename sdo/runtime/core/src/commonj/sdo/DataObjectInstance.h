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

#ifndef commonj_sdo_DataObjectInstance_h
#define commonj_sdo_DataObjectInstance_h


#include "commonj/sdo/export.h"
#include "commonj/sdo/SDO.h"
using commonj::sdo::DataObjectPtr;
using commonj::sdo::DataObject;


namespace commonj
{
    namespace sdo
    {
/**
 * DataObjectInstance is a class which allows DataObjects to be considered
 * as instances rather than data object pointers.
 */
        class DataObjectInstance  
        {
        
        public:
            SDO_API DataObjectInstance();
            SDO_API virtual ~DataObjectInstance();

            SDO_API DataObjectInstance(const DataObjectPtr& theDO);
            SDO_API DataObjectInstance(const DataObjectInstance&);

            SDO_API DataObjectInstance& operator=(const DataObjectInstance&);
            SDO_API bool operator!() {return (!dataObject);}
            SDO_API operator bool() {return !!dataObject;}

            SDO_API DataObject* operator->() {return dataObject;}

            SDO_API DataObjectPtr getDataObject() {return dataObject;}
            SDO_API operator DataObjectPtr() {return dataObject;}
        private:
            DataObjectPtr dataObject;
        };
    } // End namespace sdo
} // End namespace commonj

#endif // commonj_sdo_DataObjectInstance_h
