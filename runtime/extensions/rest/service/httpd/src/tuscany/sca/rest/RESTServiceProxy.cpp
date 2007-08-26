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

#include <sstream>

#include "commonj/sdo/SDO.h"

#include "RESTServiceProxy.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/Exceptions.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/SDOUtils.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/ComponentType.h"
#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/ServiceBinding.h"
#include "tuscany/sca/model/WSDLDefinition.h"
#include "tuscany/sca/model/WSDLInterface.h"
#include "tuscany/sca/model/WSDLOperation.h"
#include "tuscany/sca/model/WSDLMessagePart.h"
#include "model/RESTReferenceBinding.h"

using namespace std;
using namespace commonj::sdo;
using namespace tuscany::sca::model;
using namespace tuscany::sca::util;

namespace tuscany
{
    namespace sca
    {
        namespace rest
        {
            
            // ============================
            // Constructor: Create a proxy
            // ============================
            RESTServiceProxy::RESTServiceProxy(Reference* reference)
                : ServiceProxy(reference)
            {
                logentry();
    
                // Get the target service wrapper
                RESTReferenceBinding* referenceBinding = (RESTReferenceBinding*)reference->getBinding();
                serviceWrapper = referenceBinding->getTargetServiceBinding()->getServiceWrapper();
                
                DataFactoryPtr dataFactory = reference->getComponent()->getComposite()->getDataFactory();
                try {
                    const Type& tempType = dataFactory->getType("http://tempuri.org", "RootType");
                } catch (SDORuntimeException&)
                {
                    dataFactory->addType("http://tempuri.org", "RootType", false, false, false);                
                    dataFactory->addType("http://tempuri.org", "Wrapper", false, true, false);                
                    dataFactory->addPropertyToType(
                        "http://tempuri.org", "RootType",
                        "Wrapper",
                        "http://tempuri.org", "Wrapper",
                        false, false, true);
                    dataFactory->addType("http://tempuri.org", "Part", false, true, false);                
                    dataFactory->addPropertyToType(
                        "http://tempuri.org", "RootType",
                        "Part",
                        "http://tempuri.org", "Part",
                        false, false, true);
                }
            }
            
            // ==========
            // Destructor
            // ==========
            RESTServiceProxy::~RESTServiceProxy()
            {
                logentry();
            }
            
