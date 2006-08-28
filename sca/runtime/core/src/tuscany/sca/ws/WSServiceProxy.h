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

#ifndef tuscany_sca_ws_wsserviceproxy_h
#define tuscany_sca_ws_wsserviceproxy_h

#include "osoa/sca/export.h"
#include "tuscany/sca/core/TuscanyRuntime.h"
#include "tuscany/sca/core/Operation.h"
#include "tuscany/sca/core/CompositeServiceHelper.h"
#include "commonj/sdo/SDO.h"

#include <string>
using std::string;

using namespace commonj::sdo;
using namespace tuscany::sca;


namespace tuscany
{
    namespace sca
    {
        namespace ws
        {        
            class SCA_API WSServiceProxy
            {
            public:
                static WSServiceProxy* getInstance();
                virtual ~WSServiceProxy();
                virtual void init(const char* systemRoot, const char* compositeServiceName);
                virtual DataFactoryPtr getDataFactory(void);
                virtual DataObjectPtr invoke(const char* operationName, DataObjectPtr inputDataObject);    
            private:
                WSServiceProxy();
                static WSServiceProxy* compositeServiceProxyInstance;
                virtual void setOutputData(Operation operation, DataObjectPtr outputDataObject);
                string compositeServiceName;
                string systemRoot;
                string defaultComponentName;
                TuscanyRuntime* tuscanyRuntime;
                CompositeServiceHelper* compositeServiceHelper;
            };
        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_ws_wsserviceproxy_h
