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

#include "commonj/sdo/SdoRuntime.h"
#include <iostream>
#include <stdio.h>

namespace commonj{
namespace sdo{


const unsigned int  SdoRuntime::major = 0;
const unsigned int  SdoRuntime::minor = 9;
const unsigned int  SdoRuntime::fix =4;
char* SdoRuntime::version = 0;



const char* SdoRuntime::getVersion()
{
    if (SdoRuntime::version == 0) 
    {
    SdoRuntime::version = new char[11];
    sprintf(SdoRuntime::version,"%02d:%02d:%04d", 
        SdoRuntime::major, SdoRuntime::minor, SdoRuntime::fix);
    }
    return (const char *)SdoRuntime::version;

}

const unsigned int SdoRuntime::getMajor()
{
    return SdoRuntime::major;
}

const unsigned int SdoRuntime::getMinor()
{
    return SdoRuntime::minor;
}

const unsigned int SdoRuntime::getFix()
{
    return SdoRuntime::fix;
}

};
};

