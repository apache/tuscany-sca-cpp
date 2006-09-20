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

#include "WSServiceProxy.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/util/Utils.h"
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
#include "model/WSReferenceBinding.h"

#include "commonj/sdo/SDO.h"
using namespace commonj::sdo;

using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        namespace ws
        {
            
            // ============================
            // Constructor: Create a proxy
            // ============================
            WSServiceProxy::WSServiceProxy(Reference* reference)
                : ServiceProxy(reference)
            {
                LOGENTRY(1,"WSServiceProxy::constructor");
    
                // Get the target service wrapper
                WSReferenceBinding* referenceBinding = (WSReferenceBinding*)reference->getBinding();
                serviceWrapper = referenceBinding->getTargetServiceBinding()->getServiceWrapper();
                
                LOGEXIT(1,"WSServiceProxy::constructor");
            }
            
            // ==========
            // Destructor
            // ==========
            WSServiceProxy::~WSServiceProxy()
            {
                LOGENTRY(1,"WSServiceProxy::destructor");
                LOGEXIT(1,"WSServiceProxy::destructor");
            }
            
            ///
            /// This method will be called to process an operation invocation.
            ///
            DataObjectPtr WSServiceProxy::invoke(const WSDLOperation& wsdlOperation, DataObjectPtr inputDataObject)
            {
                LOGENTRY(1,"WSServiceProxy::invoke");
    
                //printf("inputDataObject %s#%s\n", inputDataObject->getType().getURI(), inputDataObject->getType().getName());
                //Utils::printType(inputDataObject->getType());
                //Utils::printDO(inputDataObject);
            
                Reference* reference = getReference();
                Component* component = reference->getComponent();
                Composite* composite = component ->getComposite();
            
                WSReferenceBinding* referenceBinding = (WSReferenceBinding*)reference->getBinding();
                DataFactoryPtr dataFactoryPtr = reference->getComponent()->getComposite()->getDataFactory();
                                    
                bool documentStyle = wsdlOperation.isDocumentStyle();
                bool wrappedStyle = wsdlOperation.isWrappedStyle();

                const char* outputTypeURI = wsdlOperation.getOutputTypeUri().c_str();
                const char* outputTypeName = wsdlOperation.getOutputTypeName().c_str();

                LOGINFO_2(4, "WSServiceProxy has got WSDLOperation with inputType: %s#%s",
                    wsdlOperation.getInputTypeUri().c_str(), 
                    wsdlOperation.getInputTypeName().c_str());
                LOGINFO_2(4, "WSServiceProxy has got WSDLOperation with outputType: %s#%s",
                    outputTypeURI, 
                    outputTypeName);
                LOGINFO_2(4, "WSServiceProxy has got WSDLOperation with documentStyle=%d and wrapped=%d",        
                    documentStyle,
                    wrappedStyle);
                
                if (documentStyle)
                {
                    if (wrappedStyle)
                    {

                        // Create new Operation object and set parameters and return value
                        Operation operation(wsdlOperation.getOperationName().c_str());
                    
                        // Go through input data object to set the operation parameters
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
                                    //printf("inputDataObject has BooleanType named %s with value %d\n", name, boolData);
                                    operation.addParameter(boolData);
                                }
                                break;
                            case Type::ByteType:
                                {
                                    char* byteData = new char;
                                    //printf("inputDataObject has ByteType named %s\n", name);
                                    *byteData = inputDataObject->getByte(pl[i]);
                                    operation.addParameter(byteData);
                                }
                                break;
                            case Type::BytesType:
                                {
                                    int len = inputDataObject->getLength(pl[i]);
                                    char* bytesData = new char[len+1];
                                    int bytesWritten = inputDataObject->getBytes(pl[i], bytesData, len);
                                    // Ensure the bytes end with the null char. Not sure if this is neccessary
                                    if(bytesWritten <= len)
                                    {
                                        bytesData[bytesWritten] = 0;
                                    }
                                    else
                                    {
                                        bytesData[len] = 0;
                                    }
                                    //printf("inputDataObject has BytesType named %s with length %d\n", name, bytesWritten);
                                    operation.addParameter(&bytesData);
                                }
                                break;
                            case Type::CharacterType:
                                {
                                    // This code should work but won't be used as there is no mapping from an XSD type to the SDO CharacterType
                                    wchar_t* charData = new wchar_t;
                                    //printf("inputDataObject has CharacterType named %s\n", name);
                                    *charData = inputDataObject->getCharacter(pl[i]);
                                    operation.addParameter(charData);
                                }
                                break;
                            case Type::DoubleType:
                                {
                                    long double* doubleData = new long double;
                                    *doubleData = inputDataObject->getDouble(pl[i]);
                                    //printf("inputDataObject has DoubleType named %s\n", name);            
                                    operation.addParameter(doubleData);
                                }
                                break;
                            case Type::FloatType:
                                {
                                    float* floatData = new float;
                                    *floatData = inputDataObject->getFloat(pl[i]);
                                    //printf("inputDataObject has FloatType named %s with value %f\n", name, *floatData);
                                    operation.addParameter(floatData); 
                                }
                                break;
                            case Type::IntegerType:
                                {
                                    long* intData = new long;
                                    //printf("inputDataObject has IntegerType named %s\n", name);
                                    *intData = inputDataObject->getInteger(pl[i]);
                                    operation.addParameter(intData);
                                }
                                break;
                            case Type::ShortType:
                                {
                                    short* shortData = new short;
                                    //printf("inputDataObject has ShortType named %s\n", name);
                                    *shortData = inputDataObject->getShort(pl[i]);
                                    operation.addParameter(shortData);
                                }
                                break;
                            case Type::StringType:
                                {
                                    const char** stringData = new const char*; 
                                    *stringData = inputDataObject->getCString(pl[i]);
                                    //printf("inputDataObject has StringType named %s with value %s\n", name, stringData);
                                    operation.addParameter(stringData);
                                }
                                break;
                            case Type::DataObjectType:
                                {
                                    DataObjectPtr dataObjectData = inputDataObject->getDataObject(pl[i]);
                                    printf("inputDataObject has DataObjectType named %s (#%d)\n", name, dataObjectData);
                    
                                    if(!dataObjectData)
                                    {
                                        LOGINFO_1(4, "SDO DataObject parameter named %s was null", name);
                                    }
                                    operation.addParameter(&dataObjectData);
                                }
                                break;
                            case Type::OpenDataObjectType:
                                {         
                                    /*
                                     * This code deals with xsd:any element parameters
                                     * Get each element as a DataObject and add in to the parameter list
                                     */
                    
                                    printf("inputDataObject has OpenDataObjectType named %s\n", name);
                                    DataObjectList& dataObjectList = inputDataObject->getList(pl[i]);
                                    
                                    for(int j=0; j<dataObjectList.size(); j++)
                                    {
                                        DataObjectPtr dataObjectData = dataObjectList[j];
                                        if(!dataObjectData)
                                        {
                                            
                                            // Add a null DataObject ptr
                                            LOGINFO_2(4, "SDO OpenDataObject parameter named %s[%d] was null", name, j);
                                            operation.addParameter(&dataObjectData);
                                        }
                                        else
                                        {
                                            
                                            SequencePtr sequence = dataObjectData->getSequence();
                                            if (sequence->size()!=0)
                                            {
                                                // Add a text element        
                                                if (sequence->isText(0))
                                                {                                        
                                                    const char** stringData = new const char*; 
                                                    *stringData = sequence->getCStringValue(0);
        
                                                    operation.addParameter(stringData);
                                                }
                                                else
                                                {
                                                    // Add a complex element DataObject
                                                    DataObjectPtr dob = sequence->getDataObjectValue(0);
                                                    if(!dob)
                                                    {
                                                        LOGINFO_1(4, "SDO DataObject parameter named %s was null", name);
                                                    }
                                                    operation.addParameter(&dob);
                                                }
                                            }
                                            else
                                            {
                                                // Empty content, add an empty string
                                                LOGINFO_2(4, "SDO OpenDataObject parameter named %s[%d] was empty", name, j);
                                                const char** stringData = new const char*; 
                                                *stringData = "";
    
                                                operation.addParameter(stringData);
                                            }
                                        }                       
                                    }
                                }
                                break;
                            case Type::DateType:
                                LOGERROR_1(0, "SDO DateType parameters are not yet supported (%s)", name);
                                return NULL;
                            case Type::LongType:
                                LOGERROR_1(0, "SDO LongType (int64_t) parameters are not yet supported (%s)", name);
                                return NULL;
                                break;
                            case Type::UriType:
                                LOGERROR_1(0, "SDO UriType parameters are not yet supported (%s)", name);
                                return NULL;
                                break;
                            case Type::BigDecimalType:
                                LOGERROR_1(0, "SDO BigDecimalType parameters are not yet supported (%s)", name);
                                return NULL;
                                break;
                            case Type::BigIntegerType:
                                LOGERROR_1(0, "SDO BigIntegerType parameters are not yet supported (%s)", name);
                                return NULL;
                                break;
                            default:
                                LOGERROR_1(0, "Unknown SDO type parameter named %s has been found. Unknown types are not yet supported", name);
                                return NULL;
                            }         
                        }
                                
                        try
                        {
                            // Call into the target service wrapper
                            serviceWrapper->invoke(operation);
                    
                            // Set the data in the outputDataObject to be returned
                            DataObjectPtr outputDataObject = dataFactoryPtr->create(outputTypeURI, outputTypeName);
                            
                            //const Type& rootType = dataFactoryPtr->getType(outputDataObject->getType().getURI(), "RootType");
                            //printf("rootType %s#%s\n", rootType.getURI(), rootType.getName());
                            //Utils::printType(rootType);
                            //printf("outputDataObject %s#%s\n", outputDataObject->getType().getURI(), outputDataObject->getType().getName());
                            //Utils::printType(outputDataObject->getType());

                            setOutputData(operation, outputDataObject, dataFactoryPtr);                            

                            Utils::printDO(outputDataObject);

                            LOGEXIT(1,"WSServiceProxy::invoke");
                        
                            return outputDataObject;

                        }
                      catch(SDORuntimeException &ex)
                      {   
                            // TODO: Return more error information than just a null DataObject
                            LOGERROR_2(0, "%s has been caught: %s\n", ex.getEClassName(), ex.getMessageText());
                            return NULL;
                      }  
                      catch(TuscanyRuntimeException &ex)
                      {   
                            // TODO: Return more error information than just a null DataObject
                            LOGERROR_2(0, "%s has been caught: %s\n", ex.getEClassName(), ex.getMessageText());
                            return NULL;
                      }  
                    }
                    else
                    {
                        LOGERROR(0, "CompositeServices with non-wrapped document style WSDL Operations are not yet supported");
                        LOGEXIT(1,"WSServiceProxy::invoke");
                        return NULL;
                    }
                }
                else
                {
                    // RPC style
                    LOGERROR(0, "CompositeServices with RPC style WSDL Operations are not yet supported");
                    return NULL;
                }
            }
            
            
            void WSServiceProxy::setOutputData(Operation operation, DataObjectPtr outputDataObject, DataFactoryPtr dataFactoryPtr)
            {    
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
                                l.append(*(int*)operation.getReturnValue());
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
                                l.append(*(int*)operation.getReturnValue());
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
                                string msg = "Unsupported parameter type";
                                msg += resultType;
                                throw msg.c_str();
                            }
                        }
                    }
                    else
                    {
                        LOGINFO(4, "No return values are defined");
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
                                outputDataObject->setInteger(pl[i], *(int*)operation.getReturnValue());
                                break;
                            }
                        case Operation::LONG: 
                            {
                                outputDataObject->setInteger(pl[i], *(long*)operation.getReturnValue());
                                break;
                            }
                        case Operation::USHORT: 
                            {
                                outputDataObject->setInteger(pl[i], *(unsigned short*)operation.getReturnValue());
                                break;
                            }
                        case Operation::UINT: 
                            {
                                outputDataObject->setInteger(pl[i], *(unsigned int*)operation.getReturnValue());
                                break;
                            }
                        case Operation::ULONG: 
                            {
                                outputDataObject->setInteger(pl[i], *(unsigned long*)operation.getReturnValue());
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
                                outputDataObject->setCString(pl[i], *(char**)operation.getReturnValue());
                                break;
                            }
                        case Operation::STRING: 
                            {
                                outputDataObject->setCString(pl[i], (*(string*)operation.getReturnValue()).c_str());
                                break;
                            }
                        case Operation::DATAOBJECT: 
                            {
                                outputDataObject->setDataObject(pl[i], *(DataObjectPtr*)operation.getReturnValue());
                                break;
                            }
                        default:
                            {
                                string msg = "Unsupported parameter type";
                                msg += resultType;
                                throw msg.c_str();
                            }
                        }
                    }
                }
            }
    
        } // End namespace ws        
    } // End namespace sca
} // End namespace tuscany
