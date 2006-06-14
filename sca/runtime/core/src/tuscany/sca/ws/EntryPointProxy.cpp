/*
 * Copyright 2006 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "tuscany/sca/ws/EntryPointProxy.h"

using namespace tuscany::sca;

#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/core/SCARuntime.h"

#include "tuscany/sca/model/WSBinding.h"

#include "commonj/sdo/SDO.h"
using namespace commonj::sdo;

using namespace tuscany::sca::ws;

EntryPointProxy::EntryPointProxy(const char* systemRoot, const char* fullEntryPointName)
{
    LOGENTRY(1,"EntryPointProxy::constructor");

    try
	{
        SCARuntime::setSystemRoot(systemRoot);

		// fullEntryPointName is of the form "subsystem name"/"module component name"/"entry point name"
		// Get the "subsystem name"/"module component name" part for setDefaultModuleComponent
        // Keep the "entry point name" part for use in invoke
		string subsystemAndComponentName;
		Utils::rTokeniseString("/", fullEntryPointName, subsystemAndComponentName, entryPointName);

        //printf("EntryPointProxy tokenised URI: %s -> %s and %s\n", fullEntryPointName, subsystemAndComponentName.c_str(), entryPointName.c_str());
		SCARuntime::setDefaultModuleComponent(subsystemAndComponentName);

        scaEntryPoint = new SCAEntryPoint(fullEntryPointName);
	}
	catch(SystemConfigurationException &ex)
	{	
		LOGERROR_1(0, "SystemConfigurationException has been caught: %s\n", ex.getMessageText());
	}
	catch(ServiceRuntimeException &ex)
	{	
        LOGERROR_2(0, "%s has been caught: %s\n", ex.getEClassName(), ex.getMessageText());
	}  
    LOGEXIT(1,"EntryPointProxy::constructor");
}

EntryPointProxy::~EntryPointProxy()
{
    LOGENTRY(1,"EntryPointProxy::destructor");
    delete scaEntryPoint;
    LOGEXIT(1,"EntryPointProxy::destructor");
}

DataFactoryPtr EntryPointProxy::getDataFactory()
{
    return scaEntryPoint->getDataFactory();
}

///
/// This method will be called when an EntryPoint needs to be invoked.
///
DataObjectPtr EntryPointProxy::invoke(const char* operationName, DataObjectPtr inputDataObject)
{
    LOGENTRY(1,"EntryPointProxy::invoke");

    DataFactoryPtr dataFactoryPtr = scaEntryPoint->getDataFactory();
						
    DataObjectPtr outputDataObject = NULL;
    SCARuntime* runtime = SCARuntime::getInstance();
    
	Module* module = runtime->getCurrentModule();

	EntryPoint* entryPoint = module->findEntryPoint(entryPointName);

    Binding* binding = entryPoint->getBinding();
    if(binding->getBindingType() == Binding::SCA)
    {
        LOGINFO_1(4, "EntryPointProxy has got SCA binding: %s\n", binding->getUri().c_str());
        LOGERROR(0, "EntryPoints with SCA bindings are not yet supported");
        return NULL;
    }
    else if(binding->getBindingType() == Binding::WS)
    {
        //printf("EntryPointProxy has got WS binding: %s\n", binding->getUri().c_str());
        WSBinding* wsBinding = (WSBinding*) binding;

        string wsdlNamespace = wsBinding->getWSDLNamespaceURL();
        string wsdlPort  = wsBinding->getPortName();
        string wsdlService = wsBinding->getServiceName();

        //printf("EntryPointProxy has got WS Binding with NamespaceURL: %s Port: %s Service: %s\n", wsdlNamespace.c_str(), wsdlPort.c_str(), wsdlService.c_str());

        Wsdl* wsdl = module->findWsdl(wsdlNamespace);

        WsdlOperation operation = wsdl->findOperation(wsdlService, wsdlPort, operationName);

        LOGINFO_2(4, "EntryPointProxy has got WsdlOperation with inputType: %s#%s",
            operation.getInputTypeUri().c_str(), 
            operation.getInputTypeName().c_str());
        LOGINFO_2(4, "EntryPointProxy has got WsdlOperation with outputType: %s#%s",
            operation.getOutputTypeUri().c_str(), 
            operation.getOutputTypeName().c_str());
        LOGINFO_2(4, "EntryPointProxy has got WsdlOperation with documentStyle=%d and encoded=%d",        
            operation.isDocumentStyle(),
            operation.isEncoded());
        
        if(operation.isDocumentStyle())
        {
            // Document style
            outputDataObject = dataFactoryPtr->create(operation.getOutputTypeUri().c_str(), operation.getOutputTypeName().c_str());
        }
        else
        {
            // RPC style
            LOGERROR(0, "EntryPoints with RPC style WSDL Operations are not yet supported");
            return NULL;
        }
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
                const char* stringData = inputDataObject->getCString(pl[i]);
                //printf("inputDataObject has StringType named %s with value %s\n", name, stringData);
                operation.addParameter(&stringData);
            }
            break;
        case Type::DataObjectType:
            {                
                //printf("inputDataObject has DataObjectType named %s\n", name);
                DataObjectPtr dataObjectData = inputDataObject->getDataObject(pl[i]);
                operation.addParameter(&dataObjectData);
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

    // There should only be one return value, but go through any list anyway?
    if(pl.size() > 1)
    {
        LOGINFO(4, "More than one return value is defined in the WSDL, just defining the first");
    }

    if(pl.size() > 0)
    {
        const char* name = pl[0].getName();
        
        switch (pl[0].getTypeEnum()) 
        {
        case Type::BooleanType:
            {
                //printf("outputDataObject has BooleanType named %s\n", name);
                bool boolData = 0;
                operation.setReturnValue(&boolData);
            }
            break;
        case Type::ByteType:
            {
                //printf("outputDataObject has ByteType named %s\n", name);
                char byteData = 0;
                operation.setReturnValue(&byteData);
            }
            break;
        case Type::CharacterType:
            {
                //printf("outputDataObject has CharacterType named %s\n", name);
                wchar_t charData = 0;
                operation.setReturnValue(&charData);
            }
            break;
        case Type::DoubleType:
            {
                //printf("outputDataObject has DoubleType named %s\n", name);
                long double doubleData = 0;
                operation.setReturnValue((long double*) &doubleData);
            }
            break;
        case Type::FloatType:
            {
                //printf("outputDataObject has FloatType named %s\n", name);
                float floatData = 0;
                operation.setReturnValue(&floatData);
            }
            break;
        case Type::IntegerType:
            {
                //printf("outputDataObject has IntegerType named %s\n", name);
                long intData = 0;
                operation.setReturnValue(&intData);
            }
            break;
        case Type::ShortType:
            {
                //printf("outputDataObject has ShortType named %s\n", name);
                short shortData = 0;
                operation.setReturnValue(&shortData);
            }
            break;
        case Type::StringType:
        case Type::BytesType:
             {
                //printf("outputDataObject has StringType or BytesType named %s\n", name);
                const char* stringData;
                operation.setReturnValue((const char**) &stringData);
            }
            break;
        case Type::DataObjectType:
            {
                //printf("outputDataObject has DataObjectType named %s\n", name);
                DataObjectPtr dataObjectData = dataFactoryPtr->create(pl[0].getType());
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

    // Call into the wired module
    scaEntryPoint->invoke(operation);

    // Set the data in the outputDataObject to be returned
    setOutputData(operation, outputDataObject);                            

    LOGEXIT(1,"EntryPointProxy::invoke");

    return outputDataObject;
}


void EntryPointProxy::setOutputData(Operation operation, DataObjectPtr outputDataObject)
{    
    // Go through data object to set the return value
    PropertyList pl = outputDataObject->getInstanceProperties();

    // Should only be one return value.. This goes through all return
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
                //printf("outputDataObject has ByteType named %s with value %s\n", name, byteData);
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
                //printf("outputDataObject has FloatType named %s with value %f\n", name, *floatData);
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

