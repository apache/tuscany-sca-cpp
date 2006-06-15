/**
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

#include "AccountService.h"


namespace com
{
    namespace bigbank
    {
        namespace account
        {

class AccountServiceImpl:  public AccountService 
    {

    public:


    AccountServiceImpl();
    virtual ~AccountServiceImpl();

    // public API

    virtual DataObjectPtr /*AccountReport*/
        getAccountReport(const char* id);


    //virtual void lazyGetDependencies();
    
private:

   
    float fromUSDollarToCurrency(float value);

    bool initialized;
	
};

}
}
} // namespaces

using namespace com::bigbank::account;
