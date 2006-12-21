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

/* $Rev$ $Date$ */

#ifndef tuscany_sca_cpp_tuscanyruntime_h
#define tuscany_sca_cpp_tuscanyruntime_h

#include <string>

#include "osoa/sca/export.h"

namespace tuscany
{
    namespace sca
    {
        namespace cpp
        {
            
            /**
             * A singleton which represents the executing SCA runtime.
             */
            class SCA_CPP_API TuscanyRuntime 
            {
            public:
                /**
                * Default constructor
                */
                TuscanyRuntime(const std::string& defaultComponentName = "",
                    const std::string& root = "", const std::string& path = "");            
                
                /**
                * Destructor
                */
                virtual ~TuscanyRuntime();            
                
                
                /**
                * Set the system root configuration path
                * @param root The path to the system configuration.
                */
                void setSystemRoot(const std::string& root);
                
                /**
                * Set the system composite search path
                * @param root The search path for composites.
                */
                void setSystemPath(const std::string& path);
                
                /**
                * Set the default component for the system
                * @param componentName The name of the default component.
                */
                void setDefaultComponentName(const std::string& componentName);
                
                /**
                * start the runtime
                */
                void start();       
                
                /**
                * stop the runtime
                */
                void stop();            
                
                
            private:
                std::string systemRoot;
                std::string systemPath;
                std::string defaultComponentName;
            };
    
        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_cpp_tuscanyruntime_h
