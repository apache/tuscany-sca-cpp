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

#ifndef MyValue_h
#define MyValue_h
#include <string>
using std::string;
class MyValue  
{
public:
    virtual float getMyValue(const char* customerID) = 0;
    virtual float getMyValueS(const string& customerID) = 0;
    virtual string getCustname(string& customerID) = 0;
    virtual const string& getCustnamecs(string customerID) = 0;

};

#endif
