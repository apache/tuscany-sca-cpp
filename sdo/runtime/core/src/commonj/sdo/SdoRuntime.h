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

#ifndef SDO_RUNTIME_H
#define SDO_RUNTIME_H
#include "commonj/sdo/export.h"

namespace commonj{
namespace sdo{

/**
 * SDORuntime gives information about the version of the library.
 */

class SdoRuntime 
{
     public:

    virtual ~SdoRuntime();

    /**
     * The version as a string major:minor:fix
     */

    static SDO_API const char* getVersion();

    /**
     * The major version as an integer.
     */

    static SDO_API const unsigned int getMajor();

    /**
     * The minor version as an integer.
     */

    static SDO_API const unsigned int getMinor();

    /**
     * The fix level as an integer.
     */

    static SDO_API const unsigned int getFix();


    private:
        static const unsigned int major;
        static const unsigned int minor;
        static const unsigned int fix;
        static char* version;

};
};
};


#endif

