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
#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4091)
#endif

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Exceptions.h"
#include "RESTServiceWrapper.h"
#include "tuscany/sca/core/Operation.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/CompositeReference.h"
#include "model/RESTServiceBinding.h"

#include "commonj/sdo/SDO.h"
using namespace commonj::sdo;

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

using namespace tuscany::sca;
using namespace tuscany::sca::model;

extern "C"
{
    struct Chunk {
        char *memory;
        size_t size;
    };

    static void *my_realloc(void *ptr, size_t size)
    {
        if (ptr)
            return realloc(ptr, size);
        else
            return malloc(size);
    }

    static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *data)
    {
        size_t realsize = size * nmemb;
        struct Chunk *mem = (struct Chunk *)data;

        mem->memory = (char *)my_realloc(mem->memory, mem->size + realsize + 1);
        if (mem->memory) {
            memcpy(&(mem->memory[mem->size]), ptr, realsize);
            mem->size += realsize;
            mem->memory[mem->size] = 0;
        }
        return realsize;
    }
 
 }

namespace tuscany
{
    namespace sca
    {
        namespace rest
        {
            
            bool RESTServiceWrapper::initialized = false;
            
            RESTServiceWrapper::RESTServiceWrapper(Service* service) : ServiceWrapper(service)
            {
                if (!initialized)
                {
                    curl_global_init(CURL_GLOBAL_ALL);
                    initialized = true;
                 }
            }
            
            RESTServiceWrapper::~RESTServiceWrapper()
            {
            }
            
            ///
            /// This method will be called when a service call needs to be made.
            ///
            void RESTServiceWrapper::invoke(Operation& operation) 
            {
                logentry();
            
                const string& operationName = operation.getName();
            
                loginfo("Service: %s, operation: %s", getService()->getType()->getName().c_str() , operationName.c_str());
            
                for (unsigned int i=0; i<operation.getNParms(); i++)
                {
                    loginfo("Parameter: %p, type %u", operation.getParameterValue(i),(int) operation.getParameterType(i));
                }

                // Get the service, composite reference and composite
                Service* service = getService();
                CompositeReference* compositeReference = (CompositeReference*)service->getComponent();
                Composite* composite = compositeReference->getComposite();
                
                XMLHelper* xmlHelper = composite->getXMLHelper();

                // Get the REST binding
                RESTServiceBinding* binding = (RESTServiceBinding *)service->getBinding();
                
                // Init the curl session
                CURL *curl_handle = curl_easy_init();
                Chunk chunk;
                chunk.memory=NULL;
                chunk.size = 0;
                
                // Some servers don't like requests that are made without a user-agent
                curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

                // HTTP GET
                const string& opName = operation.getName();
                if (opName == "get")
                {
                    // Build the request URL
                    string uri = binding->getURI();
                    ostringstream os;
                    if (uri[uri.length()-1] == '?')
                    {
                        // Add the parameters to the end of the URL in the form
                        // param=value&
                        os << uri;
                        for (int i=0; i<operation.getNParms(); i++)
                        {
                            os << "param" << (i + 1) << "=";
                            writeParameter(xmlHelper, os, operation.getParameter(i));
                            if (i < operation.getNParms()-1)
                                os << "&";
                        }
                    }
                    else
                    {
                        // Add the parameters to the end of the URL in the
                        // form value1 / value2 / value3 
                        os << uri;
                        for (int i=0; i<operation.getNParms(); i++)
                        {
                            os << "/";
                            writeParameter(xmlHelper, os, operation.getParameter(i));
                        }
                    }

                    string url = os.str();                                        
                    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
 
                    // Send all data to this function
                    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
 
                    // Pass our 'chunk' struct to the callback function
                    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
                    // Perform the HTTP get
                    CURLcode rc = curl_easy_perform(curl_handle);
                    
                    if (rc)
                    {
                        if (chunk.memory)
                            free(chunk.memory);
                        throwException(ServiceInvocationException, curl_easy_strerror(rc)); 
                    }

                    // Get the output data out of the returned document 
                    if (chunk.memory)
                    {
                        string payload((const char*)chunk.memory, chunk.size);
                        free(chunk.memory);

                        // TODO This is a temp hack, clean this up
                        // Wrap the returned document inside a part element 
                        string part = 
                        "<part xsi:type=\"sdo:OpenDataObject\" xmlns:sdo=\"commonj.sdo\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
                        + payload
                        + "</part>";
                        setReturn(xmlHelper, part, operation);
                    }
                }
                else if (opName == "post")
                {
                    // HTTP POST
                    
                    // Set the target URL
                    string url = binding->getURI();  
                    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());

                    // If the request contains complex content then we'll use
                    // a multipart form POST, otherwise we use a simpler
                    // url-encoded POST
                    bool complexContent = false; 
                    for (int i=0; i<operation.getNParms(); i++)
                    {
                        if (operation.getParameter(i).getType() == Operation::DATAOBJECT)
                        {
                            complexContent = true;
                            break;
                        }
                    }
                    struct curl_httppost *formpost = NULL;
                    if (complexContent)
                    {
                        // Build the input form
                        struct curl_httppost *lastptr = NULL;
                        for (int i=0; i<operation.getNParms(); i++)
                        {
                            ostringstream pname;
                            pname << "param" << (i+1);
                            
                            ostringstream pvalue;
                            writeParameter(xmlHelper, pvalue, operation.getParameter(i));
                            
                            curl_formadd(&formpost,
                                &lastptr,
                                CURLFORM_COPYNAME, pname.str().c_str(),
                                CURLFORM_COPYCONTENTS, pvalue.str().c_str(),
                                CURLFORM_END);
                        }
                        
                        // Set the form into the request
                        curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, formpost);
                    }
                    else
                    {
                        // Build the request string
                        // Add the parameters in the form // param=value&
                        ostringstream os;
                        for (int i=0; i<operation.getNParms(); i++)
                        {
                            os << "param" << (i + 1) << "=";
                            writeParameter(xmlHelper, os, operation.getParameter(i));
                            if (i < operation.getNParms()-1)
                                os << "&";
                        }
                        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, os.str().c_str());
                    }                    
                        
