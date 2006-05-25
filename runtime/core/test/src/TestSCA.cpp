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

#include "tuscany/sca/util/Logger.h"
#include "commonj/sdo/Logger.h"
#include "osoa/sca/sca.h"
#include <iostream>
using namespace osoa::sca;
#include "tuscany/sca/core/TuscanyRuntime.h"
using namespace tuscany::sca;
#include "MyValue.h"

int main(int argc, char* argv[])
{
    tuscany::sca::Logger::setLogging(4);
    commonj::sdo::Logger::setLogging(20);
    cout << "Start of SCA test" << endl;

    // Set default module
    string systemRoot = getenv("TUSCANY_SCACPP");
    if (systemRoot  == "")
    {
        cout << "TUSCANY_SCACPP environment variable not set" <<endl;
        return -1;
    }

#if defined(WIN32)  || defined (_WINDOWS)
    systemRoot += "/projects/tuscany_sca/tuscany_sca_test/testSCASystem";
#else
    systemRoot += "/runtime/core/test/testSCASystem";
#endif
    TuscanyRuntime runtime("SubSystem1", systemRoot);

    try
    {
        runtime.start();

        // Locate a service
        ModuleContext myContext = ModuleContext::getCurrent();
        ModuleContext myContext2 = myContext;

        MyValue* myService = (MyValue*) myContext.locateService("MyValueServiceComponent");
        if (myService == 0)
        {
            cout << "Unable to find service" << endl;
        }
        else
        {
            
            try
            {
                float value = myService->getMyValue("12345");
                cout << "My value is: " << value << endl;
                value = myService->getMyValueS("12345");
                cout << "My valueSS is: " << value << endl;
                string custid = "fred";
                string name = myService->getCustname(custid);
                cout << "Returned string: " << name.c_str() << " custid now: " << custid.c_str() <<endl;
                custid = "fred";
                const string& csname = myService->getCustnamecs(custid);
                cout << "Returned string: " << csname.c_str() << " custid now: " << custid.c_str() <<endl;
                //value = myService->getMyValue("999");
                cout << "My value is: " << value << endl;
            }
            catch (char* x)
            {
                cout << "Exception caught: " << x << endl;
            }
        }
        cout << "End of SCA test" << endl;
        
    }
    catch (ServiceRuntimeException& ex)
    {
        cout << ex << endl;
    }

    runtime.stop();
    return 0;
}
