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

#ifndef _DASTYPE_H_
#define _DASTYPE_H_

#include "commonj/sdo/disable_warn.h"


#include "commonj/sdo/Type.h"
#include "commonj/sdo/DASValues.h"
namespace commonj{
namespace sdo{
    
/** 
 * DASType is a class to provide data access services
 * with a more flexible Type
 */

class DASType : public Type, public DASValues
{
     public:

    virtual ~DASType();


    
    virtual void setOpen(bool set) = 0;
    virtual void setSequenced( bool set) = 0;
    virtual void setBaseType( const Type* type, bool isRestriction=false) = 0;

 

};
};
};
#endif //_DASDATAOBJECT_H_
