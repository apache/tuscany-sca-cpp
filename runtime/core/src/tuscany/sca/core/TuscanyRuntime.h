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

#ifndef tuscany_sca_core_tuscanyruntime_h
#define tuscany_sca_core_tuscanyruntime_h

#include "osoa/sca/export.h"
#include <string>
using std::string;

namespace tuscany
{
    namespace sca
    {
        
        /**
         * A singleton which represents the executing SCA runtime.
         */
        class SCA_API TuscanyRuntime 
        {
        public:
            /**
            * Default constructor
            */
            TuscanyRuntime(const string& compositeComponent = "", const string& root = "");            
            
            /**
            * Destructor
            */
            virtual ~TuscanyRuntime();            
            
            
            /**
            * Set the system root
            * @param root The path to the deployed system.
            */
            void setSystemRoot(const string& root);
            
            /**
            * Set the default CompositeComponent for the system
            * @param compositeComponent The name of the default compositeComponent.
            */
            void setDefaultCompositeComponent(const string& compositeComponent);
            
            /**
            * start the runtime
            */
            void start();       
            
            /**
            * stop the runtime
            */
            void stop();            
            
            
        private:
            string systemRoot;
            string defaultCompositeComponent;
        };

        
    } // End namespace sca
} // End namespace tuscany




#endif // tuscany_sca_core_tuscanyruntime_h

