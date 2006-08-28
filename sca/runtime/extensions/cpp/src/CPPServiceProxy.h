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

#ifndef tuscany_sca_extension_cpp_model_cppserviceproxy_h
#define tuscany_sca_extension_cpp_model_cppserviceproxy_h

#include "osoa/sca/export.h"
#include "tuscany/sca/core/ServiceProxy.h" 
#include "tuscany/sca/core/ServiceWrapper.h" 
#include "tuscany/sca/util/Library.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/Service.h"

using namespace tuscany::sca::model;
using namespace tuscany::sca;


namespace tuscany
{
    namespace sca
    {
        namespace cpp
        {
            
            /**
             * Holds a proxy for a given component and reference.
             * The proxy which is held inside a ServiceProxy will be specific to the component
             * and reference and will have been code generated and be contained in a dll 
             * created by a developer of an SCA application.
             */
            class CPPServiceProxy : public ServiceProxy
            {
            public:
                /**
                 * Create a new service proxy for a reference. The proxy will contain a pointer to
                 * the target ServiceWrapper.
                 * @param reference The reference on the source component.
                 * @param target The wrapper of the service which is wired to this reference.
                 */
                CPPServiceProxy(Reference* reference);
    
                /**
                 * Create a new service proxy for a service. The proxy will contain a pointer to
                 * the target ServiceWrapper.
                 * @param reference The service on the target component.
                 * @param target The wrapper of the target service.
                 */
                CPPServiceProxy(Service* service);
    
                /**
                 * Destructor.
                 */
                virtual ~CPPServiceProxy();
    
                /**
                 * Return an instance of the proxy created for this particular component and reference.
                 * @return The proxy.
                 */
                virtual void* getProxy();
                
                /**
                 * Return the proxies created for this particular component and reference.
                 * @return The proxies.
                 */
                virtual PROXIES getProxies();
    
            private:
    
                /**
                 * Create the proxy
                 */        
                void createProxy(Component* component, const string& name, ServiceWrapper* serviceWrapper);
            
                /**
                 * Holds the instances of the code generated proxies.
                 */ 
                PROXIES proxies;
    
                /**
                 * A function pointer to the destructor of the proxy.
                 */
                typedef void (* PROXYDESTRUCTOR) (void*);
                PROXYDESTRUCTOR destructor;
                
                /**
                 * The library which contains the code for the proxy.
                 */
                Library proxyLibrary;
    
            };
            
        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_extension_cpp_model_cppserviceproxy_h
