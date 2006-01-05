/* 
+----------------------------------------------------------------------+
| (c) Copyright IBM Corporation 2005.                                  | 
| All Rights Reserved.                                                 |
+----------------------------------------------------------------------+ 
|                                                                      | 
| Licensed under the Apache License, Version 2.0 (the "License"); you  | 
| may not use this file except in compliance with the License. You may | 
| obtain a copy of the License at                                      | 
|  http://www.apache.org/licenses/LICENSE-2.0                          |
|                                                                      | 
| Unless required by applicable law or agreed to in writing, software  | 
| distributed under the License is distributed on an "AS IS" BASIS,    | 
| WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or      | 
| implied. See the License for the specific language governing         | 
| permissions and limitations under the License.                       | 
+----------------------------------------------------------------------+ 
*/

#include "Calculator.h"
#include "osoa/sca/sca.h"
#include <iostream>
#include <stdlib.h>

using namespace osoa::sca;
using namespace std;

void usage();
bool IsNumber(const char *p);

int main(int argc, char* argv[])
{
    const char *operation;
    float arg1 = 0;
    float arg2 = 0;
	
    if (argc == 4)
    {
        operation = argv[1];
		
        if (!IsNumber(argv[2]))
        {
            cout << "Calc.exe: Argument 1 is not a number" << endl;
			usage();
        }
        else
        {
            arg1 = atof(argv[2]);
        }

        if (!IsNumber(argv[3]))
        {
            cout << "Calc.exe: Argument 2 is not a number" << endl;
			usage();
        }
        else
        {
            arg2 = atof(argv[3]);
        }
    }
    else
    {
        usage();
    }

	try
	{
		
		// Set the default module environment variable: <subsystem>/<moduleName>
		//char *defaultModule = "SCA4CPP_DEFAULT_MODULE=SubSystem2/CalculatorServiceModule";
		//putenv(defaultModule);

        // Locate a service
		ModuleContext myContext = ModuleContext::getCurrent();
		Calculator *calcService = (Calculator*) myContext.locateService("CalculatorServiceComponent");
		if (calcService == 0)
		{
			cout << "Calc.exe: Unable to find Calculator service" << endl;
		}
		else
		{
			try
			{
                float result = 0;
                if (strcmp(operation, "add") == 0)
                {
                    result = calcService->add(arg1, arg2);
                    cout << "Calculator: add(" << arg1 << "," << arg2 << ") = " << result << endl;
                }
                else
                if (strcmp(operation, "sub") == 0)
                {
                    result = calcService->sub(arg1, arg2);
                    cout << "Calculator: sub(" << arg1 << "," << arg2 << ") = " << result << endl;
                }
                else
                if (strcmp(operation, "mul") == 0)
                {
                    result = calcService->mul(arg1, arg2);
                    cout << "Calculator: mul(" << arg1 << "," << arg2 << ") = " << result << endl;
                }
                else
                if (strcmp(operation, "div") == 0)
                {
                    result = calcService->div(arg1, arg2);
                    cout << "Calculator: div(" << arg1 << "," << arg2 << ") = " << result << endl;
                }
				else
			    {
			        cout << "Calculator: Unrecognized operation: " << operation << endl;
				}
			}
			catch (char* x)
			{
				cout << "Calc.exe: exception caught: " << x << endl;
			}
		}
		
	}
	catch (ServiceRuntimeException& ex)
	{
		cout << ex << endl;
	}
	return 0;
}

void usage()
{
   cout << "Usage: calc add|sub|mul|div arg1 arg2" << endl;
   exit(1);
}

bool IsNumber (const char *p)
{
    int len = strlen(p);
    for (int i = 0; i < len; i++)
    {
	   if (!isdigit (p[i]))
       {
           return false;
       }
    }
    return true;
}
