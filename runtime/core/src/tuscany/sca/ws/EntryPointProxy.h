/*
 *
 *  Copyright 2006 The Apache Software Foundation or its licensors, as applicable.
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

#ifndef tuscany_sca_ws_entrypointproxy_h
#define tuscany_sca_ws_entrypointproxy_h

#include "osoa/sca/export.h"

#include "commonj/sdo/SDO.h"
using commonj::sdo::DataObjectPtr;

#include "tuscany/sca/core/SCAEntryPoint.h"
using tuscany::sca::SCAEntryPoint;

#include "tuscany/sca/core/TuscanyRuntime.h"
using tuscany::sca::TuscanyRuntime;

#include <string>
using std::string;

namespace tuscany
{
    namespace sca
    {
        namespace ws
        {        
            class SCA_API EntryPointProxy
            {
            public:
                static EntryPointProxy* getInstance();
                virtual ~EntryPointProxy();
                virtual void init(const char* systemRoot, const char* entryPointName);
                virtual DataFactoryPtr getDataFactory(void);
                virtual DataObjectPtr invoke(const char* operationName, DataObjectPtr inputDataObject);    
            private:
                EntryPointProxy();
                static EntryPointProxy* entryPointProxyInstance;
                virtual void setOutputData(Operation operation, DataObjectPtr outputDataObject);
                string entryPointName;
                string systemRoot;
                string compositeComponent;
                SCAEntryPoint* scaEntryPoint;
                TuscanyRuntime* tuscanyRuntime;
            };
        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_ws_entrypointproxy_h
