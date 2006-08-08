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

#ifndef tuscany_sca_core_externalservicewrapper_h
#define tuscany_sca_core_externalservicewrapper_h

#include "osoa/sca/export.h"
#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/core/Operation.h"
#include "tuscany/sca/model/ExternalService.h"
using tuscany::sca::model::ExternalService;

namespace tuscany
{
    namespace sca
    {
        
        /**
         * Wraps an SCA external service so that it can be called by a 
         * proxy.
         * This abstract class will be extended by a class which supports
         * a specific external service binding (for example, the web services binding).
         */
        class SCA_API ExternalServiceWrapper : public ServiceWrapper
        {
        public:
            /**
             * Factory method to create a new ExternaServiceWrapper for a given
             * external service described in the SCA composite.
             */
            static ExternalServiceWrapper* createServiceWrapper(ExternalService* service);

            /**
             * Constructor.
             * @param target The target external service for which this wrapper is to be
             * created.
             * @return A wrapper that references the given target.
             */
            ExternalServiceWrapper(tuscany::sca::model::WireTarget* target);

            /**
             * Destructor.
             */
            virtual    ~ExternalServiceWrapper();

            /**
             * Return the external service (in the runtime model) that is wrapped by 
             * the instance of this class.
             */
            virtual ExternalService* getExternalService() {return (ExternalService*)getTarget();};

            /**
             * All business method calls to the external service go through the invoke method.
             * This is a delegated method which will be implemented by the subclass.
             * @param operation The details of the method, paramaters and return value for the
             * business method to be called on the external service.
             */
            virtual void invoke(Operation& operation) = 0;
        private:
            
        };
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_core_externalservicewrapper_h
