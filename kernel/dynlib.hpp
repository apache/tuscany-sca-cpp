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

#ifndef tuscany_dlib_hpp
#define tuscany_dlib_hpp

/**
 * Simple dynamic library access functions.
 */

#include <dlfcn.h>

#include "function.hpp"
#include "gc.hpp"
#include "monad.hpp"

namespace tuscany {

/**
 * OS specific dynamic library file extension.
 */
#ifdef IS_DARWIN
const std::string dynlibExt(".dylib");
#else
const std::string dynlibExt(".so");
#endif

/**
 * Represents a reference to a dynamic library.
 */
class lib {
public:
    lib() : dl(NULL) {
    }

    lib(const std::string& name) : dl(new DynLib(name)) {
    }

    ~lib() {
    }

private:
    class DynLib {
    public:
        DynLib(const std::string& name) : name(name), h(dlopen(name.c_str(), RTLD_NOW)) {
        }
        ~DynLib() {
            if (h == NULL)
                return;
            dlclose(h);
        }

        const std::string name;
        void* h;
    };

    gc_ptr<DynLib> dl;

    friend const failable<lib, std::string> dynlib(const std::string& name);
    template<typename S> friend const failable<lambda<S>, std::string> dynlambda(const std::string& name, const lib& l);
};

/**
 * Load a dynamic library.
 */
const failable<lib, std::string> dynlib(const std::string& name) {
    const lib l(name);
    if (l.dl->h == NULL)
        return mkfailure<lib, std::string>("Could not load library: " + name + ": " + dlerror());
    return l;
}

/**
 * Find a lambda function in a dynamic library.
 */
template<typename S> const failable<lambda<S>, std::string> dynlambda(const std::string& name, const lib& l) {
    const void* s = dlsym(l.dl->h, name.c_str());
    if (s == NULL)
        return mkfailure<lambda<S>, std::string>(std::string("Could not load symbol: " + name));
    return lambda<S>((S*)s);
}

}
#endif /* tuscany_dlib_hpp */
