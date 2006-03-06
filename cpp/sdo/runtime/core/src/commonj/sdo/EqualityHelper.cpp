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

/* $Rev$ $Date: 2005/12/22 16:54:15 $ */

#include "commonj/sdo/Property.h"
#include "commonj/sdo/Type.h"
#include "commonj/sdo/TypeList.h"
#include "commonj/sdo/Sequence.h"

#include "commonj/sdo/DataObject.h"



#include "commonj/sdo/EqualityHelper.h"

namespace commonj{
namespace sdo{

    bool EqualityHelper::compareitem(DataObjectPtr to, DataObjectPtr from, const Property& p)
    {
        switch (p.getTypeEnum())
        {
        case Type::BooleanType:
            return (to->getBoolean(p) == from->getBoolean(p));
        case Type::ByteType:
            return (to->getByte(p) == from->getByte(p));
        case Type::CharacterType:
            return (to->getCharacter(p) == from->getCharacter(p));
        case Type::IntegerType: 
            return (to->getInteger(p) == from->getInteger(p));
        case Type::ShortType:
            return (to->getShort(p) == from->getShort(p));
        case Type::DoubleType:
            return (to->getDouble(p) == from->getDouble(p));
        case Type::FloatType:
            return (to->getFloat(p) ==  from->getFloat(p));
        case Type::LongType:
            return (to->getLong(p) == from->getLong(p));
        case Type::DateType:
            return (to->getDate(p).getTime() ==  from->getDate(p).getTime());
        case Type::BigDecimalType: 
        case Type::BigIntegerType: 
        case Type::UriType:
        case Type::StringType:
            {
                unsigned int siz =     from->getLength(p);
                if (siz != to->getLength(p)) return false;

                if (siz > 0)
                {
                    wchar_t * buff = new wchar_t[siz];
                    wchar_t * buft = new wchar_t[siz];
                    from->getString(p, buff, siz);
                    to->getString(p, buft, siz);
                    for (int i=0;i< siz; i++)
                    {
                        if (buff[i] != buft[i])
                        {
                            delete buff;
                            delete buft;
                            return false;
                        }
                    }
                    delete buff;
                    delete buft;
                }
                return true;
            }
        case Type::BytesType:
            {
                unsigned int siz =     from->getLength(p);
                if (siz != to->getLength(p)) return false;

                if (siz > 0)
                {
                    char * buff = new char[siz];
                    char * buft = new char[siz];
                    from->getBytes(p, buff, siz);
                    to->getBytes(p, buft, siz);
                    for (int i=0;i< siz; i++)
                    {
                        if (buff[i] != buft[i])
                        {
                            delete buff;
                            delete buft;
                            return false;
                        }
                    }
                    delete buff;
                    delete buft;
                }
                return true;
            }
        default:
            return true;
        }  // switch
    }

