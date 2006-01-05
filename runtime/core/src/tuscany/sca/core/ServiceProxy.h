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

#ifndef tuscany_sca_core_serviceproxy_h
#define tuscany_sca_core_serviceproxy_h

#include "osoa/sca/export.h"

#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/util/Library.h"

using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        /**
         * Holds a proxy for a given component and reference.
         * The proxy which is held inside a ServiceProxy will be specific to the component
         * and reference and will have been code generated and be contained in a dll 
         * created by a developer of an SCA application.
         */
        class ServiceProxy 
        {
        public:
            /**
             * Create a new service proxy for a given component and reference name. The
             * create proxy will contain a pointer to the target ServiceWrapper.
             * @param component The component for which this proxy will be created (the
             * source component).
             * @param name The name of the reference on the component.
             * @param target The wrapper of the component which is wired to this component and
             * reference.
             */
            ServiceProxy(Component* component, const string& name, ServiceWrapper* target);

            /**
             * Destructor.
             */
            virtual    ~ServiceProxy();

            /**
             * Return an instance of the proxy created for this particular component and reference.
             * @return The proxy.
             */
            void* getProxy() {return proxy;}


        private:
            /**
             * Holds the instance of the code generated proxy.
             */ 
            void* proxy;

            typedef void (* PROXYDESTRUCTOR) (void*);

            /**
             * A function pointer to the destructor of the proxy.
             */
            PROXYDESTRUCTOR destructor;
            
            /**
             * The library which contains the code for the proxy.
             */
            Library proxyLibrary;

        };
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_core_serviceproxy_h
