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

#ifndef _SDO_SDODATE_H_
#define _SDO_SDODATE_H_


#include "commonj/sdo/export.h"
#include "time.h"



 
namespace commonj{
namespace sdo{


    ///////////////////////////////////////////////////////////////////////////
    // A representation of the type of a date.
    ///////////////////////////////////////////////////////////////////////////

/**
 * SDODate - a means of hiding the time_t.
 * The SDO API in several places uses method overloads to allow the
 * same method to set many types of data. As time_t and long are 
 * sometimes synonymous, it was not possible to have a time_t and a
 * long method, so SDODate simply wraps the time_t.
 * SDODate also allows modifcation of the implementation, as time_t is
 * only a short-term solution - better date/time handling is required.
 */

class SDODate
{

public:


    virtual SDO_API ~SDODate();

    SDO_API SDODate(time_t inval);

    /**
     * Hand out the date as a time_t
     */
    virtual SDO_API const time_t getTime() const;

    /**
     * Format the date as a string
     */

    virtual const char* ascTime(void) const;
  

private:
    time_t value;
};

};
};
#endif //_SDO_SDODATE_H_

