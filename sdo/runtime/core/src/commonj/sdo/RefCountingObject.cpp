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

#include "commonj/sdo/RefCountingObject.h"
#include "commonj/sdo/Logging.h"

#include <iostream>
using namespace std;

namespace commonj{
namespace sdo{

long RefCountingObject::allObs = 0;

RefCountingObject::RefCountingObject()
: refCount(0)
{
    LOGINFO_2(HIGHVOLUME,"RCO:CREATE:%p Count:%ld",this, ++allObs);
}

RefCountingObject::RefCountingObject(const RefCountingObject& rc)
: refCount(0)
{
    LOGINFO_2(HIGHVOLUME,"RCO:COPCON:%p Count:%ld",this,++allObs);
}

RefCountingObject& RefCountingObject::operator=(const RefCountingObject& rc)
{
    return *this;
}

RefCountingObject::~RefCountingObject()
{
    LOGINFO_2(HIGHVOLUME,"RCO:DELETE:%p Count:%ld",this, --allObs);
    //
    //if (allObs < 0) 
    //    LOGINFO(HIGHVOLUME,"RCO:More objects deleted than created");
    //if (allObs == 0) 
    //    LOGINFO(HIGHVOLUME,"RCO: All data objects deleted");;
}

void RefCountingObject::addRef()

{
    
    ++refCount;
     LOGINFO_2(HIGHVOLUME,"RCO:ADDREF:%p:%ld",this,refCount);
}

void RefCountingObject::releaseRef()
{
    LOGINFO_2(HIGHVOLUME,"RCO:DECREF:%p:%ld",this,refCount-1);
    if (--refCount == 0) delete this;
}


};
};
