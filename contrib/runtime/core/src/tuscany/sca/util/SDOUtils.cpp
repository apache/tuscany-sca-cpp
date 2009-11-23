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

#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif

#include "tuscany/sca/util/SDOUtils.h"

using namespace std;
using namespace commonj::sdo;

namespace tuscany
{
    namespace sca
    {
        namespace util
        {
        
            /**
             * Constructor
             */
            SDOVisitor::SDOVisitor()
            {
            }
            
            /**
             * Destructor
             */
            SDOVisitor::~SDOVisitor()
            {
            }
            
            SDOVisitor::RESULT SDOVisitor::visit(DataObjectPtr parent, const Property& property, bool boolData)
            {
                return SDOVisitor::RESULT_NOT_SUPPORTED;
            }
            
            SDOVisitor::RESULT SDOVisitor::visit(DataObjectPtr parent, const Property& property, char byteData)
            {
                return SDOVisitor::RESULT_NOT_SUPPORTED;
            }
            
            SDOVisitor::RESULT SDOVisitor::visit(DataObjectPtr parent, const Property& property, const char* bytesData)
            {
                return SDOVisitor::RESULT_NOT_SUPPORTED;
            }
            
            SDOVisitor::RESULT SDOVisitor::visit(DataObjectPtr parent, const Property& property, wchar_t charData)
            {
                return SDOVisitor::RESULT_NOT_SUPPORTED;
            }
            
            SDOVisitor::RESULT SDOVisitor::visit(DataObjectPtr parent, const Property& property, const SDODate& dateData)
            {
                return SDOVisitor::RESULT_NOT_SUPPORTED;
            }
            
            SDOVisitor::RESULT SDOVisitor::visit(DataObjectPtr parent, const Property& property, long double doubleData)
            {
                return SDOVisitor::RESULT_NOT_SUPPORTED;
            }
            
            SDOVisitor::RESULT SDOVisitor::visit(DataObjectPtr parent, const Property& property, float floatData)
            {
                return SDOVisitor::RESULT_NOT_SUPPORTED;
            }
            
            SDOVisitor::RESULT SDOVisitor::visit(DataObjectPtr parent, const Property& property, long intData)
            {
                return SDOVisitor::RESULT_NOT_SUPPORTED;
            }
            
            SDOVisitor::RESULT SDOVisitor::visit(DataObjectPtr parent, const Property& property, short shortData)
            {
                return SDOVisitor::RESULT_NOT_SUPPORTED;
            }
            
            SDOVisitor::RESULT SDOVisitor::visit(DataObjectPtr parent, const Property& property, const string& stringData)
            {
                return SDOVisitor::RESULT_CONTINUE;
            }
            
            SDOVisitor::RESULT SDOVisitor::visit(DataObjectPtr parent, const Property& property, DataObjectPtr dataObjectData)
            {
                return SDOVisitor::RESULT_CONTINUE;
            }
            
            const char* SDOUtils::sdoURI   = "commonj.sdo";        

