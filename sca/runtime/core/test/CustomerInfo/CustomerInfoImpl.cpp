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

#include "CustomerInfoImpl.h"
#include <ostream>
#include <string.h>
#include "osoa/sca/sca.h"
#include "commonj/sdo/DataObjectInstance.h"
using namespace osoa::sca;
using namespace commonj::sdo;
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CustomerInfoImpl::CustomerInfoImpl()
{
    cout << "Construct CustomerInfoImpl" << endl;
    
}

CustomerInfoImpl::~CustomerInfoImpl()
{
    cout << "Destroy CustomerInfoImpl" << endl;
}

//////////////////////////////////////////////////////////////////////
// Other methods
//////////////////////////////////////////////////////////////////////
const char* CustomerInfoImpl::getCustomerInformation(const char* customerID) 
{
    cout << "In getCustomerInformation with customerid: " << customerID << endl;
    
    
    
    ComponentContext myContext = ComponentContext::getCurrent();
    
    DataObjectInstance properties = myContext.getProperties();
    DataObjectInstance props2 = properties;
    if (properties)
    {
        const char* fredVal = properties->getCString("Fred");
        cout << "Property Fred value: " << fredVal <<endl;


        DataObjectList& joeList = properties->getList("Joe");
        for (int i=0; i<joeList.size();i++)
        {
           cout << "Property Joe value " << i << ": " << joeList.getInteger(i) <<endl;
        }
    }
    
    if (!strcmp("12345", customerID))
    {
        return "xzderg";
    }
    else
    {
        throw "Wobbler";
    }
    
    
}
