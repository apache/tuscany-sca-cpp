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

#ifndef tuscany_sca_core_scaentrypoint_h
#define tuscany_sca_core_scaentrypoint_h

#include "osoa/sca/export.h"

#include <vector>
using std::vector;
#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/model/WireTarget.h"
#include "tuscany/sca/model/Subsystem.h"
#include "tuscany/sca/model/EntryPoint.h"
#include "tuscany/sca/model/Composite.h"
using namespace tuscany::sca::model;
#include "commonj/sdo/SDO.h"
using commonj::sdo::DataObjectPtr;
using commonj::sdo::DataFactoryPtr;

namespace tuscany
{
    namespace sca
    {
        /**
         * A runtime class which is used by external bindings to provide the entry
         * point into the SCA runtime. An entry point will be defined in an SCA
         * composite, and a binding class will be provided to service the
         * communication between the external binding (e.g. for web services the
         * SOAP/HTTP communication) and the SCA runtime. The binding class will
         * use SCAEntryPoint to call into the SCA runtime.
         */
        class  SCAEntryPoint
        {
        public:
            /**
             * Constructor to create a new instance which will be associated with
             * a given entry point in an SCA composite.
             * @param entryPointName The qualified name of the entry point in an 
             * SCA composite. It is of the form 
             * "subsystem name"/"composite component name"/"entry point name".
             */
            SCA_API SCAEntryPoint(const char* entryPointName);

            /**
             * Destructor.
             */
            SCA_API virtual    ~SCAEntryPoint();

            /**
             * Invokes an operation on the component to which the SCA entry point
             * is wired in the SCA composite.
             * @param operation. The operation request
             */
            SCA_API virtual void invoke(Operation& operation);

            /**
             * Returns a SDO data factory from which all the types associated with the entry point
             * and composite can be created.
             */
            SCA_API DataFactoryPtr getDataFactory() {return dataFactory;}

        protected:

        private:
            /**
             * The subsystem in which this entry point is found.
             */ 
            Subsystem* subsystem;

            /**
             * The entry point in the composite.
             */
            EntryPoint* entrypoint;

            /**
             * The composite component in the subsystem.
             */
            Composite* composite;

            /**
             * Cached data factory which has all the types defined for the composite in which
             * this entry point is found.
             */
            DataFactoryPtr dataFactory;

            typedef vector<ServiceWrapper*> SERVICES;
            /**
             * Vector of all the service wrappers created for this entry point. 
             * Used to destroy the wrappers when this class is destroyed.
             */
            SERVICES services;

            /**
             * Creates and returns the service wrapper for a given target.
             * @param target The target of the wire for which a service wrapper is
             * to be created.
             * @return A service wrapper to be used for invoking the target implementation.
             */
            ServiceWrapper* getServiceWrapper(WireTarget* target);
            
        };
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_core_scaentrypoint_h
