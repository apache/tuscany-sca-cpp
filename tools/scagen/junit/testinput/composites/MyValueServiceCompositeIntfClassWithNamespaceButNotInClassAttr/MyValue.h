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

// MyValue.h: interface for the MyValueImpl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MyValue_h
#define MyValue_h
#include <string>
using std::string;

namespace Other {
   class MyValue
   {
      public:
         virtual float getMyValueOther(const char* customerID) = 0;
    virtual float getMyValueSOther(const string& customerID) = 0;
    virtual string getCustnameOther(string& customerID) = 0;
    virtual const string& getCustnamecsOther(string customerID) = 0;
   }
}
#endif
