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
//char
   virtual const char* getCustomerInformationChar(const char* customerID ) = 0;
   virtual const char* getCustomerInfoAChar(const char*) = 0;
   virtual const char* getCustomerInfoBChar(char* customerID ) = 0;
   virtual const char* getCustomerInfoCChar(char customerID ) = 0;
   virtual const char* getCustomerInfoDChar(char) = 0;
   virtual const char* getCustomerInfoEChar(char *) = 0;
   virtual const char* getCustomerInfoFChar(char *customerID ) = 0;
   virtual const char* getCustomerInfoGChar(signed char *customerID ) = 0;
        virtual const char* getCustomerInfoHChar(unsigned char *customerID ) = 0;

//long
          virtual const long* getCustomerInformationLong(const long* customerID ) = 0;
   virtual const long* getCustomerInfoALong(const long*) = 0;
   virtual const long* getCustomerInfoBLong(long* customerID ) = 0;
   virtual const long* getCustomerInfoCLong(long customerID ) = 0;
   virtual const long* getCustomerInfoDLong(long) = 0;
   virtual const long* getCustomerInfoELong(long *) = 0;
   virtual const long* getCustomerInfoFLong(long *customerID ) = 0;
   virtual const long* getCustomerInfoGLong(signed long *customerID ) = 0;
        virtual const long* getCustomerInfoHLong(unsigned long *customerID ) = 0;

//int
        virtual const int* getCustomerInformationInt(const int* customerID ) = 0;
   virtual const int* getCustomerInfoAInt(const int*) = 0;
   virtual const int* getCustomerInfoBInt(int* customerID ) = 0;
   virtual const int* getCustomerInfoCInt(int customerID ) = 0;
   virtual const int* getCustomerInfoDInt(int) = 0;
   virtual const int* getCustomerInfoEInt(int *) = 0;
   virtual const int* getCustomerInfoFInt(int *customerID ) = 0;
   virtual const int* getCustomerInfoGInt(signed int *customerID ) = 0;
        virtual const int* getCustomerInfoHInt(unsigned int *customerID ) = 0;


//__int64
        virtual const __int64* getCustomerInformationint64(const __int64* customerID ) = 0;
   virtual const __int64* getCustomerInfoAint64(const __int64*) = 0;
   virtual const __int64* getCustomerInfoBint64(__int64* customerID ) = 0;
   virtual const __int64* getCustomerInfoCint64(__int64 customerID ) = 0;
   virtual const __int64* getCustomerInfoDint64(__int64) = 0;
   virtual const __int64* getCustomerInfoEint64(__int64 *) = 0;
   virtual const __int64* getCustomerInfoFint64(__int64 *customerID ) = 0;
   virtual const __int64* getCustomerInfoGint64(signed __int64 *customerID ) = 0;
        virtual const __int64* getCustomerInfoHint64(unsigned int64 *customerID ) = 0;
         
//void
        virtual const void* getCustomerInformationVoid(const __int64* customerID ) = 0;
   virtual void getCustomerInfoAVoid(const __int64*) = 0;
   virtual void getCustomerInfoBVoid( ) = 0;
   virtual void getCustomerInfoCVoid() = 0;
   virtual void getCustomerInfoDVoid(void) = 0;
   
//inline
        virtual inline const char * getCustomerInfoAInline(char* customer id) = 0;
   inline int getCustomerInfoBInline(char* customer id) = 0;
   virtual inline friend const unsigned int * getCustomerInfoCInline(char* customer id) = 0;
   
//tricky examples
        int getCustomerInfoTrickyA(const char) = 0;
        int getCustomerInfoTrickyB(int myInt) = 0;
   



};

#endif