            SDOVisitor::RESULT SDOUtils::accept(DataObjectPtr dataObject, SDOVisitor& visitor, SDOVisitor::DEPTH depth)
            {
                SDOVisitor::RESULT result = SDOVisitor::RESULT_CONTINUE;
    
                const Type& type = dataObject->getType();
                if (type.isSequencedType())
                {
                    SequencePtr sequence = dataObject->getSequence();
                    int size = sequence->size();
                    for (int i = 0; i < size; i++)
                    {
                        if (sequence->isText(i))
                        {
                            const string text(sequence->getCStringValue(i)); 
                            result = visitor.visit(dataObject, *(Property*)NULL, (const string&)text);
                        } 
                        else 
                        {
                            const Property& seqProperty = sequence->getProperty(i); 
                            const Type& seqType = seqProperty.getType();
                            if (seqType.isDataObjectType())
                            {                                
                                DataObjectPtr dob;
                                if (seqProperty.isMany())
                                {
                                    int index = sequence->getListIndex(i);
                                    dob = dataObject->getList(seqProperty)[index];
                                }
                                else
                                {
                                    dob = dataObject->getDataObject(seqProperty);
                                }
                                
                                if (dob)
                                {
                                    result = visitor.visit(dataObject, seqProperty, (DataObjectPtr)dob);
                                }
                            } 
                            else
                            {
                                Type::Types t = seqProperty.getTypeEnum(); 
                                switch (t)
                                {
                                case Type::BooleanType:
                                    {
                                        result = visitor.visit(dataObject, seqProperty, (bool)sequence->getBooleanValue(i));
                                    }
                                    break;
                                case Type::ByteType:
                                    {
                                        result = visitor.visit(dataObject, seqProperty, (char)sequence->getByteValue(i));
                                    }
                                    break;
                                case Type::CharacterType:
                                    {
                                        result = visitor.visit(dataObject, seqProperty, (wchar_t)sequence->getCharacterValue(i));
                                    }
                                    break;
                                case Type::IntType: 
                                    {
                                        result = visitor.visit(dataObject, seqProperty, (long)sequence->getIntValue(i));
                                    }
                                    break;
                                case Type::ShortType:
                                    {
                                        result = visitor.visit(dataObject, seqProperty, (short)sequence->getShortValue(i));
                                    }
                                    break;
                                case Type::DoubleType:
                                    {
                                        result = visitor.visit(dataObject, seqProperty, (long double)sequence->getDoubleValue(i));
                                    }
                                    break;
                                case Type::FloatType:
                                    {
                                        result = visitor.visit(dataObject, seqProperty, (float)sequence->getFloatValue(i));
                                    }
                                    break;
                                case Type::LongType:
                                    {
                                        result = visitor.visit(dataObject, seqProperty, (long)sequence->getLongValue(i));
                                    }
                                    break;
                                case Type::DateType:
                                    {
                                        const SDODate date = sequence->getDateValue(i);
                                        result = visitor.visit(dataObject, seqProperty, (long)(const time_t)date.getTime());
                                    }
                                    break;
                                case Type::BigDecimalType: 
                                case Type::BigIntegerType: 
                                case Type::UriType:
                                case Type::StringType:
                                    {
                                        ;
                                        const string stringData(sequence->getCStringValue(i));
                                        result = visitor.visit(dataObject, seqProperty, (const string&)stringData);
                                    }
                                    break;
                                case Type::BytesType:
                                    {
                                        unsigned int len = sequence->getLength(i);
                                        char* byteData = new char[len];
                                        sequence->getBytesValue(i, byteData, len);
                                        result = visitor.visit(dataObject, seqProperty, byteData);
                                        delete byteData;
                                    }
                                    break;
                                default:
                                    result = SDOVisitor::RESULT_NOT_SUPPORTED;
                                    break;
                                }
                                if (result == SDOVisitor::RESULT_NOT_SUPPORTED)
                                {
                                    ;
                                    const string stringData(sequence->getCStringValue(i));
                                    result = visitor.visit(dataObject, seqProperty, (const string&)stringData);
                                }
                            } 
                        }
                        if (result == SDOVisitor::RESULT_STOP)
                            return result;
                    }
                }
                else
                {
                    PropertyList pl = dataObject->getInstanceProperties();
                    for (unsigned int i = 0; i < pl.size(); i++)
                    {
                        if (dataObject->isSet(pl[i]))
                        {
                            if (pl[i].getType().isDataObjectType()) 
                            {
                                if (pl[i].isMany())
                                {
                                    DataObjectList& doList = dataObject->getList(pl[i]);
                                    for (unsigned int li = 0; li < doList.size(); li++)
                                    {
                                        DataObjectPtr dob = doList[li];
                                        result = visitor.visit(dataObject, pl[i], (DataObjectPtr)dob);
                                        if (result == SDOVisitor::RESULT_STOP)
                                            return result;
                                    }
                                }
                                else 
                                {
                                    DataObjectPtr dob = dataObject->getDataObject(pl[i]);
                                    result = visitor.visit(dataObject, pl[i], (DataObjectPtr)dob);
                                }
                            }
                            else 
                            {
                                if (pl[i].isMany())
                                {
                                    Type::Types t = pl[i].getTypeEnum();
                                    DataObjectList& doList = dataObject->getList(pl[i]);
                                    for (unsigned int li = 0; li < doList.size(); li++)
                                    {
                                        switch (t)
                                        {
                                        case Type::BooleanType:
                                            {
                                                result = visitor.visit(dataObject, pl[i], (bool)doList.getBoolean(li));
                                            }
                                            break;
                                        case Type::ByteType:
                                            {
                                                result = visitor.visit(dataObject, pl[i], (char)doList.getByte(li));
                                            }
                                            break;
                                        case Type::CharacterType:
                                            {
                                                result = visitor.visit(dataObject, pl[i], (wchar_t)doList.getCharacter(li));
                                            }
                                            break;
                                        case Type::IntType: 
                                            {
                                                result = visitor.visit(dataObject, pl[i], (long)doList.getInt(li));
                                            }
                                            break;
                                        case Type::ShortType:
                                            {
                                                result = visitor.visit(dataObject, pl[i], (short)doList.getShort(li));
                                            }
                                            break;
                                        case Type::DoubleType:
                                            {
                                                result = visitor.visit(dataObject, pl[i], (long double)doList.getDouble(li));
                                            }
                                            break;
                                        case Type::FloatType:
                                            {
                                                result = visitor.visit(dataObject, pl[i], (float)doList.getFloat(li));
                                            }
                                            break;
                                        case Type::LongType:
                                            {
                                                result = visitor.visit(dataObject, pl[i], (long)doList.getLong(li));
                                            }
                                            break;
                                        case Type::DateType:
                                            {
                                                const SDODate date = doList.getDate(li);
                                                result = visitor.visit(dataObject, pl[i], (long)(const time_t)date.getTime());
                                            }
                                            break;
                                        case Type::BigDecimalType: 
                                        case Type::BigIntegerType: 
                                        case Type::UriType:
                                        case Type::StringType:
                                            {
                                                const string stringData(doList.getCString(li));
                                                result = visitor.visit(dataObject, pl[i], (const string&)stringData);
                                            }
                                            break;
                                        case Type::BytesType:
                                            {
                                                unsigned int len = doList.getLength(li);
                                                char* byteData = new char[len];
                                                doList.getBytes(li, byteData, len);
                                                result = visitor.visit(dataObject, pl[i], byteData);
                                                delete byteData;
                                            }
                                            break;
                                        default:
                                            result = SDOVisitor::RESULT_NOT_SUPPORTED;
                                            break;
                                        }
                                        if (result == SDOVisitor::RESULT_NOT_SUPPORTED)
                                        {
                                            const string stringData(doList.getCString(li));
                                            result = visitor.visit(dataObject, pl[i], (const string&)stringData);
                                        }
                                        if (result == SDOVisitor::RESULT_STOP)
                                            return result;
                                    }
                                }
                                else 
                                {
                                    Type::Types t = pl[i].getTypeEnum();
                                    switch (t)
                                    {
                                    case Type::BooleanType:
                                        {
                                            result = visitor.visit(dataObject, pl[i], (bool)dataObject->getBoolean(pl[i]));
                                        }
                                        break;
                                    case Type::ByteType:
                                        {
                                            result = visitor.visit(dataObject, pl[i], (char)dataObject->getByte(pl[i]));
                                        }
                                        break;
                                    case Type::CharacterType:
                                        {
                                            result = visitor.visit(dataObject, pl[i], (wchar_t)dataObject->getCharacter(pl[i]));
                                        }
                                        break;
                                    case Type::IntType: 
                                        {
                                            result = visitor.visit(dataObject, pl[i], (long)dataObject->getInt(pl[i]));
                                        }
                                        break;
                                    case Type::ShortType:
                                        {
                                            result = visitor.visit(dataObject, pl[i], (short)dataObject->getShort(pl[i]));
                                        }
                                        break;
                                    case Type::DoubleType:
                                        {
                                            result = visitor.visit(dataObject, pl[i], (long double)dataObject->getDouble(pl[i]));
                                        }
                                        break;
                                    case Type::FloatType:
                                        {
                                            result = visitor.visit(dataObject, pl[i], (float)dataObject->getFloat(pl[i]));
                                        }
                                        break;
                                    case Type::LongType:
                                        {
                                            result = visitor.visit(dataObject, pl[i], (long)dataObject->getLong(pl[i]));
                                        }
                                        break;
                                    case Type::DateType:
                                        {
                                            const SDODate date = dataObject->getDate(pl[i]);
                                            result = visitor.visit(dataObject, pl[i], (long)(const time_t)date.getTime());
                                        }
                                        break;
                                    case Type::BigDecimalType: 
                                    case Type::BigIntegerType: 
                                    case Type::UriType:
                                    case Type::StringType:
                                        {
                                            const string stringData(dataObject->getCString(pl[i]));
                                            result = visitor.visit(dataObject, pl[i], (const string&)stringData);
                                        }
                                        break;
                                    case Type::BytesType:
                                        {
                                            unsigned int len = dataObject->getLength(pl[i]);
                                            char* byteData = new char[len];
                                            dataObject->getBytes(pl[i], byteData, len);
                                            result = visitor.visit(dataObject, pl[i], byteData);
                                            delete byteData;
                                        }
                                        break;
                                    default:
                                        result = SDOVisitor::RESULT_NOT_SUPPORTED;
                                        break;
                                    }
                                    if (result == SDOVisitor::RESULT_NOT_SUPPORTED)
                                    {
                                        const string stringData(dataObject->getCString(pl[i]));
                                        result = visitor.visit(dataObject, pl[i], (const string&)stringData);
                                    }
                                }
                            }
                        }
                        if (result == SDOVisitor::RESULT_STOP)
                            return result;
                    } 
                }
        
                return SDOVisitor::RESULT_CONTINUE;
            }
        
        } // End namespace util
    } // End namespace sca
} // End namespace tuscany

