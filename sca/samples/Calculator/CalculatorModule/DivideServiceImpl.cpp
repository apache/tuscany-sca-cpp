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


#include "DivideServiceImpl.h"
#include <stdio.h>

DivideServiceImpl::DivideServiceImpl()
{
}
    
DivideServiceImpl::~DivideServiceImpl()
{
}

// DivideService interface
float DivideServiceImpl::divide(float arg1, float arg2)
{
    if(arg2 == 0.0)
    {
        printf("DivideServiceImpl::div %f / %f !! Cannot divide by zero, so returning 0\n", arg1, arg2);
        return 0;
    }

    float result = arg1 / arg2;
    printf("DivideServiceImpl::div %f / %f = %f\n", arg1, arg2, result);
    return result;
}
	

