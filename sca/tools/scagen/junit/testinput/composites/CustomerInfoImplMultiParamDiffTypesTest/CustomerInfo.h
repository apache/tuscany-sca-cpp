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
//char (simple extra type)
   virtual const char* getCustomerInformationChar(char * p1, const char* customerID ) = 0;
   virtual const char* getCustomerInfoAChar(char * p1,const char*) = 0;
   virtual const char* getCustomerInfoBChar(char * p1, char* customerID ) = 0;
   virtual const char* getCustomerInfoCChar(char * p1, char customerID ) = 0;
   virtual const char* getCustomerInfoDChar(char * p1,char) = 0;
   virtual const char* getCustomerInfoEChar(char * p1 , char *) = 0;
   virtual const char* getCustomerInfoFChar(char * p1, char * p1, char *customerID ) = 0;
   virtual const char* getCustomerInfoGChar(char * p1, char *, char * p1,signed char *customerID ) = 0;
        virtual const char* getCustomerInfoHChar(unsigned char *customerID, const char, ) = 0;
        virtual const char* getCustomerInfoHChar(unsigned char *customerID, const char*) = 0;

//long (duplicate the types)
          virtual const long* getCustomerInformationLong(const long* customerID, const long* customerID ) = 0;
   virtual const long* getCustomerInfoALong(const long*,const long*) = 0;
   virtual const long* getCustomerInfoBLong(long* customerID, long* customerID2 ) = 0;
   virtual const long* getCustomerInfoCLong(long customerID , long customerID2 ) = 0;
   virtual const long* getCustomerInfoDLong(long,long) = 0;
   virtual const long* getCustomerInfoELong(long *, long*) = 0;
   virtual const long* getCustomerInfoFLong(long *customerID, long* customerID2 ) = 0;
   virtual const long* getCustomerInfoGLong(signed long *customerID, signed long *customerID2 ) = 0;
        virtual const long* getCustomerInfoHLong(signed long *customerID, signed long *customerID2 ) = 0;

//int
        virtual const int* getCustomerInformationInt(char*, const int* customerID ) = 0;
   virtual const int* getCustomerInfoAInt(char*,const int*) = 0;
   virtual const int* getCustomerInfoBInt(char*,int* customerID ) = 0;
   virtual const int* getCustomerInfoCInt(char*,int customerID ) = 0;
   virtual const int* getCustomerInfoDInt(char*,int) = 0;
   virtual const int* getCustomerInfoEInt(char*,int *) = 0;
   virtual const int* getCustomerInfoFInt(char*,int *customerID ) = 0;
   virtual const int* getCustomerInfoGInt(char*,signed int *customerID ) = 0;
        virtual const int* getCustomerInfoHInt(char*,unsigned int *customerID ) = 0;


//__int64
        virtual const __int64* getCustomerInformationint64(Diamond&,const __int64* customerID ) = 0;
   virtual const __int64* getCustomerInfoAint64(Diamond&, const __int64*) = 0;
   virtual const __int64* getCustomerInfoBint64( Diamond&, __int64* customerID ) = 0;
   virtual const __int64* getCustomerInfoCint64(Diamond&,__int64 customerID ) = 0;
   virtual const __int64* getCustomerInfoDint64(Diamond& myDiamond,__int64) = 0;
   virtual const __int64* getCustomerInfoEint64(Diamond& myDiamond, __int64 *) = 0;
   virtual const __int64* getCustomerInfoFint64(Diamond& myDiamond, __int64 *customerID ) = 0;
   virtual const __int64* getCustomerInfoGint64(Diamond& myDiamond, signed __int64 *customerID ) = 0;
        virtual const __int64* getCustomerInfoHint64(const Diamond& myDiamond, unsigned int64 *customerID ) = 0;
         
//void
        virtual const void* getCustomerInformationVoid(float& f, const __int64* customerID ) = 0;
   virtual void getCustomerInfoAVoid(const float& f, const __int64*) = 0;
   virtual void getCustomerInfoBVoid( ) = 0;
   virtual void getCustomerInfoCVoid() = 0;
   virtual void getCustomerInfoDVoid(void) = 0;
    virtual char getCustomerInfoEVoid(void) = 0;
   virtual char getCustomerInfoFVoid() = 0;
   virtual char getCustomerInfoGVoid( void      ) = 0;
   
//inline
        virtual inline const char * getCustomerInfoAInline(int,int,int,int, char* customer id) = 0;
   inline int getCustomerInfoBInline(int*,int* f, int* g, char* customer id) = 0;
   virtual inline friend const unsigned int * getCustomerInfoCInline(char* customer id) = 0;
   
//tricky examples
        int getCustomerInfoTrickyA(const char, const char) = 0;
        int getCustomerInfoTrickyB(int myInt, int myInt) = 0;
   



};

#endif
