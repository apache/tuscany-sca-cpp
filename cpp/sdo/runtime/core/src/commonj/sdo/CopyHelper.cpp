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

/* $Rev$ $Date: 2005/12/22 16:54:14 $ */

#include "commonj/sdo/Property.h"
#include "commonj/sdo/Type.h"
#include "commonj/sdo/TypeList.h"
#include "commonj/sdo/Sequence.h"
#include "commonj/sdo/RefCountingPointer.h"
#include "commonj/sdo/DataObjectImpl.h"


#include "commonj/sdo/CopyHelper.h"

namespace commonj{
namespace sdo{

    /** CopyHelper provides static copying helper functions.
     *
     * CopyHelper provides shallow and deep copy of data objects.
     * copyShallow() copies the DataType members of the data object.
     * copy() copies all the members and recurses downwards though
     * the data graph
     */

    void CopyHelper::transferitem(DataObjectPtr to, DataObjectPtr from, const Property& p)
    {
        switch (p.getTypeEnum())
        {
        case Type::BooleanType:
            to->setBoolean(    p, from->getBoolean(p));
            break;
        case Type::ByteType:
            to->setByte( p, from->getByte(p));
            break;
        case Type::CharacterType:
            to->setCharacter( p, from->getCharacter(p));
            break;
        case Type::IntegerType: 
            to->setInteger( p, from->getInteger(p));
            break;
        case Type::ShortType:
            to->setShort( p,from->getShort(p));
            break;
        case Type::DoubleType:
            to->setDouble( p, from->getDouble(p));
            break;
        case Type::FloatType:
            to->setFloat( p, from->getFloat(p));
            break;
        case Type::LongType:
            to->setLong( p, from->getLong(p));
            break;
        case Type::DateType:
            to->setDate( p, from->getDate(p));
            break;
        case Type::BigDecimalType: 
        case Type::BigIntegerType: 
        case Type::UriType:
        case Type::StringType:
            {
                unsigned int siz =     from->getLength(p);
                if (siz > 0)
                {
                    wchar_t * buf = new wchar_t[siz];
                    from->getString(p,buf, siz);
                    to->setString(p, buf, siz);
                    delete buf;
                }
            }
            break;
        case Type::BytesType:
            {
                unsigned int siz = from->getLength(p);
                if (siz > 0)
                {
                    char * buf = new char[siz];
                    from->getBytes(p,buf, siz);
                    to->setBytes(p, buf, siz);
                    delete buf;
                }
            }
            break;
        default:
            break;
        }  // switch
    }

    void CopyHelper::transferlist(DataObjectList& to, DataObjectList& from, Type::Types t)
    {
        for (int i=0;i< from.size(); i++)
        {
            switch (t)
            {
            case Type::BooleanType:
                to.append(from.getBoolean(i));
                break;
            case Type::ByteType:
                to.append(from.getByte(i));
                break;
            case Type::CharacterType:
                to.append(from.getCharacter(i));
                break;
            case Type::IntegerType: 
                to.append(from.getInteger(i));
                break;
            case Type::ShortType:
                to.append(from.getShort(i));
                break;
            case Type::DoubleType:
                to.append(from.getDouble(i));
                break;
            case Type::FloatType:
                to.append(from.getFloat(i));
                break;
            case Type::LongType:
                to.append(from.getLong(i));
                break;
            case Type::DateType:
                to.append(from.getDate(i));
                break;
            case Type::BigDecimalType: 
            case Type::BigIntegerType: 
            case Type::UriType:
            case Type::StringType:
                {
                    unsigned int siz = from.getLength(i);
                    if (siz > 0) 
                    {
                        wchar_t * buf = new wchar_t[siz];
                        from.getString(i,buf,siz);
                        to.append(buf,siz);
                        delete buf;
                    }
                }
                break;

            case Type::BytesType:
                {
                    unsigned int siz = from.getLength(i);
                    if (siz > 0) 
                    {
                        char * buf = new char[siz];
                        from.getBytes(i,buf,siz);
                        to.append(buf,siz);
                        delete buf;
                    }
                }
                break;

            default:
                break;
            } // case
        } // for
    } // method

    /** CopyHelper provides static copying helper functions.
     *
     * copyShallow() copies the DataType members of the data object.
     * copy() copies all the members and recurses downwards though
     * the data graph
     */
    DataObjectPtr CopyHelper::copyShallow(DataObjectPtr dataObject)
    {
        return internalCopy(dataObject, false);
        
    }

    /** CopyHelper provides static copying helper functions.
     *
     * copyShallow() copies the DataType members of the data object.
     * copy() copies all the members and recurses downwards though
     * the data graph
     */
    DataObjectPtr CopyHelper::copy(DataObjectPtr dataObject)
    {
        return internalCopy(dataObject, true);
    }

    DataObjectPtr CopyHelper::internalCopy(DataObjectPtr dataObject,
                                           bool fullCopy)
    {

        DataObject* theob = dataObject;
        DataFactoryPtr fac = ((DataObjectImpl*)theob)->getDataFactory();
        if (!fac) return 0;

        const Type& t = dataObject->getType();
        DataObjectPtr newob = fac->create(t);
        if (!newob) return 0;

        PropertyList pl = dataObject->getInstanceProperties();
        for (int i=0;i < pl.size(); i++)
        {
            if (dataObject->isSet(pl[i]))
            {
                // data objects are only copied in the deep copy case
                if (pl[i].getType().isDataObjectType()) 
                {
                    if (!fullCopy) 
                    {
                        continue;
                    }
                    else
                    {
                        if (pl[i].isMany())
                        {
                            DataObjectList& dolold = dataObject->getList(pl[i]);
                            DataObjectList& dolnew = newob->getList(pl[i]);
                            for (int i=0;i< dolold.size(); i++)
                            {
                                dolnew.append(internalCopy(dolold[i],true));
                            }
                        }
                        else 
                        {
                            DataObjectPtr dob = dataObject->getDataObject(pl[i]);
                            newob->setDataObject(pl[i],internalCopy(dob,true));
                        }
                    }
                }
                else 
                {
                    if (pl[i].isMany())
                    {
                        DataObjectList& dolold = dataObject->getList(pl[i]);
                        DataObjectList& dolnew = newob->getList(pl[i]);
                        transferlist(dolnew,dolold, pl[i].getTypeEnum());
                    }
                    else 
                    {
                        transferitem(newob,dataObject, pl[i]);
                    }
                } // else
            } 
        } 
        return newob;
    }
    

}
};

