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
#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif

#include "tuscany/sca/util/Library.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/util/Logging.h"

namespace tuscany
{
    namespace sca
    {
        Library::Library()
            : hDLL(NULL)
        {
        }
        
        Library::Library(const string& libraryName)
            : name(libraryName), hDLL(NULL)
        {
            LOGINFO_1(3, "Library::construcor : %s", name.c_str()); 
            load();
        }

        Library::Library(const Library& lib)
            : name(lib.name), hDLL(NULL)
        {
            LOGINFO_1(3, "Library::copy constructor : %s", name.c_str()); 
            if (lib.hDLL)
            {
                load();
            }
        }
        
        Library& Library::operator=(const Library& lib)
        {
            LOGINFO_1(3, "Library::operator= : %s", name.c_str()); 
            if (&lib != this)
            {
                unload();
                name = lib.name;
                load();
            }
            return *this;
        }
        
        Library::~Library()
        {
            LOGINFO_1(3, "Library::destructor: %s", name.c_str()); 
            unload();
        }
        
        
        void Library::load()
        {
            LOGINFO_1(3, "Library::load : %s", name.c_str()); 
            string msg;
#if defined(WIN32)  || defined (_WINDOWS)
            int l = name.length();
            string dllName;
            if (l>=4 && name.substr(l-4, 4)==".dll")
            {
                dllName = name;
            }
            else
            {
                dllName = name+".dll";
            }
            hDLL = LoadLibrary(dllName.c_str());
            if (hDLL == NULL)
            {
                msg = "Unable to load library: " + dllName;
            }
#else
            int l = name.length();
            string soName;
            if (l>=3 && name.substr(l-3, 3)==".so")
            {
                soName = name;
            }
            else
            {
                int s = name.rfind("/");
                if (s == name.length())
                {
                    soName = name + ".so";
                }
                else
                {
                    s++;
                    soName = name.substr(0, s) + "lib" + name.substr(s, name.length()-s) + ".so";
                }
            }
            hDLL = dlopen(soName.c_str(), RTLD_NOW);
            if (hDLL == NULL)
            {
                msg = "Unable to load library: " + soName + ": " + dlerror();
            }
#endif
            if (hDLL == NULL)
            {
                LOGERROR_1(1, "Library::load: Unable to load library %s", name.c_str());
                throw TuscanyRuntimeException(msg.c_str());
            }    
        }
        
        void Library::unload()
        {
            if (hDLL != NULL)
            {
            LOGINFO_1(3, "Library::unload : %s", name.c_str()); 
#if defined(WIN32)  || defined (_WINDOWS)
                FreeLibrary(hDLL);
#else
                dlclose(hDLL);
#endif
                hDLL = NULL;
            }
        }
    
        void* Library::getSymbol(const string& symbol)
        {
            LOGINFO_1(3, "Library::getSymbol : %s", symbol.c_str()); 
            if (!hDLL)
            {
                return 0;
            }
#if defined(WIN32)  || defined (_WINDOWS)
            return GetProcAddress(hDLL, symbol.c_str());        
#else
            return dlsym(hDLL, symbol.c_str());    
#endif            
        }    
        
    } // End namespace sca
} // End namespace tuscany