                    // Send all data to this function
                    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
 
                    // Pass our 'chunk' struct to the callback function
                    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
                    // Perform the HTTP post
                    CURLcode rc = curl_easy_perform(curl_handle);
                    
                    // Cleanup the form
                    if (complexContent)
                    {
                        curl_formfree(formpost);
                    }                    
                    
                    if (rc)
                    {
                        if (chunk.memory)
                            free(chunk.memory);
                        throwException(ServiceInvocationException, curl_easy_strerror(rc)); 
                    }

                    // Get the output data out of the returned document 
                    if (chunk.memory)
                    {
                        string payload((const char*)chunk.memory, chunk.size);
                        free(chunk.memory);

                        // TODO This is a temp hack, clean this up
                        // Wrap the returned document inside a part element 
                        string part = 
                        "<part xsi:type=\"sdo:OpenDataObject\" xmlns:sdo=\"commonj.sdo\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
                        + payload
                        + "</part>";
                        setReturn(xmlHelper, part, operation);
                    }
                }
                else if (opName == "put")
                {
                    // HTTP PUT
                }
                else if (opName == "delete")
                {
                    // HTTP DELETE
                    
                }
                else
                {
                    // XML / HTTP RPC style
                    
                    // If the request contains complex content then we'll use
                    // a multipart form POST, otherwise we use a GET
                    bool complexContent = false; 
                    for (int i=0; i<operation.getNParms(); i++)
                    {
                        if (operation.getParameter(i).getType() == Operation::DATAOBJECT)
                        {
                            complexContent = true;
                            break;
                        }
                    }
                    struct curl_httppost *formpost = NULL;
                    if (complexContent)
                    {
                       // Set the target URL
                        string url = binding->getURI();  
                        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());

                        // Build the input form
                        struct curl_httppost *lastptr = NULL;
                        for (int i=0; i<operation.getNParms(); i++)
                        {
                            ostringstream pname;
                            pname << "param" << (i+1);
                            
                            ostringstream pvalue;
                            writeParameter(xmlHelper, pvalue, operation.getParameter(i));
                            
                            curl_formadd(&formpost,
                                &lastptr,
                                CURLFORM_COPYNAME, pname.str().c_str(),
                                CURLFORM_COPYCONTENTS, pvalue.str().c_str(),
                                CURLFORM_END);
                        }
                        
                        // Set the form into the request
                        curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, formpost);
                    }
                    else
                    {

                        // Build the request URL, uri / opName ? params
                        string uri = binding->getURI();
                        ostringstream os;
                        os << uri << "/" << opName;
    
                        // Add the parameters to the end of the URL in the form
                        // param=value&
                        for (int i=0; i<operation.getNParms(); i++)
                        {
                            if (i == 0)
                                os << "?";
                            os << "param" << (i + 1) << "=";
                            writeParameter(xmlHelper, os, operation.getParameter(i));
                            if (i < operation.getNParms()-1)
                                os << "&";
                        }
    
                        string url = os.str();                                        
                        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
                    }
                    
                    // Send all data to this function
                    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
 
                    // Pass our 'chunk' struct to the callback function
                    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
                    // Perform the HTTP request
                    CURLcode rc = curl_easy_perform(curl_handle);

                    // Cleanup the form
                    if (complexContent)
                    {
                        curl_formfree(formpost);
                    }                    
                    
                    if (rc)
                    {
                        if (chunk.memory)
                            free(chunk.memory);
                        throwException(ServiceInvocationException, curl_easy_strerror(rc)); 
                    }
 
                    if (chunk.memory)
                    {
                        string payload((const char*)chunk.memory, chunk.size);
                        free(chunk.memory);
                        
                        setReturn(xmlHelper, payload, operation);
                    }
                }
                
                // Cleanup curl session
                curl_easy_cleanup(curl_handle);
            }
            
            void RESTServiceWrapper::writeParameter(XMLHelper* xmlHelper, ostringstream& os, const Operation::Parameter& parm)
            {
                logentry();

                switch(parm.getType())
                {
                case Operation::BOOL: 
                    {
                        os << *(bool*)parm.getValue();
                        break;
                    }
                case Operation::SHORT: 
                    {
                        os << *(short*)parm.getValue();
                        break;
                    }
                case Operation::INT: 
                    {
                        os << *(long*)parm.getValue();
                        break;
                    }
                case Operation::LONG: 
                    {
                        os << *(long*)parm.getValue();
                        break;
                    }
                case Operation::USHORT: 
                    {
                        os << *(short*)parm.getValue();
                        break;
                    }
                case Operation::UINT: 
                    {
                        os << *(long*)parm.getValue();
                        break;
                    }
                case Operation::ULONG: 
                    {
                        os << *(long*)parm.getValue();
                        break;
                    }
                case Operation::FLOAT: 
                    {
                        os << *(float*)parm.getValue();
                        break;
                    }
                case Operation::DOUBLE: 
                    {
                        os << *(long double*)parm.getValue();
                        break;
                    }
                case Operation::LONGDOUBLE: 
                    {
                        os << *(long double*)parm.getValue();
                        break;
                    }
                case Operation::CHARS: 
                    {
                        os << *(char**)parm.getValue();
                        break;
                    }
                case Operation::STRING: 
                    {
                        os << (*(string*)parm.getValue()).c_str();
                        break;
                    }
                case Operation::DATAOBJECT: 
                    {
                        DataObjectPtr dob = *(DataObjectPtr*)parm.getValue(); 
                        XMLDocumentPtr doc = xmlHelper->createDocument(dob, NULL, NULL);
                        xmlHelper->save(doc, os);
                        break;
                    }
                default:
                    {
                        ostringstream msg;
                        msg << "Unsupported parameter type: " << parm.getType();
                        throwException(ServiceDataException, msg.str().c_str());
                    }
                }
            }

             void RESTServiceWrapper::setReturn(XMLHelper* xmlHelper, string& payload,
                 Operation& operation)
             {  
                logentry();
                
                //TODO Remove this workaround once SDO supports loading of open top level content
                // The workaround is to wrap the open content in a wrapper element of type OpenDataObject
                string body = 
                "<body xsi:type=\"sdo:OpenDataObject\" xmlns:sdo=\"commonj.sdo\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
                + payload
                + "</body>";
                
                // Convert the body to an SDO DataObject
                DataObjectPtr outputBodyDataObject = NULL;
                XMLDocumentPtr theXMLDocument = xmlHelper->load(body.c_str(), NULL);
                if (theXMLDocument != 0)
                {
                    outputBodyDataObject = theXMLDocument->getRootDataObject();
                }
                if(!outputBodyDataObject)
                {
                    logerror("Could not convert received document to SDO");
                    return;
                }                    

                // Get the body part
                DataObjectPtr outputDataObject = NULL; 
                PropertyList bpl = outputBodyDataObject->getInstanceProperties();
                if (bpl.size()!=0)
                {
                    if (bpl[0].isMany())
                    {
                        DataObjectList& parts = outputBodyDataObject->getList((unsigned int)0);
                        outputDataObject = parts[0];
                    }
                    else
                    {
                        outputDataObject = outputBodyDataObject->getDataObject(bpl[0]);
                    }
                }
                if (outputDataObject == NULL)
                {
                    logerror("Could not convert body part to SDO");
                    return;
                }
                 
                PropertyList pl = outputDataObject->getInstanceProperties();
                unsigned int i = 0;
                 
                switch(pl[i].getTypeEnum())
                {
                case Type::BooleanType:
                    {
                        bool* boolData = new bool;
                        *boolData = outputDataObject->getBoolean(pl[i]);
                        operation.setReturnValue(boolData);
                    }
                    break;
                case Type::ByteType:
                    {
                        char* byteData = new char;
                        *byteData = outputDataObject->getByte(pl[i]);
                        operation.setReturnValue(byteData);
                    }
                    break;
                case Type::BytesType:
                    {
                        int len = outputDataObject->getLength(pl[i]);
                        char* bytesData = new char[len+1];
                        int bytesWritten = outputDataObject->getBytes(pl[i], bytesData, len);
                        // Ensure the bytes end with the null char. Not sure if this is neccessary
                        if(bytesWritten <= len)
                        {
                            bytesData[bytesWritten] = 0;
                        }
                        else
                        {
                            bytesData[len] = 0;
                        }
                        operation.setReturnValue(&bytesData);
                    }
                    break;
                case Type::CharacterType:
                    {
                        // This code should work but won't be used as there is no mapping from an XSD type to the SDO CharacterType
                        wchar_t* charData = new wchar_t;
                        *charData = outputDataObject->getCharacter(pl[i]);
                        operation.setReturnValue(charData);
                    }
                    break;
                case Type::DoubleType:
                    {
                        long double* doubleData = new long double;
                        *doubleData = outputDataObject->getDouble(pl[i]);
                        operation.setReturnValue(doubleData);
                    }
                    break;
                case Type::FloatType:
                    {
                        float* floatData = new float;
                        *floatData = outputDataObject->getFloat(pl[i]);
                        operation.setReturnValue(floatData); 
                    }
                    break;
                case Type::IntegerType:
                    {
                        long* intData = new long;
                        *intData = outputDataObject->getInteger(pl[i]);
                        operation.setReturnValue(intData);
                    }
                    break;
                case Type::ShortType:
                    {
                        short* shortData = new short;
                        *shortData = outputDataObject->getShort(pl[i]);
                        operation.setReturnValue(shortData);
                    }
                    break;
                case Type::StringType:
                    {
                        string* str = new string(outputDataObject->getCString(pl[i]));
                        operation.setReturnValue(str);
                    }
                    break;
                case Type::DataObjectType:
                    {
                        DataObjectPtr* dataObjectData = new DataObjectPtr;
                        *dataObjectData = outputDataObject->getDataObject(pl[i]);
                        if(!*dataObjectData)
                        {
                            loginfo("Null SDO DataObject return value");
                        }
                        operation.setReturnValue(dataObjectData);
                    }
                    break;
                case Type::OpenDataObjectType:
                    {         
                        /*
                         * This code deals with xsd:any element parameters
                         * Get each element as a DataObject and add in to the parameter list
                         */
                        DataObjectList& dataObjectList = outputDataObject->getList(pl[i]);
                        
                        for(int j=0; j<dataObjectList.size(); j++)
                        {
                            DataObjectPtr dob = dataObjectList[j];
                            if(!dob)
                            {
                                DataObjectPtr* dataObjectData = new DataObjectPtr;
                                *dataObjectData = NULL;
                                operation.setReturnValue(dataObjectData);
                                loginfo("Null OpenDataObject return value");
                            }
                            else 
                            {
                                
                                SequencePtr sequence = dob->getSequence();
                                if (sequence->size()!=0)
                                {
                                    // Return a text element        
                                    if (sequence->isText(0))
                                    {                                        
                                        string* stringData = new string(sequence->getCStringValue(0));
                                        operation.setReturnValue(stringData);
                                    }
                                    else
                                    {
                                        // Return a DataObject representing a complex element
                                        DataObjectPtr* dataObjectData = new DataObjectPtr;
                                        *dataObjectData = sequence->getDataObjectValue(0);
                                        if(!*dataObjectData)
                                        {
                                            loginfo("Null DataObject return value");
                                        }
                                        operation.setReturnValue(dataObjectData);
                                    }
                                }
                                else
                                {
                                    // Empty content, add an empty string
                                    loginfo("Null OpenDataObject return value");
                                    string *stringData = new string(""); 
                                    operation.setReturnValue(stringData);
                                }
                            }
                        }
                    }
                    break;
                case Type::DateType:
                    logwarning("SDO DateType return values are not yet supported");
                    break;
                case Type::LongType:
                    logwarning("SDO LongType (int64_t) return values are not yet supported");
                    break;
                case Type::UriType:
                    logwarning("SDO UriType return values are not yet supported");
                    break;
                case Type::BigDecimalType:
                    logwarning("SDO BigDecimalType return values are not yet supported");
                    break;
                case Type::BigIntegerType:
                    logwarning("SDO BigIntegerType return values are not yet supported");
                    break;
                default:
                    logwarning("Unknown SDO type has been found in return value. Unknown types are not yet supported");
                    break;
                }         
             }
             
        } // End namespace rest
    } // End namespace sca
} // End namespace tuscany
