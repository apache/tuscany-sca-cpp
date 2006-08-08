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

#ifndef tuscany_sca_core_componentservicewrapper_h
#define tuscany_sca_core_componentservicewrapper_h

#include "osoa/sca/export.h"
#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/core/Operation.h"
#include "tuscany/sca/model/CPPInterface.h"
#include "tuscany/sca/util/Library.h"

using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        /**
         * Wraps the service on a component implementation.
         * This abstract class is extended by generated code which provides 
         * the implementation of some of the methods. 
         * An instance of this class wraps the actual component implementation which 
         * has been written by a developer of an SCA application.
         */
        class ComponentServiceWrapper : public ServiceWrapper
        {
        public:
            /**
             * Factory method to create a new ComponentServiceWrapper for a given target
             * service. This method will provide all the loading of dlls required to 
             * create the target component.
             * @param target The service on the component for which this wrapper is to be
             * created.
             * @return A wrapper that references the given target.
             */
            static SCA_API ComponentServiceWrapper* createServiceWrapper(Service* target);

            /**
             * Constructor.
             * @param target The component service to which this wrapper refers.
             */
            SCA_API ComponentServiceWrapper(Service* target);

            /**
             * Destructor.
             */ 
            SCA_API virtual    ~ComponentServiceWrapper();

            /**
             * All business method calls to the target component go through the invoke method.
             * @param operation The details of the method, paramaters and return value for the
             * business method to be called on the target component.
             */
            SCA_API virtual void invoke(Operation& operation);

            /**
             * Return the loaded shared library for the target component.
             */
            Library* getLibrary() {return wrapperLibrary;}
        protected:
            /**
             * Delegated method to invoke the correct method on the target component. 
             * Implemented by the subtype.
             */
            SCA_API virtual void invokeService(Operation& operation) = 0;

            /**
             * Delegated method to create a new component implementation.
             * Implemented by the subtype.
             * @return A pointer to an instance of the component implementation class.
             */
            SCA_API virtual void* newImplementation() = 0;

            /**
             * Delegated method to delete the current instance of the component
             * implementation.
             * Implemented by the subtype.
             */
            SCA_API virtual void deleteImplementation() = 0;

            /**
             * Return the current instance of the component implementation.
             * @return A pointer to an instance of the component implementation class.
             */
            SCA_API virtual void* getImplementation();

            /**
             * Indicates that the current instance of the component implementation
             * has been finished with. 
             * Will call ComponentServiceWrapper#deleteImplementation if the
             * implementation is stateless (so that a new instance is returned
             * for each call).
             */
            SCA_API virtual void releaseImplementation();

        private:
            /**
             * Holds an implementation instance if the scope is set to composite.
             */
            static void* staticImpl;

            /**
             * The component to which this wrapper refers.
             */
            Component* component;

            /**
             * The service on the component to which this wrapper refers.
             */
            Service* service;

            /**
             * A pointer to the interface which the service exposes.
             */
            CPPInterface* interf;

            /**
             * Set to true if the service is remotable.
             */
            bool remotable;
            
            /**
             * Pointer to the loaded library which contains the component
             * implementation.
             */
            Library* wrapperLibrary;

            /**
             * Set the loaded library which contains the component 
             * implementation.
             * @param lib The library.
             */
            void setLibrary(Library* lib);
        };
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_core_componentservicewrapper_h
