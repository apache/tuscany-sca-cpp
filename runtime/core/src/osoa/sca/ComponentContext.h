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

#ifndef osoa_sca_componentcontext_h
#define osoa_sca_componentcontext_h

#include "osoa/sca/export.h"
#include "osoa/sca/ServiceList.h"
#include "commonj/sdo/SDO.h"
namespace osoa
{
    namespace sca
    {

        /**
         * An SCA component implementation uses the ComponentContext class to 
         * retrieve information about the configured SCA component.
         */
        class SCA_API ComponentContext  
        {
        
        public:
            /** 
             * Return a new ComponentContext for the current Component.
             */
            static ComponentContext getCurrent();

            /** 
             * Resolve a reference name into a single configured service.
             * If the component's reference is wired to more than one service
             * then theis method will return an exception.
             * @param referenceName The reference to be resolved. This must match
             * the name of a reference configured in the component type file for
             * this component.
             */
            virtual void* getService(const char* referenceName);

            /** 
             * Resolve a reference name into a list of configured services.
             * @param referenceName The reference to be resolved. This must match
             * the name of a reference configured in the component type file for
             * this component.
             */
            virtual ServiceList getServices(const char* referenceName);

            /** 
             * Get the configured properties for the component.
             * @return A data object representing all the properties that
             * are configured for this component.
             */
            virtual commonj::sdo::DataObjectPtr getProperties();

            /** 
             * Get an SDO data factory which will allow the component to 
             * create data objects for all the types configured for this
             * component.
             * @return A data factory to be used by the component to create
             * new data objects.
             */
            virtual commonj::sdo::DataFactoryPtr getDataFactory();

            /** 
             * Destructor
             */
            virtual ~ComponentContext();

            /**
             * Constructor
             */
            ComponentContext(const ComponentContext&);

            /**
             * Copy operator
             */
            ComponentContext& operator=(const ComponentContext&);
            
            /** 
             * Constructor to create an interface class from the contained
             * implementation.
             * @param  implementation the actual implementation class
             */
            ComponentContext(ComponentContext* implementation);

        private:

            /**
             * Pointer to the class which provides the actual implementation.
             */
            ComponentContext* impl;
                        
        };
    } // End namespace sca
} // End namespace osoa

#endif // osoa_sca_componentcontext_h
