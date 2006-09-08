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

#ifndef tuscany_sca_php_phpservicewrapper_h
#define tuscany_sca_php_phpservicewrapper_h


#include "osoa/sca/export.h"
#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/core/Operation.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Interface.h"

using namespace tuscany::sca::model;


namespace tuscany
{
    namespace sca
    {
        namespace php
        {
            class PHPInterface;
            
            /**
             * Wraps the service on a component implementation.
             * This abstract class is extended by generated code which provides 
             * the implementation of some of the methods. 
             * An instance of this class wraps the actual component implementation which 
             * has been written by a developer of an SCA application.
             */
            class SCA_PHP_API PHPServiceWrapper : public ServiceWrapper
            {
            public:
                /**
                 * Factory method to create a new PHPServiceWrapper for a given target
                 * service. This method will provide all the loading of dlls required to 
                 * create the target component.
                 * @param target The service on the component for which this wrapper is to be
                 * created.
                 * @return A wrapper that references the given target.
                 */
                static PHPServiceWrapper* getServiceWrapper(Service* service);
    
                /**
                 * Constructor.
                 * @param target The component service to which this wrapper refers.
                 */
                PHPServiceWrapper(Service* service);
    
                /**
                 * Destructor.
                 */ 
                virtual    ~PHPServiceWrapper();
    
                /**
                 * All business method calls to the target component go through the invoke method.
                 * @param operation The details of the method, paramaters and return value for the
                 * business method to be called on the target component.
                 */
                virtual void invoke(Operation& operation);
    
                
            protected:
                
    
            private:


                /**
                 * The component to which this wrapper refers.
                 */
                Component* component;
    
                /**
                 * Set to true if the service is remotable.
                 */
                bool remotable;                

                /**
                 * A pointer to the interface which the service exposes.
                 */
                Interface* interf;

            };
            
        } // End namespace php
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_php_phpservicewrapper_h