            ///
            /// This method will be called to process an operation invocation.
            ///
            DataObjectPtr RESTServiceProxy::invoke(const WSDLOperation& wsdlOperation, DataObjectPtr inputDataObject)
            {
                logentry();
    
                Reference* reference = getReference();
                Component* component = reference->getComponent();
                Composite* composite = component ->getComposite();
            
                RESTReferenceBinding* referenceBinding = (RESTReferenceBinding*)reference->getBinding();
                DataFactoryPtr dataFactoryPtr = reference->getComponent()->getComposite()->getDataFactory();
                                    
                // Since its Document wrapped, there will only be one message part
                std::list<std::string> partList = wsdlOperation.getOutputMessagePartNames();
                const WSDLMessagePart &part = wsdlOperation.getOutputMessagePart(partList.front());
                const char* outputTypeURI = part.getPartUri().c_str();
                const char* outputTypeName = part.getPartName().c_str();

                loginfo("WSDLOperation input message Type: %s#%s",
                    wsdlOperation.getInputMessageUri().c_str(), 
                    wsdlOperation.getInputMessageName().c_str());
                loginfo("WSDLOperation output part Type: %s#%s",
                    outputTypeURI, 
                    outputTypeName);
                
                // Create new Operation object and set parameters and return value
                Operation operation(wsdlOperation.getOperationName().c_str());
            
                try
                {
                    
                    // Go through the input data object to set the operation parameters
                    PropertyList pl = inputDataObject->getInstanceProperties();
                
                    for(int i=0; i<pl.size(); i++)
                    {
                        const char* name = pl[i].getName();
                
                        switch (pl[i].getTypeEnum()) 
                        {
                        case Type::BooleanType:
                            {
                                bool* boolData = new bool;
                                *boolData = inputDataObject->getBoolean(pl[i]);
                                operation.addParameter(boolData);
                            }
                            break;
                        case Type::ByteType:
                            {
                                char* byteData = new char;
                                *byteData = inputDataObject->getByte(pl[i]);
                                operation.addParameter(byteData);
                            }
                            break;
                        case Type::BytesType:
                            {
                                int len = inputDataObject->getLength(pl[i]);
                                char** bytesData = new char*;
                                *bytesData = new char[len+1];
                                int bytesWritten = inputDataObject->getBytes(pl[i], *bytesData, len);
                                // Ensure the bytes end with the null char. Not sure if this is neccessary
                                if(bytesWritten <= len)
                                {
                                    (*bytesData)[bytesWritten] = 0;
                                }
                                else
                                {
                                    (*bytesData)[len] = 0;
                                }
                                operation.addParameter(bytesData);
                            }
                            break;
                        case Type::CharacterType:
                            {
                                // This code should work but won't be used as there is no mapping from an XSD type to the SDO CharacterType
                                wchar_t* charData = new wchar_t;
                                *charData = inputDataObject->getCharacter(pl[i]);
                                operation.addParameter(charData);
                            }
                            break;
                        case Type::DoubleType:
                            {
                                long double* doubleData = new long double;
                                *doubleData = inputDataObject->getDouble(pl[i]);
                                operation.addParameter(doubleData);
                            }
                            break;
                        case Type::FloatType:
                            {
                                float* floatData = new float;
                                *floatData = inputDataObject->getFloat(pl[i]);
                                operation.addParameter(floatData); 
                            }
                            break;
                        case Type::IntType:
                            {
                                long* intData = new long;
                                *intData = inputDataObject->getInt(pl[i]);
                                operation.addParameter(intData);
                            }
                            break;
                        case Type::ShortType:
                            {
                                short* shortData = new short;
                                *shortData = inputDataObject->getShort(pl[i]);
                                operation.addParameter(shortData);
                            }
                            break;
                        case Type::StringType:
                            {
                                string* stringData;
                                if (pl[i].isMany())
                                {
                                    DataObjectList& l = inputDataObject->getList(pl[i]);
                                    stringData = new string(l.getCString(0));                                        
                                }
                                else
                                {
                                    if(inputDataObject->isSet(pl[i]))
                                    {
                                        stringData = new string(inputDataObject->getCString(pl[i]));                                        
                                    }
                                    else
                                    {
                                        // The data is not set, so pass an empty string as the parameter
                                        stringData = new string();
                                    }
                                }
                                operation.addParameter(stringData);
                            }
                            break;
                        case Type::DataObjectType:
                            {
                                if (!strcmp(pl[i].getType().getURI(), SDOUtils::sdoURI) &&
                                    !strcmp(pl[i].getType().getName(), "OpenDataObject")) {
                                    
                                        /*
                                     * This code deals with xsd:any element parameters
                                     * Get each element as a DataObject and add in to the parameter list
                                     */
                
                                    DataObjectList& dataObjectList = inputDataObject->getList(pl[i]);
                                
                                    for(int j=0; j<dataObjectList.size(); j++)
                                    {
                                        DataObjectPtr dob = dataObjectList[j];
                                        if(!dob)
                                        {
                                        
                                            // Add a null DataObject ptr
                                            DataObjectPtr* dataObjectData = new DataObjectPtr;
                                            *dataObjectData = NULL; 
                                            loginfo("Null OpenDataObject parameter named %s[%d]", name, j);
                                            operation.addParameter(dataObjectData);
                                        }
                                        else
                                        {
                                        
                                            SequencePtr sequence = dob->getSequence();
                                            if (sequence->size()!=0)
                                            {
                                                // Add a text element        
                                                if (sequence->isText(0))
                                                {                                        
                                                    string* stringData = new string(sequence->getCStringValue(0));
                                                    operation.addParameter(stringData);
                                                }
                                                else
                                                {
                                                    // Add a complex element DataObject
                                                    DataObjectPtr* dataObjectData =new DataObjectPtr;
                                                    *dataObjectData = sequence->getDataObjectValue(0);
                                                    if(!*dataObjectData)
                                                    {
                                                        loginfo("Null DataObject parameter named %s", name);
                                                    }
                                                    else
                                                    {
                                                        (*dataObjectData)->detach();
                                                    }
                                                    operation.addParameter(dataObjectData);
                                                }
                                            }
                                            else
                                            {
                                                // Empty content, add an empty string
                                                loginfo("Empty OpenDataObject parameter named %s[%d]", name, j);
                                                string* stringData = new string(""); 
                                                operation.addParameter(stringData);
                                            }    
                                        }                       
                                    }
                                }
                                else {
                                DataObjectPtr* dataObjectData = new DataObjectPtr;
                                if (pl[i].isMany())
                                {
                                    DataObjectList& l = inputDataObject->getList((unsigned int)i);
                                    *dataObjectData = l[0];
                                }
                                else
                                {
                                    *dataObjectData = inputDataObject->getDataObject(pl[i]);
                                }
                                if(!*dataObjectData)
                                {
                                    loginfo("Null DataObject parameter named %s", name);
                                }
                                else
                                {
                                    (*dataObjectData)->detach();
                                }
                                operation.addParameter(dataObjectData);
                                }
                            }
                            break;
                        default:
                            {
                                ostringstream msg;
                                msg << "Unsupported param type: " << pl[i].getTypeEnum();
                                throwException(SystemConfigurationException, msg.str().c_str());
                            }
                        }         
                    }
                            
                    // Call into the target service wrapper
                    serviceWrapper->invoke(operation);
            
                    // Set the data in the outputDataObject to be returned
                    DataObjectPtr outputDataObject;
                    try {
                        
                        // Create the output wrapper
                        const Type& rootType = dataFactoryPtr->getType(outputTypeURI, "RootType");
                        const Property& prop = rootType.getProperty(outputTypeName);
                        const Type& outputType = prop.getType();
                        outputDataObject = dataFactoryPtr->create(outputType);
                    }
                    catch (SDORuntimeException&)
                    {
                        try
                        {
                            
                            // Create the output wrapper
                            const Type& outputType = dataFactoryPtr->getType(outputTypeURI, outputTypeName);
                            outputDataObject = dataFactoryPtr->create(outputType);
                        }
                        catch (SDORuntimeException&)
                        {
                            // The output wrapper type is not known, create an open DataObject 
                            outputDataObject = dataFactoryPtr->create("http://tempuri.org", "Wrapper");
                        }
                    }
                    
                    setOutputData(operation, outputDataObject, dataFactoryPtr);                            

                    return outputDataObject;
                }
                catch(SDORuntimeException& ex)
                {
                    throwException(ServiceInvocationException, ex);   
                }  
                catch(TuscanyRuntimeException& ex)
                {
                    throw;   
                }  
            }
            
            
            void RESTServiceProxy::setOutputData(Operation& operation, DataObjectPtr outputDataObject, DataFactoryPtr dataFactoryPtr)
            {    
                logentry();

                // Go through data object to set the return value
                PropertyList pl = outputDataObject->getType().getProperties();
            
                if(pl.size() == 0)
                {
                    if(outputDataObject->getType().isOpenType() && outputDataObject->getType().isDataObjectType())
                    {
                        /*
                         * This code deals with returning xsd:any elements
                         */
                        DataObjectList& l = outputDataObject->getList("return");
                        Operation::ParameterType resultType = operation.getReturnType();
                        switch(resultType)
                        {
                        case Operation::BOOL: 
                            {
                                l.append(*(bool*)operation.getReturnValue());
                                break;
                            }
                        case Operation::SHORT: 
                            {
                                l.append(*(short*)operation.getReturnValue());
                                break;
                            }
                        case Operation::INT: 
                            {
                                l.append(*(long*)operation.getReturnValue());
                                break;
                            }
                        case Operation::LONG: 
                            {
                                l.append(*(long*)operation.getReturnValue());
                                break;
                            }
                        case Operation::USHORT: 
                            {
                                l.append(*(short*)operation.getReturnValue());
                                break;
                            }
                        case Operation::UINT: 
                            {
                                l.append(*(long*)operation.getReturnValue());
                                break;
                            }
                        case Operation::ULONG: 
                            {
                                l.append(*(long*)operation.getReturnValue());
                                break;
                            }
                        case Operation::FLOAT: 
                            {
                                l.append(*(float*)operation.getReturnValue());
                                break;
                            }
                        case Operation::DOUBLE: 
                            {
                                l.append(*(long double*)operation.getReturnValue());
                                break;
                            }
                        case Operation::LONGDOUBLE: 
                            {
                                l.append(*(long double*)operation.getReturnValue());
                                break;
                            }
                        case Operation::CHARS: 
                            {
                                l.append(*(char**)operation.getReturnValue());
                                break;
                            }
                        case Operation::STRING: 
                            {
                                l.append((*(string*)operation.getReturnValue()).c_str());
                                break;
                            }
                        case Operation::DATAOBJECT: 
                            {
                                l.append(*(DataObjectPtr*)operation.getReturnValue());
                                break;
                            }
                        default:
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        loginfo("No return values defined");
                    }
                }
                else {
            
                    // Should only be one return value.. This goes through all return values
                    for(int i=0; i<pl.size(); i++)
                    {
                        const char* name = pl[i].getName();
    
                        Operation::ParameterType resultType = operation.getReturnType();
                        switch(resultType)
                        {
                        case Operation::BOOL: 
                            {
                                outputDataObject->setBoolean(pl[i], *(bool*)operation.getReturnValue());
                                break;
                            }
                        case Operation::SHORT: 
                            {
                                outputDataObject->setShort(pl[i], *(short*)operation.getReturnValue());
                                break;
                            }
                        case Operation::INT: 
                            {
                                outputDataObject->setInt(pl[i], *(int*)operation.getReturnValue());
                                break;
                            }
                        case Operation::LONG: 
                            {
                                outputDataObject->setInt(pl[i], *(long*)operation.getReturnValue());
                                break;
                            }
                        case Operation::USHORT: 
                            {
                                outputDataObject->setInt(pl[i], *(unsigned short*)operation.getReturnValue());
                                break;
                            }
                        case Operation::UINT: 
                            {
                                outputDataObject->setInt(pl[i], *(unsigned int*)operation.getReturnValue());
                                break;
                            }
                        case Operation::ULONG: 
                            {
                                outputDataObject->setInt(pl[i], *(unsigned long*)operation.getReturnValue());
                                break;
                            }
                        case Operation::FLOAT: 
                            {
                                outputDataObject->setFloat(pl[i], *(float*)operation.getReturnValue());
                                break;
                            }
                        case Operation::DOUBLE: 
                            {
                                outputDataObject->setDouble(pl[i], *(double*)operation.getReturnValue());
                                break;
                            }
                        case Operation::LONGDOUBLE: 
                            {
                                outputDataObject->setDouble(pl[i], *(long double*)operation.getReturnValue());
                                break;
                            }
                        case Operation::CHARS: 
                            {
                                if(*(char**)operation.getReturnValue() != NULL)
                                {
                                    outputDataObject->setCString(pl[i], *(char**)operation.getReturnValue());
                                }
                                else
                                {
                                    loginfo("Null return value, leaving property %s unset", pl[i].getName());
                                }
                                break;
                            }
                        case Operation::STRING: 
                            {
                                outputDataObject->setCString(pl[i], (*(string*)operation.getReturnValue()).c_str());                                
                                break;
                            }
                        case Operation::DATAOBJECT: 
                            {

                                if(*(DataObjectPtr*)operation.getReturnValue() != NULL)
                                {
                                    outputDataObject->setDataObject(pl[i], *(DataObjectPtr*)operation.getReturnValue());
                                }
                                else
                                {
                                    loginfo("Null return value, leaving property %s unset", pl[i].getName());
                                }
                                
                                break;
                            }
                        default:
                            {
                                break;
                            }
                        }
                    }
                }
            }
    
        } // End namespace rest        
    } // End namespace sca
} // End namespace tuscany
