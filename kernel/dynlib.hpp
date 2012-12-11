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
const string dynlibExt(".dylib");
#else
const string dynlibExt(".so");
#endif

/**
 * Represents a reference to a dynamic library.
 */
class lib {
public:
    inline lib() : h(NULL), owner(false) {
    }

    inline lib(const string& name) : name(name), h(open(name)), owner(true) {
    }

    inline lib(const lib& l) : name(l.name), h(l.h), owner(false) {
    }

    lib& operator=(const lib& l) = delete;

    inline ~lib() {
        if (!owner)
            return;
        if (!hasContent(h) || content(h) == NULL)
            return;
        dlclose(content(h));
    }

private:
    const failable<void*> open(const string& name) {
        void * h = dlopen(c_str(name), RTLD_NOW);
        return h != NULL? failable<void*>(h) : mkfailure<void*>(string("Could not load library: ") + name + ": " + dlerror());
    }

    template<typename S> friend const failable<lambda<S> > dynlambda(const string& name, const lib& l);

    const string name;
    const failable<void*> h;
    const bool owner;
};

/**
 * Find a lambda function in a dynamic library.
 */
template<typename S> inline const failable<lambda<S> > dynlambda(const string& name, const lib& l) {
    if (!hasContent(l.h))
        return mkfailure<lambda<S>>(l.h);
    const void* s = dlsym(content(l.h), c_str(name));
    if (s == NULL)
        return mkfailure<lambda<S> >(string("Could not load symbol: ") + name);
    return lambda<S>((S*)s);
}

}
#endif /* tuscany_dlib_hpp */
