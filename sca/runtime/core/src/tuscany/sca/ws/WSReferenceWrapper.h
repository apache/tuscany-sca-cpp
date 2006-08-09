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

#ifndef tuscany_sca_ws_wsreferencewrapper_h
#define tuscany_sca_ws_wsreferencewrapper_h
#include "osoa/sca/export.h"

#include "tuscany/sca/core/CompositeReferenceWrapper.h"
#include "tuscany/sca/model/CompositeReferenceType.h"
#include "tuscany/sca/core/Operation.h"

namespace tuscany
{
    namespace sca
    {
        namespace ws
        {
        
            class SCA_API WSReferenceWrapper : public tuscany::sca::CompositeReferenceWrapper
            {
            public:
                WSReferenceWrapper(tuscany::sca::model::WireTarget* target);
                virtual    ~WSReferenceWrapper();

                virtual void invoke(Operation& operation);    
            };
        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_ws_wsreferencewrapper_h
