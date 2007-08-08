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

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/Exceptions.h"
#include "tuscany/sca/util/Utils.h"
#include "RESTServiceWrapper.h"
#include "tuscany/sca/core/Operation.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/CompositeReference.h"
#include "model/RESTServiceBinding.h"
#include "tuscany/sca/rest/model/RESTInterface.h"

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
            
            class ResponseChunk {
            public:
                ResponseChunk() : memory(NULL), size(0)
                {
                }
                
                ~ResponseChunk()
                {
                    if (memory)
                    {
                        free(memory);
                    }
                }
            
                char *memory;
                int size;
            };
        
            class RequestChunk {
            public:
                RequestChunk() : memory(NULL), size(0), read(0)
                {
                }
                
                ~RequestChunk()
                {
                }
                
                const char *memory;
                int size;
                int read;
            };
        
            class HeaderChunk {
            public:
                HeaderChunk() : location("")
                {
                }
                
                ~HeaderChunk()
                {
                }
                
                string location;
            };
        
            size_t write_callback(void *ptr, size_t size, size_t nmemb, void *data)
            {
                int realsize = size * nmemb;
                ResponseChunk *mem = (ResponseChunk *)data;
        
                if (mem->memory != NULL)
                {
                    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
                }
                else
                {
                    mem->memory = (char *)malloc(mem->size + realsize + 1);
                }
                memcpy(&(mem->memory[mem->size]), ptr, realsize);
                mem->size += realsize;
                mem->memory[mem->size] = 0;
                return realsize;
            }
            
            static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *data)
            {
                int realsize = size * nmemb;
                RequestChunk *mem = (RequestChunk *)data;
        
                if (realsize > mem->size - mem->read)
                {
                    realsize = mem->size - mem->read;
                }
                if (realsize != 0)
                {
                    memcpy(ptr, &(mem->memory[mem->read]), realsize);
                    mem->read += realsize;
                }
                return realsize; 
            }    
         
            size_t header_callback(void *ptr, size_t size, size_t nmemb, void *data)
            {
                int realsize = size * nmemb;
                HeaderChunk* mem = (HeaderChunk*)data;
                
                char* str = new char[realsize + 1];
                memcpy(str, ptr, realsize);
                str[realsize] = 0;
                
                if (strlen(str) > 10 && !strncmp(str, "Location: ", 10))
                {
                    string s = &str[10];
                    mem->location = s.substr(0,s.find_last_not_of("\r\n")+1);
                }
                
                delete str;
        
                return realsize; 
            }    
         
            bool RESTServiceWrapper::initialized = false;
            
            RESTServiceWrapper::RESTServiceWrapper(Service* service) : ServiceWrapper(service)
            {
                logentry();
                
                DataFactoryPtr dataFactory = service->getComponent()->getComposite()->getDataFactory();
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
                XMLHelperPtr xmlHelper = composite->getXMLHelper();

                // Get the REST binding
                RESTServiceBinding* binding = (RESTServiceBinding *)service->getBinding();
                Interface* iface = service->getType()->getInterface();
                
                // Init the curl session
                CURL *curl_handle = curl_easy_init();

                RequestChunk requestChunk;
                ResponseChunk responseChunk;
                HeaderChunk headerChunk;
                
                // Some servers don't like requests that are made without a user-agent
                curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
                
                // Get the operation name
                string opName = operation.getName();
                
                // If we have a REST interface, the operation name translates to an HTTP verb
                if (iface != NULL && iface->getInterfaceTypeQName() == RESTInterface::typeQName)
                {
    
                    // HTTP GET
                    if (opName == "retrieve")
                    {
                        // Build the request URL
                        bool firstParm = 0;
                        string uri;
                        if (operation.getNParms() !=0)
                        {
                            
                            // If the first parameter is a URI, then we'll use it,
                            // otherwise we'll use the binding URI 
                            ostringstream s0;
                            writeParameter(xmlHelper, s0, operation.getParameter(0));
                            string p0 = s0.str();
                            if (p0.find("://") != string::npos)
                            {
                                firstParm = 1;
                                uri = p0;
                            }
                            else
                            {
                                uri = getBindingURI();
                            }
                        }
                        else
                        {
                            uri = getBindingURI();
                        }
                        // Add the parameters to the end of the URI
                        ostringstream os;
                        if (uri[uri.length()-1] == '?')
                        {
                            // If the URI ends with a "?" then we use the query
                            // form param=value&
                            os << uri;
                            for (unsigned int i = firstParm; i < operation.getNParms(); i++)
                            {
                                Operation::Parameter param = operation.getParameter(i);

                                if(param.hasName())
                                {
                                    os << param.getName() << "=";
                                }
                                else
                                {
                                    // No name - use "param1", etc
                                    os << "param" << (i + 1) << "=";
                                }
                                writeParameter(xmlHelper, os, param);
                                if (i < operation.getNParms()-1)
                                    os << "&";
                            }
                        }
                        else
                        {
                            // Add the parameters in the form
                            // value1 / value2 / value3 
                            os << uri;
                            for (unsigned int i = firstParm; i < operation.getNParms(); i++)
                            {
                                os << "/";
                                writeParameter(xmlHelper, os, operation.getParameter(i));
                            }
                        }
    
                        string url = os.str(); 
                        //loginfo("RESTServiceWrapper: HTTP GET %s", url.c_str());
                        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
     
                        // Send all data to this function
                        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
     
                        // Pass our 'chunk' struct to the callback function
                        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&responseChunk);
     
                        // Send all headers to this function
                        curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, header_callback);
     
                        // Pass our 'chunk' struct to the callback function
                        curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, (void *)&headerChunk);
     
                        // Perform the HTTP GET
                        CURLcode rc = curl_easy_perform(curl_handle);
                        
                        if (rc)
                        {
                            throwException(ServiceInvocationException, curl_easy_strerror(rc)); 
                        }
    
                        // Get the output data out of the returned document
                        long httprc;
                        curl_easy_getinfo (curl_handle, CURLINFO_RESPONSE_CODE, &httprc);

                        string responsePayload = ""; 
                        if (responseChunk.memory)
                        {
                            responsePayload = string((const char*)responseChunk.memory, responseChunk.size);
                        }
        
                        if (httprc == 200)
                        {
                            if (responsePayload != "")
                            {
                                //TODO Remove this workaround once SDO supports loading of open top level content
                                // The workaround is to wrap the open content in a wrapper element
                                string xmldecl;
                                string xml;
                                Utils::rTokeniseString("?>", responsePayload, xmldecl, xml);
                                string part = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
                                part += "<Part xmlns=\"http://tempuri.org\" xmlns:tns=\"http://tempuri.org\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n";
                                part += xml;
                                part +=  "\n</Part>";
                                
                                setReturn(xmlHelper, part, operation);
                            }
                            else
                            {
                                throwException(ServiceInvocationException, "Failed to retrieve resource, empty response");
                            }
                        }
                        else
                        {
                            ostringstream msg;
                            msg << "Failed to retrieve REST resource, HTTP code: " << httprc;
                            if (responsePayload != "")
                            {
                                msg << ", payload: " << responsePayload;
                            } 
                            throwException(ServiceInvocationException, msg.str().c_str());
                        }
                    }
                    else if (opName == "create")
                    {
                        // HTTP POST
                        
                        // Set the target URL
                        string url = getBindingURI();  
                        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    
                        // Create the input payload     
                        ostringstream spayload;
                        writeParameter(xmlHelper, spayload, operation.getParameter(0));
                        const string& requestPayload = spayload.str(); 
                        requestChunk.memory = requestPayload.c_str();
                        requestChunk.size = requestPayload.size();
                        
                        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, requestChunk.size);
                        
                        // Read all data using this function
                        curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION, read_callback);
     
                        // Pass our 'chunk' struct to the callback function
                        curl_easy_setopt(curl_handle, CURLOPT_READDATA, (void *)&requestChunk);
                        
                        // Send all data to this function
                        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
     
                        // Pass our 'chunk' struct to the callback function
                        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&responseChunk);
     
                        // Send all headers to this function
                        curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, header_callback);
     
                        // Pass our 'chunk' struct to the callback function
                        curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, (void *)&headerChunk);

                        // Configure headers
                        curl_slist *requestHeaders = NULL;
                        requestHeaders = curl_slist_append(requestHeaders, "Expect:");
                        requestHeaders = curl_slist_append(requestHeaders, "Content-Type: text/xml");
                        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, requestHeaders);
                                                
                        // Perform the HTTP POST
                        curl_easy_setopt(curl_handle, CURLOPT_POST, true);
                        CURLcode rc = curl_easy_perform(curl_handle);
                        
                        curl_slist_free_all(requestHeaders);
                        
                        if (rc)
                        {
                            throwException(ServiceInvocationException, curl_easy_strerror(rc)); 
                        }
                        
                        // Get the output and location of the created resource
                        string responsePayload = "";
                        if (responseChunk.memory)
                        {
                            responsePayload = string((const char*)responseChunk.memory, responseChunk.size);
                        }
                        
                        long httprc;
                        curl_easy_getinfo (curl_handle, CURLINFO_RESPONSE_CODE, &httprc);
                        if (httprc == 201)
                        {
                            string* location = new string;
                            *location = headerChunk.location;
                            operation.setReturnValue(location);
                        }
                        else
                        {
                            ostringstream msg;
                            msg << "Failed to create REST resource, HTTP code: " << httprc;
                            if (responsePayload != "")
                            {
                                msg << ", payload: " << responsePayload;
                            } 
                            throwException(ServiceInvocationException, msg.str().c_str());
                        }
                    }
                    else if (opName == "update")
                    {
                        // HTTP PUT
                        
                        // Build the request URL
                        bool firstParm = 0;
                        string uri;
                        if (operation.getNParms() > 1)
                        {
                            
                            // If the first parameter is a URI, then we'll use it,
                            // otherwise we'll use the binding URI 
                            ostringstream s0;
                            writeParameter(xmlHelper, s0, operation.getParameter(0));
                            string p0 = s0.str();
                            if (p0.find("://") != string::npos)
                            {
                                firstParm = 1;
                                uri = p0;
                            }
                            else
                            {
                                uri = getBindingURI();
                            }
                        }
                        else
                        {
                            uri = getBindingURI();
                        }
                        // Add the parameters to the end of the URI
                        ostringstream os;
                        if (uri[uri.length()-1] == '?')
                        {
                            // If the URI ends with a "?" then we use the query
                            // form param=value&
                            os << uri;
                            for (unsigned int i = firstParm; i < operation.getNParms()-1; i++)
                            {
                                Operation::Parameter param = operation.getParameter(i);
                                if(param.hasName())
                                {
                                    os << param.getName() << "=";
                                }
                                else
                                {
                                    // No name - use "param1", etc
                                    os << "param" << (i + 1) << "=";
                                }
                                writeParameter(xmlHelper, os, param);
                                if (i < operation.getNParms()-1)
                                    os << "&";
                            }
                        }
                        else
                        {
                            // Add the parameters in the form
                            // value1 / value2 / value3 
                            os << uri;
                            for (unsigned int i = firstParm; i < operation.getNParms()-1; i++)
                            {
                                os << "/";
                                writeParameter(xmlHelper, os, operation.getParameter(i));
                            }
                        }
    
                        string url = os.str();                                        
                        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    
                        // Create the input payload     
                        ostringstream spayload;
                        writeParameter(xmlHelper, spayload, operation.getParameter(operation.getNParms()-1));
                        const string& requestPayload = spayload.str(); 
                        requestChunk.memory = requestPayload.c_str();
                        requestChunk.size = requestPayload.size();
                        
                        // Read all data using this function
                        curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION, read_callback);
     
                        // Pass our 'chunk' struct to the callback function
                        curl_easy_setopt(curl_handle, CURLOPT_READDATA, (void *)&requestChunk);
                        
                        // Send all data to this function
                        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
     
                        // Pass our 'chunk' struct to the callback function
                        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&responseChunk);
     
                        // Send all headers to this function
                        curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, header_callback);
     
                        // Pass our 'chunk' struct to the callback function
                        curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, (void *)&headerChunk);

                        // Configure headers
                        curl_slist *requestHeaders = NULL;
                        requestHeaders = curl_slist_append(requestHeaders, "Expect:");
                        requestHeaders = curl_slist_append(requestHeaders, "Content-Type: text/xml");
                        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, requestHeaders);
                                                
                        // Perform the HTTP PUT
                        curl_easy_setopt(curl_handle, CURLOPT_PUT, true);
                        curl_easy_setopt(curl_handle, CURLOPT_UPLOAD, true) ;
                        long size = (long)requestChunk.size;
                        curl_easy_setopt(curl_handle, CURLOPT_INFILESIZE, size);
                        
                        CURLcode rc = curl_easy_perform(curl_handle);
                        
                        curl_slist_free_all(requestHeaders);
                        
                        if (rc)
                        {
                            throwException(ServiceInvocationException, curl_easy_strerror(rc)); 
                        }
                        
                        // Get the output and location of the created resource
                        string responsePayload = "";
                        if (responseChunk.memory)
                        {
                            responsePayload = string((const char*)responseChunk.memory, responseChunk.size);
                        }
                        
                        long httprc;
                        curl_easy_getinfo (curl_handle, CURLINFO_RESPONSE_CODE, &httprc);
                        if (httprc != 200)
                        {
                            ostringstream msg;
                            msg << "Failed to update REST resource, HTTP code: " << httprc;
                            if (responsePayload != "")
                            {
                                msg << ", payload: " << responsePayload;
                            } 
                            throwException(ServiceInvocationException, msg.str().c_str());
                        }
                    }
                    else if (opName == "delete")
                    {
                        // HTTP DELETE

                        // Build the request URL
                        bool firstParm = 0;
                        string uri;
                        if (operation.getNParms() !=0)
                        {
                            
                            // If the first parameter is a URI, then we'll use it,
                            // otherwise we'll use the binding URI 
                            ostringstream s0;
                            writeParameter(xmlHelper, s0, operation.getParameter(0));
                            string p0 = s0.str();
                            if (p0.find("://") != string::npos)
                            {
                                firstParm = 1;
                                uri = p0;
                            }
                            else
                            {
                                uri = getBindingURI();
                            }
                        }
                        else
                        {
                            uri = getBindingURI();
                        }
                        // Add the parameters to the end of the URI
                        ostringstream os;
                        if (uri[uri.length()-1] == '?')
                        {
                            // If the URI ends with a "?" then we use the query
                            // form param=value&
                            os << uri;
                            for (unsigned int i = firstParm; i < operation.getNParms(); i++)
                            {
                                Operation::Parameter param = operation.getParameter(i);

                                if(param.hasName())
                                {
                                    os << param.getName() << "=";
                                }
                                else
                                {
                                    // No name - use "param1", etc
                                    os << "param" << (i + 1) << "=";
                                }
                                writeParameter(xmlHelper, os, param);
                                if (i < operation.getNParms()-1)
                                    os << "&";
                            }
                        }
                        else
                        {
                            // Add the parameters in the form
                            // value1 / value2 / value3 
                            os << uri;
                            for (unsigned int i = firstParm; i < operation.getNParms(); i++)
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
                        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&responseChunk);
     
                        // Send all headers to this function
                        curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, header_callback);
     
                        // Pass our 'chunk' struct to the callback function
                        curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, (void *)&headerChunk);
     
                        // Perform the HTTP DELETE
                        curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "DELETE");
                        CURLcode rc = curl_easy_perform(curl_handle);
                        if (rc)
                        {
                            throwException(ServiceInvocationException, curl_easy_strerror(rc)); 
                        }
    
                        // Get the output data out of the returned document
                        string responsePayload = ""; 
                        if (responseChunk.memory)
                        {
                            responsePayload = string((const char*)responseChunk.memory, responseChunk.size);
                        }
        
                        long httprc;
                        curl_easy_getinfo (curl_handle, CURLINFO_RESPONSE_CODE, &httprc);
                        if (httprc != 200)
                        {
                            ostringstream msg;
                            msg << "Failed to delete REST resource, HTTP code: " << httprc;
                            if (responsePayload != "")
                            {
                                msg << ", payload: " << responsePayload;
                            } 
                            throwException(ServiceInvocationException, msg.str().c_str());
                        }
                    }
                    else
                    {
                            string msg = "Unknown REST verb: " + opName;
                            throwException(ServiceInvocationException, msg.c_str());
                    }
                }
                else
                {
                    // Not a REST interface, XML / HTTP command style
                    curl_slist *requestHeaders = NULL;
                    struct curl_httppost *formpost = NULL;
                    ostringstream spayload;
                    string requestPayload;
                    string url; 
                    
                    // If the request contains complex content then we'll use
                    // a POST, otherwise we use a GET with a query string
                    bool complexContent = false; 
                    for (unsigned int i=0; i<operation.getNParms(); i++)
                    {
                        if (operation.getParameter(i).getType() == Operation::DATAOBJECT)
                        {
                            complexContent = true;
                            break;
                        }
                    }

                    if (complexContent)
                    {
                       // Set the target URL
                        string uri = getBindingURI();
                        ostringstream os;
                        os << uri << "/" << opName;
                        url = os.str();                                        
                        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
                        
                        // Disable the 100 continue handshake
                        requestHeaders = curl_slist_append(requestHeaders, "Expect:");
                            
                        if (operation.getNParms() == 1)
                        {
                            // Single parameter, send it as the body of the POST
                            
                            // Create the input payload     
                            writeParameter(xmlHelper, spayload, operation.getParameter(0));
                            requestPayload = spayload.str(); 
                            requestChunk.memory = requestPayload.c_str();
                            requestChunk.size = requestPayload.size();
                            
                            curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, requestChunk.size);
                            
                            // Read all data using this function
                            curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION, read_callback);
         
                            // Pass our 'chunk' struct to the callback function
                            curl_easy_setopt(curl_handle, CURLOPT_READDATA, (void *)&requestChunk);

                            // Set the content type
                            requestHeaders = curl_slist_append(requestHeaders, "Content-Type: text/xml");

                            // This will be a POST                        
                            curl_easy_setopt(curl_handle, CURLOPT_POST, true);
                        }
                        else
                        {
    
                            // Multiple parameters, use a form type POST
                            struct curl_httppost *lastptr = NULL;
                            for (unsigned int i=0; i<operation.getNParms(); i++)
                            {
                                ostringstream pname;
                                Operation::Parameter param = operation.getParameter(i);

                                if(param.hasName())
                                {
                                    pname << param.getName();
                                }
                                else
                                {
                                    // No name - use "param1", etc
                                    pname << "param" << (i+1);
                                }                                
                                
                                const char* ctype;
                                if (param.getType() == Operation::DATAOBJECT)
                                {
                                    ctype ="text/xml"; 
                                }
                                else
                                {
                                    ctype = "text/plain";
                                }
                                
                                ostringstream pvalue;
                                writeParameter(xmlHelper, pvalue, param);
                                
                                curl_formadd(&formpost,
                                    &lastptr,
                                    CURLFORM_CONTENTTYPE, ctype,
                                    CURLFORM_COPYNAME, pname.str().c_str(),
                                    CURLFORM_COPYCONTENTS, pvalue.str().c_str(),
                                    CURLFORM_END);
                            }

                            // Set the form into the request
                            curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, formpost);
                        }
                    }
                    else
                    {

                        // Build the request URL, uri / opName ? params
                        string uri = getBindingURI();
                        ostringstream os;
                        os << uri << "/" << opName;
    
                        // Add the parameters to the end of the URL in the form
                        // param=value&
                        for (unsigned int i=0; i<operation.getNParms(); i++)
                        {
                            if (i == 0)
                                os << "?";
                            
                            Operation::Parameter param = operation.getParameter(i);
                            if(param.hasName())
                            {
                                os << param.getName() << "=";
                            }
                            else
                            {
                                // No name - use "param1", etc
                                os << "param" << (i + 1) << "=";
                            }
                            writeParameter(xmlHelper, os, param);
                            if (i < operation.getNParms()-1)
                                os << "&";
                        }
    
                        url = os.str();
                        loginfo("RESTServiceWrapper: HTTP GET %s", url.c_str());
                                                                
                        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
                    }
                    
                    // Send all data to this function
                    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
 
                    // Pass our 'chunk' struct to the callback function
                    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&responseChunk);
 
                    // Configure the headers
                    if (requestHeaders)
                    {
                        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, requestHeaders);
                    }
                    
                    loginfo("RESTServiceWrapper: Performing HTTP request");
                    // Perform the HTTP request
                    CURLcode rc = curl_easy_perform(curl_handle);

                    loginfo("RESTServiceWrapper: Completed HTTP request");
                    // Free any headers
                    if (requestHeaders)
                    {
                        curl_slist_free_all(requestHeaders);
                    }
                        
                    // Cleanup the form
                    if (complexContent)
                    {
                        curl_formfree(formpost);
                    }                    
                    
                    if (rc)
                    {
                        throwException(ServiceInvocationException, curl_easy_strerror(rc)); 
                    }
 
                    if (responseChunk.memory)
                    {
                        string responsePayload((const char*)responseChunk.memory, responseChunk.size);
                        

                        loginfo("RESTServiceWrapper: responsePayload: %s", responsePayload.c_str());

                        //TODO Remove this workaround once SDO supports loading of open top level content
                        // The workaround is to wrap the open content in a wrapper element
                        string xmldecl;
                        string xml;
                        Utils::rTokeniseString("?>", responsePayload, xmldecl, xml);
                        string part = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
                        part += "<Part xmlns=\"http://tempuri.org\" xmlns:tns=\"http://tempuri.org\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n";
                        part += xml;
                        part +=  "\n</Part>";
                        
                        setReturn(xmlHelper, part, operation);
                    }
                }
                
                // Cleanup curl session
                curl_easy_cleanup(curl_handle);
            }
            

            const string RESTServiceWrapper::getBindingURI()
            {
                string bindingURI = "";
                
                // Get the binding URI configured on the top level component 
                Service* service = getService();
                CompositeReference* compositeReference = (CompositeReference*)service->getComponent();
                SCARuntime* runtime = SCARuntime::getCurrentRuntime();
                Component* component = runtime->getDefaultComponent();
                Reference* reference = component->findReference(compositeReference->getName());
                if (reference != NULL)
                {
                    ReferenceBinding* binding = reference->getBinding();
                    if (binding != NULL && binding->getURI() != "")
                    {
                        bindingURI = binding->getURI();
                    } 
                }
                if (bindingURI == "")
                {
                    // Get the binding URI configured on the binding 
                    RESTServiceBinding* binding = (RESTServiceBinding *)service->getBinding();
                    bindingURI = binding->getURI();
                }
                if (bindingURI != "")
                {
                    
                    // Prepend the default base URI if the URI is not absolute
                    if (bindingURI.find("://") == string::npos)
                    {
                        bindingURI = runtime->getDefaultBaseURI() + string("/rest/") + bindingURI;
                    }
                }
                return bindingURI;
            }
            
            
            void RESTServiceWrapper::writeParameter(XMLHelperPtr xmlHelper, ostringstream& os, const Operation::Parameter& parm)
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
                        break;
                    }
                }
            }

             void RESTServiceWrapper::setReturn(XMLHelperPtr xmlHelper, string& payload,
                 Operation& operation)
             {  
                logentry();
                
                //TODO Remove this workaround once SDO supports loading of open top level content
                // The workaround is to wrap the open content in a wrapper element
                string xmldecl;
                string xml;
                Utils::rTokeniseString("?>", payload, xmldecl, xml);
                string body = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
                body += "<Wrapper xmlns=\"http://tempuri.org\" xmlns:tns=\"http://tempuri.org\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n";
                body += xml;
                body +=  "\n</Wrapper>";
                
                // Convert the body to an SDO DataObject
                DataObjectPtr outputWrapperDataObject = NULL;
                XMLDocumentPtr theXMLDocument = xmlHelper->load(body.c_str(), NULL);
                if (theXMLDocument != 0)
                {
                    outputWrapperDataObject = theXMLDocument->getRootDataObject();
                }
                if(!outputWrapperDataObject)
                {
                    ostringstream msg;
                    msg << "Could not convert received document to SDO: " << body;
                    throwException(ServiceDataException, msg.str().c_str());
                }                    

                // Get the body part
                DataObjectPtr outputDataObject = NULL; 
                PropertyList bpl = outputWrapperDataObject->getInstanceProperties();
                if (bpl.size()!=0)
                {
                    if (bpl[0].isMany())
                    {
                        DataObjectList& parts = outputWrapperDataObject->getList((unsigned int)0);
                        outputDataObject = parts[0];
                    }
                    else
                    {
                        outputDataObject = outputWrapperDataObject->getDataObject(bpl[0]);
                    }
                }
                if (outputDataObject == NULL)
                {
                    ostringstream msg;
                    msg << "Could not convert body part to SDO: " << body;
                    throwException(ServiceDataException, msg.str().c_str());
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
                case Type::IntType:
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
                        else
                        {
                            (*dataObjectData)->detach();
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
                        
                        for(unsigned int j=0; j<dataObjectList.size(); j++)
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
                                        else
                                        {
                                            (*dataObjectData)->detach();
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
                default:
                    {
                        ostringstream msg;
                        msg << "Unsupported result type: " << pl[i].getTypeEnum();
                        throwException(ServiceDataException, msg.str().c_str());
                    }
                }
             }
             
        } // End namespace rest
    } // End namespace sca
} // End namespace tuscany
