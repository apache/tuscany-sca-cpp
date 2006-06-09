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

/* $Rev$ $Date: 2006/03/16 12:21:39 $ */

#ifndef _SDOUTILS_H_
#define _SDOUTILS_H_

#include "commonj/sdo/DataObject.h"
#include <map>
#include <string>

namespace commonj
{
    namespace sdo
    {
        

    /**
     * SDOUtils will provide utility functions.
     * The class currently holds a method to print data objects.
     */

    class SDOUtils
        {
            
        public:
            
            static SDO_API void printDataObject(std::ostream& out, DataObjectPtr d);                
            static SDO_API const char* SDOToXSD(const char* sdoname);
            static SDO_API const char*  XSDToSDO(const char* xsdname);
            
        private:

            static bool populate();
            static bool populated;

            static void printDataObject(std::ostream& out, DataObjectPtr d, unsigned int incr);                
            static void printTabs(std::ostream& out, unsigned int incr);

            static std::map<std::string,std::string> XsdToSdo;
            static std::map<std::string,std::string> SdoToXsd;

        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_SDOUTILS_H_
