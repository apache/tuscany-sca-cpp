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

#ifndef _REFCOUNTINGOBJECT_H
#define _REFCOUNTINGOBJECT_H


#include "commonj/sdo/export.h"
#include <iostream>

namespace commonj {
    namespace sdo {

/**
 * RefcountingObject is the base class for all objects in SDO
 * These objects keep a count of references to themselves, then
 * free themselves when they are unused.
 */
    class RefCountingObject 
    {
        public:
        SDO_API RefCountingObject();
        SDO_API RefCountingObject(const RefCountingObject& rc);
        SDO_API RefCountingObject& operator=(const RefCountingObject& rc);
        SDO_API virtual ~RefCountingObject() = 0;
/**
 * Add to the reference count - a new pointer has been created.
 */
        SDO_API void addRef();
/**
 * Subtract from the the reference count - a reference has dropped.
 */
        SDO_API void releaseRef();

/**
 * Print contents to stream
 */
        SDO_API virtual std::ostream& printSelf(std::ostream &os);


        private:
        unsigned int refCount;
        static long allObs;
    };

    };
};

#endif
