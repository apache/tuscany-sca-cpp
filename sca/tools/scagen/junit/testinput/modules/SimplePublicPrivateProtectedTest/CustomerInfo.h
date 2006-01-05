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
#ifndef CustomerInfo_h
#define CustomerInfo_h

class CustomerInfo
{
public:
   virtual const char* getCustomerInformationCharPublic(char * p1, const char* customerID ) = 0;
   virtual const char* getCustomerInfoACharPublic(char * p1,const char*) = 0;
   virtual const char* getCustomerInfoBCharPublic(char * p1, char* customerID ) = 0;
   
private:
   virtual const char* getCustomerInformationCharPrivate(char * p1, const char* customerID ) = 0;
   virtual const char* getCustomerInfoACharPrivate(char * p1,const char*) = 0;
   virtual const char* getCustomerInfoBCharPrivate(char * p1, char* customerID ) = 0;

protected:
   virtual const char* getCustomerInformationCharProtected(char * p1, const char* customerID ) = 0;
   virtual const char* getCustomerInfoACharProtected(char * p1,const char*) = 0;
   virtual const char* getCustomerInfoBCharProtected(char * p1, char* customerID ) = 0;


};

#endif
