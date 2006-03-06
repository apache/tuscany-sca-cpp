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

#ifndef tuscany_sca_core_servicewrapper_h
#define tuscany_sca_core_servicewrapper_h

#include "osoa/sca/export.h"

#include "tuscany/sca/core/Operation.h"
#include "tuscany/sca/model/WireTarget.h"
using tuscany::sca::model::WireTarget;

namespace tuscany
{
    namespace sca
    {
        /**
         * An abstract class that wraps a component implementation or an external
         * service.
         */
        class SCA_API ServiceWrapper 
        {
        public:
            /**
             * Constructor.
             * @param target The service wrapper wraps the target of a wire.
             */
            ServiceWrapper(WireTarget* target);

            /**
             * Destructor.
             */
            virtual    ~ServiceWrapper();

            /** 
             * Get the target represented by this wrapper.
             * @return The target represented by this wrapper.
             */
            virtual WireTarget* getTarget() {return target;}

            /**
             * All business method calls on the target service are performed through
             * this invoke method.
             * @param operation The details of the method, paramaters and return value for the
             * business method to be called on the target service.
             */
            virtual void invoke(Operation& operation) = 0;
        private:
            /**
             * The target represented by this wrapper.
             */
            WireTarget* target;
        };
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_core_servicewrapper_h
