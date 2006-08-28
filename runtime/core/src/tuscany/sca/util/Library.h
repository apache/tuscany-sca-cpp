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

/* $Rev$ $Date: 2005/12/22 11:33:21 $ */

#ifndef tuscany_sca_util_library_h
#define tuscany_sca_util_library_h
#include "osoa/sca/export.h"

#if defined(WIN32)  || defined (_WINDOWS)
#include <windows.h> 
#else
#include <unistd.h>
#include <dlfcn.h>
#endif

#include <string>
using std::string;

namespace tuscany
{
    namespace sca
    {
        /**
         * Information about shared libraries and methods to 
         * access these shared libraries.
         */
        class SCA_API Library
        {
        public:
            Library();

            /** 
             * Constructor. Will load the library.
             * @param libraryName Fully qualified name of the library.
             */
            Library(const string& libraryName);

            /**
             * Destructor. Will unload the library.
             */ 
            virtual ~Library();

            Library(const Library& lib);
            Library& operator=(const Library& lib);
            
            /**
             * Find an externalized symbol in the library.
             * @param symbol The name of the symbol to be found.
             * @return The pointer to the symbol if found, otherwise 0.
             */ 
            void* getSymbol(const string& symbol);
        private:
            /**
             * Name of the library.
             */ 
            string name;

            /**
             * Handle to the loaded library.
             */ 
#if defined(WIN32)  || defined (_WINDOWS)
            HINSTANCE hDLL;
#else
            void* hDLL;
#endif

            /**
             * Load the library.
             */
            void load();

            /**
             * Unload the library, if successfully loaded.
             */ 
            void unload();
                        
        };
                
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_util_library_h
