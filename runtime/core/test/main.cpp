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

/* $Rev: 583039 $ $Date: 2007-10-08 20:49:57 -0700 (Mon, 08 Oct 2007) $ */

#pragma warning(disable:4786)

#include <iostream>

#include "tuscany/sca/core/Exceptions.h"

#include "SCATestMacros.h"
#include "wsdlTests/wsdlTests.h"
//#include "compositeTests/compositeTests.h"

using namespace std;

extern "C"{

int main (int argc, char** argv)
{
    // These variables are used and set by the TEST macro
    int testsPassed = 0;
    int testsTotal = 0;
    bool retval = false;

    try
    {
      WSDLDefinitionTest wsdlTest;
      TEST ( wsdlTest.testSimple() );
    }
    catch(...)
    {
      // All exceptions should be caught by the TEST macro

      cout << "Unexpected exception caught" << endl; 
    }

    cout
      << "---------------------"
      << "\nTest Results"
      << "\n\tTotal tests:   " << testsTotal
      << "\n\tTests passed:  " << testsPassed
      << "\n\tTests failed:  " << testsTotal - testsPassed
      << endl;

    return testsTotal - testsPassed;
}

}
