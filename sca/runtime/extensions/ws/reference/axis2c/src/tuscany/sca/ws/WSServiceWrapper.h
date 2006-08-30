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

#ifndef tuscany_sca_extension_ws_wsservicewrapper_h
#define tuscany_sca_extension_ws_wsservicewrapper_h

#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/core/Operation.h"
#include "tuscany/sca/model/Service.h"

using namespace tuscany::sca::model;


namespace tuscany
{
    namespace sca
    {
        
        namespace ws
        {
        
            class WSServiceWrapper : public ServiceWrapper
            {
            public:

                /**
                 * Constructor.
                 * @param target The service wrapper represents a Web service.
                 */
                WSServiceWrapper(Service* service);
                
                /**
                 * Destructor
                 */
                virtual    ~WSServiceWrapper();

                /**
                 * All business method calls on the target service are performed through
                 * this invoke method.
                 * @param operation The details of the method, paramaters and return value for the
                 * business method to be called on the target service.
                 */
                virtual void invoke(Operation& operation);
            };
        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_ws_wsservicewrapper_h
