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

/* $Rev$ $Date$ */

#include "commonj/sdo/SDORuntimeException.h"

#include "commonj/sdo/SettingList.h"
#include "commonj/sdo/Logger.h"
#include "commonj/sdo/Setting.h"

namespace commonj{
namespace sdo {


//SettingList::SettingList(SETTING_VECTOR p) : slist (p)
//{
//}

/*SettingList::SettingList(const SettingList &pin)
{
    Logger::log("SettingList copy constructor\n");
    slist = std::vector<Setting>(pin.getVec());
}
*/

SettingList::SettingList()
{
}

SettingList::~SettingList()
{
    while (slist.size() > 0)
    {
        Setting* s = slist[0];
        slist.erase(slist.begin());
        delete s;
    }

}


Setting& SettingList::operator[] (int pos) const
{    
    validateIndex(pos);
    return (*slist[pos]);
}

Setting* SettingList::get(int pos) 
{    
    validateIndex(pos);
    return (slist[pos]);
}

int SettingList::size () const
{
    return slist.size();
}

//SETTING_VECTOR SettingList::getVec() const
//{
//    return slist;
//}

void SettingList::insert (unsigned int index,  Setting* d)
{
    slist.insert(slist.begin()+index, d);
}

void SettingList::append ( Setting* d)
{
    slist.insert(slist.end(),d);
}

void SettingList::remove(unsigned int index)
{
    validateIndex(index);
    const Setting* s = slist[index];
    slist.erase(slist.begin()+index);
    delete s;
    return;
}

void SettingList::validateIndex(int index) const
{
    if ((index < 0) || (index > size()))
    {
        std::string msg("Index out of range:");
        msg += index;
        SDO_THROW_EXCEPTION("ValidateIndex", SDOIndexOutOfRangeException,
            msg.c_str());

    }

}

};
};

