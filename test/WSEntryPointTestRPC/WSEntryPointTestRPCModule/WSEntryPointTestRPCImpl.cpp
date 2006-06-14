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

#include "WSEntryPointTestRPCImpl.h"
#include <stdio.h>

WSEntryPointTestRPCImpl::WSEntryPointTestRPCImpl()
{
}
    
WSEntryPointTestRPCImpl::~WSEntryPointTestRPCImpl()
{
}

// WSEntryPointTestRPC interface
char* WSEntryPointTestRPCImpl::doString(char* arg1)
{
    printf("WSEntryPointTestRPCImpl::doString %s\n", arg1);

    return "WSEntryPointTestRPCImpl::doString response";
}

