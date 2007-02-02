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

// some strangeness in the build that causes 
// WinSock.h and WinSock2.h to be included leading to redefinitions
//#define _WINSOCKAPI_


#include <iostream>

#include <php_embed.h>

#include <string>

#include "commonj/sdo/SDO.h"

#include "osoa/sca/sca.h"

#include "Calculator.h"


using namespace std;
using namespace commonj::sdo;
using namespace osoa::sca;


int  main (int argc, char** argv)
{

    try
    {
        CompositeContext myContext = CompositeContext::getCurrent();
        Calculator *calcService = (Calculator*) myContext.locateService("CalculatorCPPComponent/CalculatorService");
        float result = calcService->div(10, 2);
        cout << "calculator_client: div(10,2) = " << result << endl;
    }
    catch (...)
    {
        printf("Got and exception");
    }

    return 0;
}
