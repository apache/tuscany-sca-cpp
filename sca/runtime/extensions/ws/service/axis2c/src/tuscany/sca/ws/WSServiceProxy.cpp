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
            DataObjectPtr WSServiceProxy::invoke(const char* operationName, DataObjectPtr inputDataObject)
            {
                LOGENTRY(1,"WSServiceProxy::invoke");
            
                Reference* reference = getReference();
                Component* component = reference->getComponent();
                Composite* composite = component ->getComposite();
            
                WSReferenceBinding* referenceBinding = (WSReferenceBinding*)reference->getBinding();
                DataFactoryPtr dataFactoryPtr = reference->getComponent()->getComposite()->getDataFactory();
                                    
                DataObjectPtr outputDataObject = NULL;
                
                string wsdlNamespace = referenceBinding->getWSDLNamespaceURL();
                string wsdlPort  = referenceBinding->getPortName();
                string wsdlService = referenceBinding->getServiceName();
        
                WSDLDefinition* wsdl = composite->findWSDLDefinition(wsdlNamespace);
                if (wsdl == 0)
                {
                    LOGINFO_1(0, "WSDL description %s not found\n", wsdlNamespace.c_str());
                    return NULL;
                }
        
                WSDLOperation wsdlOperation;
                try
                {
                    wsdlOperation = wsdl->findOperation(wsdlService, wsdlPort, operationName);
                }
              catch(SystemConfigurationException &ex)
              {   
                  LOGERROR_1(0, "SystemConfigurationException has been caught: %s\n", ex.getMessageText());
                    return NULL;
                }
        
                LOGINFO_2(4, "WSServiceSkeleton has got WSDLOperation with inputType: %s#%s",
                    wsdlOperation.getInputTypeUri().c_str(), 
                    wsdlOperation.getInputTypeName().c_str());
                LOGINFO_2(4, "WSServiceSkeleton has got WSDLOperation with outputType: %s#%s",
                    wsdlOperation.getOutputTypeUri().c_str(), 
                    wsdlOperation.getOutputTypeName().c_str());
                LOGINFO_2(4, "WSServiceSkeleton has got WSDLOperation with documentStyle=%d and encoded=%d",        
                    wsdlOperation.isDocumentStyle(),
                    wsdlOperation.isEncoded());
                
                if(wsdlOperation.isDocumentStyle())
                {
                    // Document style
                    outputDataObject = dataFactoryPtr->create(wsdlOperation.getOutputTypeUri().c_str(), wsdlOperation.getOutputTypeName().c_str());
        
                    //printf("outputDataObject %s#%s\n", outputDataObject->getType().getURI(), outputDataObject->getType().getName());
                    //Utils::printDO(outputDataObject);
                }
                else
                {
                    // RPC style
                    LOGERROR(0, "CompositeServices with RPC style WSDL Operations are not yet supported");
                    return NULL;
                }
            
                // Create new Operation object and set parameters and return value
                Operation operation(operationName);
            
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
                            //printf("inputDataObject has DataObjectType named %s (#%d)\n", name, dataObjectData);
            
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
            
                            //printf("inputDataObject has OpenDataObjectType named %s\n", name);
                            DataObjectList& dataObjectList = inputDataObject->getList(pl[i]);
                            
                            for(int j=0; j<dataObjectList.size(); j++)
                            {
                                DataObjectPtr dataObjectData = dataObjectList[j];
                                if(!dataObjectData)
                                {
                                    LOGINFO_2(4, "SDO OpenDataObject parameter named %s[%d] was null", name, j);
                                }
                                operation.addParameter(&dataObjectData);
                                //Utils::printDO(dataObjectData);
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
                        
                // Now go through outputDataObject to set the return value
                pl = outputDataObject->getInstanceProperties();
            
                // Set up the possible return value pointers
                bool boolData = 0;
                char byteData = 0;
                wchar_t charData = 0;
                long double doubleData = 0;
                float floatData = 0;
                long intData = 0;
                short shortData = 0;
                const char* stringData;
                DataObjectPtr dataObjectData;
            
            
                // There should only be one return value, but go through any list anyway?
                if(pl.size() > 1)
                {
                    LOGINFO(4, "More than one return value is defined in the WSDL, just defining the first");
                }
                else if(pl.size() == 0)
                {
                    if(outputDataObject->getType().isOpenType() && outputDataObject->getType().isDataObjectType())
                    {
                        /*
                         * This code deals with returning xsd:any elements
                         * Return as a DataObject set within the outputDataObject
                         */
                       
                        // An OpenDataObject for the data to return in
                        operation.setReturnValue(&dataObjectData);            
                    }
                    else
                    {
                        LOGINFO(4, "No return values are defined in the WSDL");
                    }
            
                }
            
                if(pl.size() > 0)
                {
                    const char* name = pl[0].getName();
                    
                    switch (pl[0].getTypeEnum()) 
                    {
                    case Type::BooleanType:
                        {
                            //printf("outputDataObject has BooleanType named %s\n", name);
                            operation.setReturnValue(&boolData);
                        }
                        break;
                    case Type::ByteType:
                        {
                            //printf("outputDataObject has ByteType named %s\n", name);
                            operation.setReturnValue(&byteData);
                        }
                        break;
                    case Type::CharacterType:
                        {
                            //printf("outputDataObject has CharacterType named %s\n", name);
                            operation.setReturnValue(&charData);
                        }
                        break;
                    case Type::DoubleType:
                        {
                            //printf("outputDataObject has DoubleType named %s\n", name);
                            operation.setReturnValue((long double*) &doubleData);
                        }
                        break;
                    case Type::FloatType:
                        {
                            //printf("outputDataObject has FloatType named %s\n", name);
                            operation.setReturnValue(&floatData);
                        }
                        break;
                    case Type::IntegerType:
                        {
                            //printf("outputDataObject has IntegerType named %s\n", name);
                            operation.setReturnValue(&intData);
                        }
                        break;
                    case Type::ShortType:
                        {
                            //printf("outputDataObject has ShortType named %s\n", name);
                            operation.setReturnValue(&shortData);
                        }
                        break;
                    case Type::StringType:
                    case Type::BytesType:
                         {
                            //printf("outputDataObject has StringType or BytesType named %s\n", name);
                            operation.setReturnValue((const char**) &stringData);
                        }
                        break;
                    case Type::DataObjectType:
                        {
                            // printf("outputDataObject has DataObjectType named %s with type %s # %s\n", name, pl[0].getType().getURI(), pl[0].getType().getName());
                            operation.setReturnValue(&dataObjectData);
                        }
                        break;
                    case Type::DateType:
                        LOGERROR_1(0, "SDO DateType return values are not yet supported (%s)", name);
                        return NULL;
                    case Type::LongType:
                        LOGERROR_1(0, "SDO LongType (int64_t) return values are not yet supported (%s)", name);
                        return NULL;
                    case Type::UriType:
                        LOGERROR_1(0, "SDO UriType return values are not yet supported (%s)", name);
                        return NULL;
                    case Type::BigDecimalType:
                        LOGERROR_1(0, "SDO BigDecimalType return values are not yet supported (%s)", name);
                        return NULL;
                    case Type::BigIntegerType:
                        LOGERROR_1(0, "SDO BigIntegerType return values are not yet supported (%s)", name);
                        return NULL;
                    default:
                        LOGERROR_1(0, "Unknown SDO type return value named %s has been found. Unknown types are not yet supported", name);
                        return NULL;
                    }         
                }
            
                try
                {
                    // Call into the target service wrapper
                    serviceWrapper->invoke(operation);
            
                    // Set the data in the outputDataObject to be returned
                    setOutputData(operation, outputDataObject);                            
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
            
                LOGEXIT(1,"WSServiceSkeleton::invoke");
            
                return outputDataObject;

            }
            
            
            void WSServiceProxy::setOutputData(Operation operation, DataObjectPtr outputDataObject)
            {    
                // Go through data object to set the return value
                PropertyList pl = outputDataObject->getInstanceProperties();
            
                if(pl.size() == 0)
                {
                    if(outputDataObject->getType().isOpenType() && outputDataObject->getType().isDataObjectType())
                    {
                        /*
                         * This code deals with returning xsd:any elements
                         * Return as a DataObject set within the outputDataObject
                         */
                        
                        DataObjectPtr* dataObjectData = (DataObjectPtr*) operation.getReturnValue();
                        //Utils::printDO(*dataObjectData);
            
                        // Need to provide a name for the dataobject being returned, use the containment property name if there is one.
                        const char* rootName = "OpenDataObject";
                        try
                        {
                            const Property& prop = (*dataObjectData)->getContainmentProperty();
                            rootName = prop.getName();
                            (*dataObjectData)->detach();
                        }
                        catch(SDOPropertyNotFoundException&)
                        {
                            // DataObject has no containment property - use default rootName
                        }
                        outputDataObject->setDataObject(rootName, *dataObjectData);
                    }
                    else
                    {
                        LOGINFO(4, "No return values are defined in the WSDL");
                    }
            
                }
            
                // Should only be one return value.. This goes through all return values
                for(int i=0; i<pl.size(); i++)
                {
                    const char* name = pl[i].getName();
            
                    switch (pl[i].getTypeEnum()) 
                    {
                    case Type::BooleanType:
                        {
                            bool* boolData = (bool*)operation.getReturnValue();
                            //printf("outputDataObject has BooleanType named %s with value %d\n", name, *boolData);
                            outputDataObject->setBoolean(pl[i], *boolData); 
                        }
                        break;
                    case Type::ByteType:
                        {
                            char* byteData = (char*) operation.getReturnValue();
                            //printf("outputDataObject has ByteType named %s with value %c (#%d)\n", name, *byteData, *byteData);
                            outputDataObject->setByte(pl[i], *byteData);
                        }
                        break;
                    case Type::BytesType:
                        {
                            const char** bytesData = (const char**) operation.getReturnValue();
                            // TODO This looks for the first NULL byte - this may not be the full length of the data...
                            int len = 0;
                            while((*bytesData)[len] != 0)
                            {
                                len++;
                            }
                            //printf("outputDataObject has BytesType named %s with value %s and length %d\n", name, *bytesData, len);
                            outputDataObject->setBytes(pl[i], *bytesData, len);
                        }
                        break;
                    case Type::CharacterType:
                        {
                            wchar_t* charData = (wchar_t*) operation.getReturnValue();
                            //printf("outputDataObject has CharacterType named %s with value %s\n", name, charData);
                            outputDataObject->setCharacter(pl[i], *charData);
                        }
                        break;
                    case Type::DoubleType:
                        {
                            long double* doubleData = (long double*) operation.getReturnValue();
                            //printf("outputDataObject has DoubleType named %s with value %f\n", name, *doubleData);
                            outputDataObject->setDouble(pl[i], *doubleData);
                        }
                        break;
                    case Type::FloatType:
                        {
                            float* floatData = (float*) operation.getReturnValue();
                            //printf("outputDataObject has FloatType named %s with value %f \n", name, *floatData);
                            outputDataObject->setFloat(pl[i], *floatData);
                        }
                        break;
                    case Type::IntegerType:
                        {
                            long* intData = (long*) operation.getReturnValue();
                            //printf("outputDataObject has IntegerType named %s with value %d\n", name, *intData);
                            outputDataObject->setInteger(pl[i], *intData);
                        }
                        break;
                    case Type::ShortType:
                        {
                            short* shortData = (short*) operation.getReturnValue();
                            //printf("outputDataObject has ShortType named %s  with value %d\n", name, *shortData);
                            outputDataObject->setShort(pl[i], *shortData);
                        }
                        break;
                    case Type::StringType:
                        {
                            const char** stringData = (const char**) operation.getReturnValue();
                            //printf("outputDataObject has StringType named %s with value %s\n", name, *stringData);
                            outputDataObject->setCString(pl[i], *stringData);
                        }
                        break;
                    case Type::DataObjectType:
                        {
                            DataObjectPtr* dataObjectData = (DataObjectPtr*) operation.getReturnValue();
                            //printf("outputDataObject has DataObjectType named %s with value (%d)\n", name, (*dataObjectData));
                            outputDataObject->setDataObject(pl[i], *dataObjectData);
                        }
                        break;
                    case Type::DateType:
                        LOGERROR_1(0, "SDO DateType return values are not yet supported (%s)", name);
                        break;
                    case Type::LongType:
                        LOGERROR_1(0, "SDO LongType (int64_t) return values are not yet supported (%s)", name);
                        break;
                    case Type::UriType:
                        LOGERROR_1(0, "SDO UriType return values are not yet supported (%s)", name);
                        break;
                    case Type::BigDecimalType:
                        LOGERROR_1(0, "SDO BigDecimalType return values are not yet supported (%s)", name);
                        break;
                    case Type::BigIntegerType:
                        LOGERROR_1(0, "SDO BigIntegerType return values are not yet supported (%s)", name);
                        break;
                    default:
                        LOGERROR_1(0, "Unknown SDO type return value named %s has been found. Unknown types are not yet supported", name);
                    }   
                }
            }
    
        } // End namespace ws        
    } // End namespace sca
} // End namespace tuscany