    bool EqualityHelper::comparelist(DataObjectList& to, DataObjectList& from, Type::Types t)
    {
        switch (t)
        {
        case Type::BooleanType:
        {
            for (int i=0;i< from.size(); i++)
            {
                if (to.getBoolean(i) != from.getBoolean(i))
                {
                    return false;
                }
            }
            return true;
        }

        case Type::ByteType:
        {
            for (int i=0;i< from.size(); i++)
            {
                if (to.getByte(i) != from.getByte(i))
                {
                    return false;
                }
            }
            return true;
        }

        case Type::CharacterType:
                {
            for (int i=0;i< from.size(); i++)
            {
                if (to.getCharacter(i) != from.getCharacter(i))
                {
                    return false;
                }
            }
            return true;
        }

        case Type::IntegerType: 
                {
            for (int i=0;i< from.size(); i++)
            {
                if (to.getInteger(i) != from.getInteger(i))
                {
                    return false;
                }
            }
            return true;
        }

        case Type::ShortType:
                {
            for (int i=0;i< from.size(); i++)
            {
                if (to.getShort(i) != from.getShort(i))
                {
                    return false;
                }
            }
            return true;
        }
    case Type::DoubleType:
        {
            for (int i=0;i< from.size(); i++)
            {
                if (to.getDouble(i) != from.getDouble(i))
                {
                    return false;
                }
            }
            return true;
        }

    case Type::FloatType:
        {
            for (int i=0;i< from.size(); i++)
            {
                if (to.getFloat(i) != from.getFloat(i))
                {
                    return false;
                }
            }
            return true;
        }

    case Type::LongType:
        {
            for (int i=0;i< from.size(); i++)
            {
                if (to.getLong(i) != from.getLong(i))
                {
                    return false;
                }
            }
            return true;
        }
    
    case Type::DateType:
        {
            for (int i=0;i< from.size(); i++)
            {
                if (to.getDate(i).getTime() != from.getDate(i).getTime())
                {
                    return false;
                }
            }
            return true;
        }

    case Type::BigDecimalType: 
    case Type::BigIntegerType: 
    case Type::UriType:
    case Type::StringType:
        {
            for (int i=0;i< from.size(); i++)
            {
                unsigned int siz = from.getLength(i);
                if (siz != to.getLength(i)) return false;

                if (siz > 0) 
                {
                    wchar_t * buff = new wchar_t[siz];
                    wchar_t * buft = new wchar_t[siz];
                    from.getString(i,buff,siz);
                    to.getString(i,buft,siz);
                    for (int j=0;j<siz;j++)
                    {
                        if (buff[j] != buft[j]) 
                        {
                            delete buff;
                            delete buft;
                            return false;
                        }
                    }
                    delete buff;
                    delete buft;
                }
            }
            return true;
        }

    case Type::BytesType:
        {
            for (int i=0;i< from.size(); i++)
            {
                unsigned int siz = from.getLength(i);
                if (siz != to.getLength(i)) return false;

                if (siz > 0) 
                {
                    char * buff = new char[siz];
                    char * buft = new char[siz];
                    from.getBytes(i,buff,siz);
                    to.getBytes(i,buft,siz);
                    for (int j=0;j<siz;j++)
                    {
                        if (buff[j] != buft[j]) 
                        {
                            delete buff;
                            delete buft;
                            return false;
                        }
                    }
                    delete buff;
                    delete buft;
                }
            }
            return true;
        }

    default:
        return true;

    } // case
    } // method

    bool EqualityHelper::equalShallow(DataObjectPtr dataObject1, DataObjectPtr dataObject2)
    {
        return internalEqual(dataObject1, dataObject2,  false);
        
    }

    bool EqualityHelper::equal(DataObjectPtr dataObject1, DataObjectPtr dataObject2)
    {
        return internalEqual(dataObject1, dataObject2,  true);
    }


    bool EqualityHelper::internalEqual(DataObjectPtr dataObject1,
                                                DataObjectPtr dataObject2,
                                            bool fullEqual)
    {

    
        PropertyList pl = dataObject1->getInstanceProperties();
        for (int i=0;i < pl.size(); i++)
        {
            // data objects are only compared the deep equality case
            if (pl[i].getType().isDataObjectType())
            {
                if (!fullEqual) 
                {
                    continue;
                }
                else
                {
                    if (pl[i].isMany())
                    {
                        DataObjectList& dolold = dataObject1->getList(pl[i]);
                        DataObjectList& dolnew = dataObject2->getList(pl[i]);
                        if (dolnew.size() != dolold.size()) return false;

                        for (int i=0;i< dolold.size(); i++)
                        {
                            if (!internalEqual(dolold[i], dolnew[i], true))
                            {
                                return false;
                            }
                        }
                    }
                    else 
                    {
                        if (!internalEqual(
                            dataObject1->getDataObject(pl[i]),
                            dataObject2->getDataObject(pl[i]), true))
                        {
                            return false;
                        }
                    }
                }
            }
            else 
            {
                if (pl[i].isMany())
                {
                    DataObjectList& dolold = dataObject1->getList(pl[i]);
                    DataObjectList& dolnew = dataObject2->getList(pl[i]);
                    if (dolold.size() != dolnew.size()) return false;
                    if (!comparelist(dolold, dolnew, pl[i].getTypeEnum())) return false;
                }
                else 
                {
                    if (!compareitem(dataObject1,dataObject2, pl[i]))
                    {
                        return false;
                    }
                } // else
            } 
        } 
        return true;
    }
    
}
};

