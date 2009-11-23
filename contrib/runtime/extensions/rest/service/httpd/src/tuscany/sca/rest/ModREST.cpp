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

#include <sstream>

#include "apr_strings.h"
#include "apr_fnmatch.h"
#include "apr_lib.h"

#define APR_WANT_STRFUNC
#include "apr_want.h"
#include "ap_config.h"
#include "httpd.h"

// This section removes the windows/httpd build error "strtoul_is_not_a_portable_function_use_strtol_instead"
#if defined(WIN32)  || defined (_WINDOWS)
#ifdef strtoul
#undef strtoul
#endif
#define strtoul strtoul
#endif

#include "http_config.h"
#include "http_core.h"
#include "http_request.h"
#include "http_protocol.h"
#include "http_log.h"
#include "http_main.h"
#include "util_script.h"
#include "util_md5.h"

#include "mod_core.h"

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/core/Exceptions.h"
#include "tuscany/sca/util/Logging.h"
#include "RESTServiceProxy.h"
#include "model/RESTReferenceBinding.h"
#include "tuscany/sca/rest/model/RESTInterface.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/CompositeService.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/model/WSDLDefinition.h"
#include "tuscany/sca/model/WSDLOperation.h"
#include "tuscany/sca/model/WSDLMessagePart.h"
#include "tuscany/sca/model/WSDLInterface.h" 
#include "tuscany/sca/model/Interface.h" 
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/util/Utils.h"

using namespace std;
using namespace commonj::sdo;
using namespace tuscany::sca;
using namespace tuscany::sca::model;
using namespace tuscany::sca::util;


extern "C"
{                                                                                   
    extern module AP_MODULE_DECLARE_DATA sca_rest_module;
}

namespace tuscany
{
    namespace sca
    {
        namespace rest
        {
            
            typedef struct rest_server_config_rec
            {
                char * home;
            } rest_server_config_rec_t;
            
            typedef struct rest_dir_config_rec
            {
                char * root;
                char * path;
                char * base_uri;
                char * component;
            } rest_dir_config_rec_t;
            
            CompositeService* initializeSCARuntime(const char* home, const char* root,
                const char* path, const char* baseURI, const char *component, const char* service);
            
            DataObjectPtr createPayload(DataFactoryPtr dataFactory,
                Operation& operation, const WSDLOperation& wsdlOperation);

            void addPart(XMLHelperPtr xmlHelper, string& payload, Operation& operation);
             
            int logHeaders(void* request, const char* key, const char* value);

            /**
             * Initialize the SCA runtime
             */
            CompositeService* initializeSCARuntime(const char* home, const char* root,
                const char* path, const char* baseURI, const char *component, const char* service)
            {
                logentry();
                loginfo("Home: %s", home);
                loginfo("Root: %s", root);
                loginfo("Path: %s", path);
                loginfo("Base URI: %s", baseURI);
                loginfo("Component: %s", component);
                loginfo("Service: %s", service);
                
                try
                {
                    SCARuntime* runtime = SCARuntime::initializeSharedRuntime(home, root, path, baseURI);

                    string componentName;
                    if (strlen(component))
                    {
                        componentName = component;
                    }
                    else
                    {
                        componentName = runtime->getDefaultComponentName();
                    }
                    string serviceName = service;
                    
                    loginfo("Resolving composite: %s, service: %s", componentName.c_str(), serviceName.c_str());
                    Component* compositeComponent = runtime->getSystem()->findComponent(componentName);
                    if (compositeComponent == NULL)
                    {
                        string msg = "Component not found " + componentName;
                        throwException(SystemConfigurationException, msg.c_str());
                    }
                    runtime->setDefaultComponent(compositeComponent);

                    Composite* composite = (Composite*)compositeComponent->getType();
                    CompositeService* compositeService = (CompositeService*)composite->findComponent(serviceName);
                    if (compositeService == NULL)
                    {
                        string msg = "Composite service not found " + serviceName;
                        throwException(SystemConfigurationException, msg.c_str());
                    }
                
                    return compositeService;
                }
                catch(TuscanyRuntimeException &ex)
                {
                    ostringstream msg;
                    msg << ex;
                    logerror("Failed to initialize SCA runtime: %s", msg.str().c_str());
                    throw;
                }
            }
            
            bool printRequest = false;
            
            int logHeaders(void* request, const char* key, const char* value)
            {
                loginfo("Header key: %s, value: %s", key, value);
                if (printRequest)
                {
                    ap_rprintf((request_rec*)request, "<br>Header key: %s, value: %s", key, value);
                }
                return 1;
            }
            
            int rest_handler(request_rec *request)
            {
                logentry();
                
                if (strcmp(request->handler, "sca_rest_module"))
                {
                    return DECLINED;
                }
                
                try {
                    
                    // Set up the read policy
                    int rc = ap_setup_client_block(request, REQUEST_CHUNKED_DECHUNK);
                    if (rc != OK)
                    {
                        return rc;
                    }
                    ap_should_client_block(request);
                    if (request->read_chunked == true && request->remaining == 0)
                    {
                        request->chunked = true;
                    }
                    
                    apr_table_setn(request->headers_out, "Connection", "close");
                
                    if (printRequest)
                    {
                        // Set the content type
                        ap_set_content_type(request, "text/html");
                        
                        // Send the response document
                        ap_rputs("<html><body><p>Tuscany Mod_rest works!", request);
                    }
                
                    rest_server_config_rec_t* server_conf = (rest_server_config_rec_t*)ap_get_module_config(request->server->module_config, &sca_rest_module);
                    loginfo("Tuscany home: %s", server_conf->home);
                    if (printRequest)
                    {
                        ap_rprintf(request, "<p>Tuscany home: %s", server_conf->home);
                    }
                
                    rest_dir_config_rec_t* dir_conf = (rest_dir_config_rec_t*)ap_get_module_config(request->per_dir_config, &sca_rest_module);
                    loginfo("Tuscany root: %s", dir_conf->root);
                    if (printRequest)
                    {
                        ap_rprintf(request, "<p>Tuscany root: %s", dir_conf->root);
                    }
                    loginfo("Tuscany path: %s", dir_conf->path);
                    if (printRequest)
                    {
                        ap_rprintf(request, "<p>Tuscany path: %s", dir_conf->path);
                    }
                    loginfo("SCA component: %s", dir_conf->component);
                    if (printRequest)
                    {
                        ap_rprintf(request, "<p>SCA component: %s", dir_conf->component);
                    }
                    
                    if (request->protocol)
                    {
                        loginfo("Protocol: %s", request->protocol);
                        if (printRequest)
                        {
                            ap_rprintf(request, "<p>Protocol: %s", request->protocol);
                        }
                    }
                    
                    if (request->method)
                    {
                        loginfo("HTTP method: %s", request->method);
                        if (printRequest)
                        {
                            ap_rprintf(request, "<p>HTTP method: %s", request->method);
                        }
                    }
                
                    loginfo("HTTP method number: %d", request->method_number);
                    if (printRequest)
                    {
                        ap_rprintf(request, "<p>HTTP method number: %d", request->method_number);
                    }
                    
                    const char* content_type = apr_table_get(request->headers_in, "Content-Type");
                    if (content_type)
                    {
                        loginfo("Content type: %s", content_type);
                        if (printRequest)
                        {
                            ap_rprintf(request, "<p>Content type: %s", content_type);
                        }
                    }
                    else
                    {
                        content_type = "text/plain";
                    }
                
                    if (request->content_encoding)
                    {
                        loginfo("Content encoding: %s", request->content_encoding);
                        if (printRequest)
                        {
                            ap_rprintf(request, "<p>Content encoding: %s", request->content_encoding);
                        }
                    }
                
                    if (printRequest)
                    {
                        ap_rputs("<p>", request);
                    }
                    apr_table_do(logHeaders, request, request->headers_in, NULL);
                    
                    if (request->uri)
                    {
                        loginfo("URI: %s", request->uri);
                        if (printRequest)
                        {
                            ap_rprintf(request, "<p>URI: %s", request->uri);
                        }
                    }
                    
                    if (request->path_info)
                    {
                        loginfo("Path info: %s", request->path_info);
                        if (printRequest)
                        {
                            ap_rprintf(request, "<p>Path info: %s", request->path_info);
                        }
                    }
                    
                    if (request->args)
                    {
                        loginfo("Args: %s", request->args);
                        if (printRequest)
                        {
                            ap_rprintf(request, "<p>Args: %s", request->args);
                        }
                    }
                    
                    if (printRequest)
                    {
                        ap_rputs("</body></html>", request);
                    }
                    
                    // Extract the service and component names from the HTTP URI path
                    string path;
                    if (strlen(request->path_info) != 0 && *(request->path_info) == '/')
                    {
                        path = request->path_info + 1;
                    }
                    else
                    {
                        path = request->path_info;
                    }
                    string uri;
            
                    string component;
                    string service;        
                    if (strlen(dir_conf->component))
                    {
                        // The path only specifies the service, the component name
                        // is configured in the directory/location configured
                        component = dir_conf->component;
                        Utils::tokeniseString("/", path, service, uri); 
                    }
                    else
                    {
                        // The path must be in the form component / service
                        string path2;
                        Utils::tokeniseString("/", path, component, path2);
                        Utils::tokeniseString("/", path2, service, uri);
                    }
            
                    loginfo("Component name: %s", component.c_str());
                    loginfo("Service name: %s", service.c_str());
                    
                    // Initialize the SCA runtime
                    CompositeService* compositeService = initializeSCARuntime(
                                server_conf->home, dir_conf->root, dir_conf->path, dir_conf->base_uri, component.c_str(), service.c_str());
                                
                    if(!compositeService)
                    {
                        throwException(SystemConfigurationException,
                            "Failed to initialize SCA runtime, could not initialize CompositeService");
                    }
        
                    Composite* composite = compositeService->getComposite();
                    DataFactoryPtr dataFactory = composite->getDataFactory();
                    if (dataFactory == 0)
                    {
                        throwException(SystemConfigurationException,
                            "Failed to initialize SCA runtime, could not get DataFactory");
                    }
                    XMLHelperPtr xmlHelper = composite->getXMLHelper();
        
                    // Get the REST binding
                    Reference* reference = compositeService->getReference();
                    RESTReferenceBinding* binding = (RESTReferenceBinding*)reference->getBinding();
                    
                    //  Get the REST proxy
                    RESTServiceProxy* proxy = (RESTServiceProxy*)binding->getServiceProxy();
        
                    // Get the component interface
                    Interface* iface = reference->getType()->getInterface();

                    if (request->method_number == M_GET)
                    {
                        // Handle an HTTP GET
                        
                        // Determine the operation to invoke                    
                        WSDLOperation wsdlOperation;
                        string wsdlNamespace = "";
                        string op_name = "";
                        string uriArgs = "";
                        if (iface != NULL)
                        {
                            // If we have a REST interface, the operation name is "retrieve"
                            if (iface->getInterfaceTypeQName() == RESTInterface::typeQName)
                            {
                                op_name = "retrieve";
                                uriArgs = uri;
                            }
                            else if (iface->getInterfaceTypeQName() == WSDLInterface::typeQName)
                            {
                                // we have a WSDL interface, the operation name is part of the URI
                                Utils::tokeniseString("/", uri, op_name, uriArgs);
                                
                                // look for the WSDL operation definition
                                WSDLInterface* wsdlInterface = (WSDLInterface*)iface;
                                wsdlNamespace = wsdlInterface->getNamespaceURI();
                                
                                if (wsdlNamespace != "")
                                {
                                    WSDLDefinition* wsdl = composite->findWSDLDefinition(wsdlNamespace);
                                    if (wsdl == 0)
                                    {
                                        string msg = "WSDL not found for: " + wsdlNamespace;
                                        throwException(SystemConfigurationException, msg.c_str());
                                    }
                                    try
                                    {
                                        wsdlOperation = wsdl->findOperation(wsdlInterface->getName(), op_name.c_str());
                                    }
                                    catch(SystemConfigurationException&)
                                    {   
                                        throw;
                                    }
                                    
                                    if (!wsdlOperation.isDocumentStyle() || !wsdlOperation.isWrappedStyle())
                                    {
                                        throwException(ServiceInvocationException,
                                            "Only wrapped document style WSDL operations are currentlysupported");
                                    }
                                }
                            }
                        }
                        else
                        {
                            Utils::tokeniseString("/", uri, op_name, uriArgs);
                        }
                    
                        // Create a default document literal wrapped WSDL operation
                        if (wsdlNamespace == "")
                        {
                            WSDLMessagePart inPart(op_name, "", "http://tempuri.org");
                            WSDLMessagePart outPart((op_name+"Response"), "", "http://tempuri.org");
                            wsdlNamespace = compositeService->getName();
                            wsdlOperation = WSDLOperation();
                            wsdlOperation.setOperationName(op_name.c_str());
                            wsdlOperation.setSoapAction(wsdlNamespace+ "#" +op_name);
                            wsdlOperation.setEndpoint("");
                            wsdlOperation.setSoapVersion(WSDLOperation::SOAP11);
                            wsdlOperation.setDocumentStyle(true);
                            wsdlOperation.setWrappedStyle(true);
                            wsdlOperation.setInputEncoded(false);
                            wsdlOperation.setOutputEncoded(false);
                            wsdlOperation.setInputMessagePart(op_name, inPart);
                            wsdlOperation.setOutputMessagePart((op_name+"Response"), outPart);
                        }
                        
                        // Create the input DataObject
                        Operation operation(op_name.c_str());
                        
                        // Parse the args part of the URI
                        if (uriArgs != "")
                        {
                            string args = uriArgs;
                            for (; args != ""; )
                            {
                                string param;
                                string next;
                                Utils::tokeniseString("/", args, param, next);
                                if (param != "")
                                {
                                    string* data = new string;
                                    *data = param;
                                    operation.addParameter(data);    
                                }
                                args = next;
                            }
                        }                        
                        
                        // Parse the query string
                        if (request->args)
                        {
                            string query = request->args;
                            for (; query != ""; )
                            {
                                string param;
                                string next;
                                Utils::tokeniseString("&", query, param, next);
                                if (param != "")
                                {
                                    string n;
                                    string* data = new string;
                                    Utils::tokeniseString("=", param, n, *data);
                                    operation.addParameter(data);    
                                }
                                query = next;
                            }
                        }
                        DataObjectPtr inputDataObject = createPayload(dataFactory, operation, wsdlOperation);
                        
                        // Dispatch to the REST proxy
                        DataObjectPtr outputDataObject = proxy->invoke(wsdlOperation, inputDataObject);
                        
                        // Send the output DataObject
                        if (iface!=NULL &&
                            iface->getInterfaceTypeQName() == RESTInterface::typeQName)
                        {
                            if (outputDataObject == NULL)
                            {
                                throwException(ServiceInvocationException, "Null output from REST create operation");
                            }
                            else
                            {
                                    
                                // Pure REST, send the response document
                                XMLHelperPtr xm = HelperProvider::getXMLHelper(dataFactory);
                                DataObjectList& l = outputDataObject->getList("return");
                                if (l.size() != 0)
                                {
                                    DataObjectPtr resourceDataObject = l[0];
                                    XMLDocumentPtr doc = xm->createDocument(
                                        resourceDataObject,
                                        resourceDataObject->getType().getURI(),
                                        resourceDataObject->getType().getName());
                                    char* str = xm->save(doc);
                                    
                                    // Calculate an Etag hash for the response
                                    char* etag = ap_md5(request->pool, (const unsigned char*)str);
                                    
                                    // Handle a conditional GET, if the etag matches the etag
                                    // sent by the client, we don't need to send the whole response
                                    const char* match = apr_table_get(request->headers_in, "If-None-Match");
                                    if (match != NULL && !strcmp(etag, match))
                                    {
                                        loginfo("REST resource matches ETag, sending HTTP 304 response code");
                                        request->status = HTTP_NOT_MODIFIED;
                                    }
                                    else
                                    {
                                        loginfo("Sending response: %s", str);
                                        ap_set_content_type(request, "text/xml");
                                        apr_table_setn(request->headers_out, "ETag", etag);
                                        
                                        // Send an Etag header to allow caching and
                                        // conditional gets
                                        apr_table_setn(request->headers_out, "ETag", etag);
                                        
                                        ap_rputs(str, request);
                                    }
                                }
                                else
                                {
                                    loginfo("REST resource not found, sending HTTP 404 response code");
                                    request->status = HTTP_NOT_FOUND;
                                    
                                    return OK;
                                }
                            }
                        }
                        else
                        {
                            // Command style, send the response wrapper element

                            if (outputDataObject == NULL)
                            {
                                loginfo("Sending empty response");
                                //request->status = HTTP_NO_CONTENT;
                            }
                            else
                            {
                                XMLHelperPtr xm = HelperProvider::getXMLHelper(dataFactory);
                                DataObjectList& l = outputDataObject->getList("return");
                                if (l.size() != 0)
                                {
                                    DataObjectPtr resultDataObject = l[0];
                                    XMLDocumentPtr doc = xm->createDocument(
                                        resultDataObject,
                                        resultDataObject->getType().getURI(),
                                        resultDataObject->getType().getName());
                                    char* str = xm->save(doc);
    
                                    loginfo("Sending response: %s", str);
                                    ap_set_content_type(request, "text/xml");
                                    ap_rputs(str, request);
                                }
                                else
                                {
                                    loginfo("Sending empty response");
                                    //request->status = HTTP_NO_CONTENT;
                                }
                            }
                        }
                        
                        return OK;
                    }
                    else if (request->method_number == M_POST)
                    {
                        // Handle an HTTP POST
                        
                        // Determine the operation to invoke                    
                        WSDLOperation wsdlOperation;
                        string wsdlNamespace = "";
                        string op_name = "";
                        string uriArgs = "";
                        if (iface != NULL)
                        {
                            // If we have a REST interface, the operation name is "create"
                            if (iface->getInterfaceTypeQName() == RESTInterface::typeQName)
                            {
                                op_name = "create";
                            }
                            else if (iface->getInterfaceTypeQName() == WSDLInterface::typeQName)
                            {
                                // we have a WSDL interface, the operation name is part of the URI
                                Utils::tokeniseString("/", uri, op_name, uriArgs);
                                
                                // look for the WSDL operation definition
                                WSDLInterface* wsdlInterface = (WSDLInterface*)iface;
                                wsdlNamespace = wsdlInterface->getNamespaceURI();
                                
                                if (wsdlNamespace != "")
                                {
                                    WSDLDefinition* wsdl = composite->findWSDLDefinition(wsdlNamespace);
                                    if (wsdl == 0)
                                    {
                                        string msg = "WSDL not found for: " + wsdlNamespace;
                                        throwException(SystemConfigurationException, msg.c_str());
                                    }
                                    try
                                    {
                                        wsdlOperation = wsdl->findOperation(wsdlInterface->getName(), op_name.c_str());
                                    }
                                    catch(SystemConfigurationException&)
                                    {   
                                        throw;
                                    }
                                    
                                    if (!wsdlOperation.isDocumentStyle() || !wsdlOperation.isWrappedStyle())
                                    {
                                        throwException(ServiceInvocationException,
                                            "Only wrapped document style WSDL operations are currentlysupported");
                                    }
                                }
                            }
                        }
                        else
                        {
                            Utils::tokeniseString("/", uri, op_name, uriArgs);
                        }
                    
                        // Create a default document literal wrapped WSDL operation
                        if (wsdlNamespace == "")
                        {
                            WSDLMessagePart inPart(op_name, "", "http://tempuri.org");
                            WSDLMessagePart outPart((op_name+"Response"), "", "http://tempuri.org");
                            wsdlNamespace = compositeService->getName();
                            wsdlOperation = WSDLOperation();
                            wsdlOperation.setOperationName(op_name.c_str());
                            wsdlOperation.setSoapAction(wsdlNamespace+ "#" +op_name);
                            wsdlOperation.setEndpoint("");
                            wsdlOperation.setSoapVersion(WSDLOperation::SOAP11);
                            wsdlOperation.setDocumentStyle(true);
                            wsdlOperation.setWrappedStyle(true);
                            wsdlOperation.setInputEncoded(false);
                            wsdlOperation.setOutputEncoded(false);
                            wsdlOperation.setInputMessagePart(op_name, inPart);
                            wsdlOperation.setOutputMessagePart((op_name+"Response"), outPart);
                        }
                        
                        // Create the input DataObject
                        Operation operation(op_name.c_str());
                        
                        // Parse the args part of the URI
                        if (uriArgs != "")
                        {
                            string args = uriArgs;
                            for (; args != ""; )
                            {
                                string param;
                                string next;
                                Utils::tokeniseString("/", args, param, next);
                                if (param != "")
                                {
                                    string* data = new string;
                                    *data = param;
                                    operation.addParameter(data);    
                                }
                                args = next;
                            }
                        }

                        // Parse the query string
                        if (request->args)
                        {
                            string query = request->args;
                            for (; query != ""; )
                            {
                                string param;
                                string next;
                                Utils::tokeniseString("&", query, param, next);
                                if (param != "")
                                {
                                    string n;
                                    string* data = new string;
                                    Utils::tokeniseString("=", param, n, *data);
                                    operation.addParameter(data);    
                                }
                                query = next;
                            }
                        }
                        
                        // Read the POST input
                        ostringstream sinput;
                        char buffer[2049];
                        for ( ; ; )
                        {
                            int size = ap_get_client_block(request, buffer, 2048);
                            if (size > 0)
                            {
                                buffer[size] = '\0';
                                sinput << buffer;
                            }
                            else if (size == 0)
                            {
                                break;
                            }
                            else if (size < 0)
                            {
                                throwException(ServiceInvocationException, "Error reading POST input");
                            }
                        }
                        string input = sinput.str();
                        
                        string contentType = content_type;
                        if (contentType.find("multipart/form-data") == 0)
                        {
                            // This is a multipart POST, extract each part from the
                            // POST body
                            string begin;
                            string boundary;
                            Utils::tokeniseString("boundary=", contentType, begin, boundary);
                            
                            for (;;)
                            {
                                // Read each part
                                string part;
                                string next;
                                Utils::tokeniseString(boundary, input, part, next);
                                input = next;
                                
                                // Skip first and last empty parts
                                if (part.length() == 0 || part == "--")
                                    continue;
                                
                                // Read headers
                                bool xml = false;
                                int empty = -1;
                                for (;;)
                                {
                                    string header;
                                    Utils::tokeniseString("\r\n", part, header, next);
                                    part = next;
                                    if (header == "")
                                    {
                                        // Two empty lines signal the beginning of the content
                                        empty++;
                                        if (empty == 1)
                                            break;
                                    }
                                    else
                                    {
                                        empty = 0;
                                        
                                        // Detect XML content
                                        if (header == "Content-Type: text/xml")
                                            xml = true;
                                    }
                                }

                                // Read the part content
                                if (part.length())
                                {                       
                                    // Strip the trailer         
                                    string value;
                                    Utils::tokeniseString("\r\n--", part, value, next);
                                    
                                    if (xml)
                                    {
                                        // Add an XML parameter to the operation
                                        addPart(xmlHelper, value, operation);
                                    }
                                    else
                                    {
                                        // Add a text parameter to the operation
                                        string* stringData = new string;
                                        *stringData = value;
                                        operation.addParameter(stringData); 
                                    }
                                }

                                // Read till the end of the POST body
                                if (input.length() == 0)
                                    break;
                            }                    
                        }
                        else
                        {
                            // The POST body represents a single part 
                            addPart(xmlHelper, input, operation);
                        }
                        
                        DataObjectPtr inputDataObject = createPayload(dataFactory, operation, wsdlOperation);
                        
                        // Dispatch to the REST proxy
                        DataObjectPtr outputDataObject = proxy->invoke(wsdlOperation, inputDataObject);
                        
                        // Send the response back to the client
                        if (iface!=NULL &&
                            iface->getInterfaceTypeQName() == RESTInterface::typeQName)
                        {
                            // Pure REST, send the location of the created resource

                            if (outputDataObject == NULL)
                            {
                                throwException(ServiceInvocationException, "Null output from REST create operation");
                            }
                            
                            string location = "";
                                                        
                            DataObjectList& l = outputDataObject->getList("return");
                            if (l.size())
                            {
                                location = l.getCString(0);
                            }
                            
                            if (location == "")
                            {
                                loginfo("No resource location, sending HTTP 400 response code");
                                request->status = HTTP_BAD_REQUEST;
                                
                                return OK;
                            }

                            string locuri = request->uri;
                            locuri += '/';
                            locuri += location;
                            
                            const char* loc = ap_construct_url(request->pool, locuri.c_str(), request);
                            loginfo("Sending resource location: %s", loc);
                            apr_table_setn(request->headers_out, "Location", loc);
                            apr_table_setn(request->headers_out, "Content-Location", loc);
                            request->status = HTTP_CREATED;

                            // Send the created resource entity back to the client
                            ap_set_content_type(request, "text/xml");
                            ap_rputs(input.c_str(), request);
                            
                        }
                        else
                        {
                            // Command style, send the response element
    
                            if (outputDataObject == NULL)
                            {
                                loginfo("Sending empty response");
                                //request->status = HTTP_NO_CONTENT;
                            }
                            else
                            {
                                XMLHelperPtr xm = HelperProvider::getXMLHelper(dataFactory);
                                DataObjectList& l = outputDataObject->getList("return");
                                if (l.size() != 0)
                                {
                                    DataObjectPtr resultDataObject = l[0];
                                    XMLDocumentPtr doc = xm->createDocument(
                                        resultDataObject,
                                        resultDataObject->getType().getURI(),
                                        resultDataObject->getType().getName());
                                    char* str = xm->save(doc);
    
                                    loginfo("Sending response: %s", str);
                                    ap_set_content_type(request, "text/xml");
                                    ap_rputs(str, request);
                                }
                                else
                                {
                                    loginfo("Sending empty response");
                                    //request->status = HTTP_NO_CONTENT;
                                }
                            }
                        }
                       
                        return OK;
                    }
                    else if (request->method_number == M_PUT)
                    {
    
                        // Handle an HTTP PUT
                        
                        // Determine the operation to invoke                    
                        WSDLOperation wsdlOperation;
                        string wsdlNamespace = "";
                        string op_name = "update";
                        string uriArgs = uri;
                    
                        // Create a default document literal wrapped WSDL operation
                        WSDLMessagePart inputPart(op_name, "", "http://tempuri.org");
                        WSDLMessagePart outputPart((op_name+"Response"), "", "http://tempuri.org");
                        wsdlNamespace = compositeService->getName();
                        wsdlOperation = WSDLOperation();
                        wsdlOperation.setOperationName(op_name.c_str());
                        wsdlOperation.setSoapAction(wsdlNamespace+ "#" +op_name);
                        wsdlOperation.setEndpoint("");
                        wsdlOperation.setSoapVersion(WSDLOperation::SOAP11);
                        wsdlOperation.setDocumentStyle(true);
                        wsdlOperation.setWrappedStyle(true);
                        wsdlOperation.setInputEncoded(false);
                        wsdlOperation.setOutputEncoded(false);
                        wsdlOperation.setInputMessagePart(op_name, inputPart);
                        wsdlOperation.setOutputMessagePart((op_name+"Response"), outputPart);
                        
                        // Create the input DataObject
                        Operation operation(op_name.c_str());
                        
                        // Parse the args part of the URI
                        if (uriArgs != "")
                        {
                            string args = uriArgs;
                            for (; args != ""; )
                            {
                                string param;
                                string next;
                                Utils::tokeniseString("/", args, param, next);
                                if (param != "")
                                {
                                    string* data = new string;
                                    *data = param;
                                    operation.addParameter(data);    
                                }
                                args = next;
                            }
                        }
                        
                        // Parse the query string
                        if (request->args)
                        {
                            string query = request->args;
                            for (; query != ""; )
                            {
                                string param;
                                string next;
                                Utils::tokeniseString("&", query, param, next);
                                if (param != "")
                                {
                                    string n;
                                    string* data = new string;
                                    Utils::tokeniseString("=", param, n, *data);
                                    operation.addParameter(data);    
                                }
                                query = next;
                            }
                        }
                        
                        // Read the PUT input
                        ostringstream sinput;
                        char buffer[2049];
                        for ( ; ; )
                        {
                            int size = ap_get_client_block(request, buffer, 2048);
                            if (size > 0)
                            {
                                buffer[size] = '\0';
                                sinput << buffer;
                            }
                            else if (size == 0)
                            {
                                break;
                            }
                            else if (size < 0)
                            {
                                throwException(ServiceInvocationException, "Error reading PUT input");
                            }
                        }
                        string input = sinput.str();
                        addPart(xmlHelper, input, operation);
                        
                        DataObjectPtr inputDataObject = createPayload(dataFactory, operation, wsdlOperation);
                        
                        // Dispatch to the REST proxy
                        DataObjectPtr outputDataObject = proxy->invoke(wsdlOperation, inputDataObject);

                        // Empty response                        
                        //request->status = HTTP_NO_CONTENT;
                        return OK;
                    }
                    else if (request->method_number == M_DELETE)
                    {
                        
                        // Determine the operation to invoke                    
                        WSDLOperation wsdlOperation;
                        string wsdlNamespace = "";
                        string op_name = "delete";
                        string uriArgs = uri;
                    
                        // Create a default document literal wrapped WSDL operation
                        WSDLMessagePart inPart(op_name, "", "http://tempuri.org");
                        WSDLMessagePart outPart((op_name+"Response"), "", "http://tempuri.org");
                        wsdlNamespace = compositeService->getName();
                        wsdlOperation = WSDLOperation();
                        wsdlOperation.setOperationName(op_name.c_str());
                        wsdlOperation.setSoapAction(wsdlNamespace+ "#" +op_name);
                        wsdlOperation.setEndpoint("");
                        wsdlOperation.setSoapVersion(WSDLOperation::SOAP11);
                        wsdlOperation.setDocumentStyle(true);
                        wsdlOperation.setWrappedStyle(true);
                        wsdlOperation.setInputEncoded(false);
                        wsdlOperation.setOutputEncoded(false);
                        wsdlOperation.setInputMessagePart(op_name, inPart);
                        wsdlOperation.setOutputMessagePart((op_name+"Response"), outPart);
                        
                        // Create the input DataObject
                        Operation operation(op_name.c_str());
                        
                        // Parse the args part of the URI
                        if (uriArgs != "")
                        {
                            string args = uriArgs;
                            for (; args != ""; )
                            {
                                string param;
                                string next;
                                Utils::tokeniseString("/", args, param, next);
                                if (param != "")
                                {
                                    string* data = new string;
                                    *data = param;
                                    operation.addParameter(data);    
                                }
                                args = next;
                            }
                        }
                        
                        // Parse the query string
                        if (request->args)
                        {
                            string query = request->args;
                            for (; query != ""; )
                            {
                                string param;
                                string next;
                                Utils::tokeniseString("&", query, param, next);
                                if (param != "")
                                {
                                    string n;
                                    string* data = new string;
                                    Utils::tokeniseString("=", param, n, *data);
                                    operation.addParameter(data);    
                                }
                                query = next;
                            }
                        }
                        
                        DataObjectPtr inputDataObject = createPayload(dataFactory, operation, wsdlOperation);
                        
                        // Dispatch to the REST proxy
                        DataObjectPtr outputDataObject = proxy->invoke(wsdlOperation, inputDataObject);

                        // Empty response
                        //request->status = HTTP_NO_CONTENT;                        
                        return OK;
                    }
                    else
                    {
                        if (request->method)
                        {
                            logerror("Unsupported HTTP method: %s", request->method);
                        }
                        else
                        {
                            logerror("Unsupported HTTP method: %d", request->method_number);
                        }
                        return HTTP_NOT_IMPLEMENTED;
                    }
                }
                catch(TuscanyRuntimeException& ex)
                {
                    ostringstream msg;
                    msg << ex;
                    logerror("Failed to process REST request: %s", msg.str().c_str());
                    return HTTP_INTERNAL_SERVER_ERROR;
                }        
            }
            
            DataObjectPtr createPayload(DataFactoryPtr dataFactory, Operation& operation, const WSDLOperation& wsdlOperation)
            {   
                logentry();
            
                DataObjectPtr inputDataObject;
                string inputTypeUri;
                string inputTypeName;
                try
                {
                    // Since its Document wrapped, there will only be one part
                    std::list<std::string> partList = wsdlOperation.getInputMessagePartNames();
                    const WSDLMessagePart &inputMessage =
                      wsdlOperation.getInputMessagePart(partList.front());
                    inputTypeName = inputMessage.getPartType();
                    inputTypeUri = inputMessage.getPartUri();
                    
                    // Create the input wrapper
                    const Type& rootType = dataFactory->getType(inputTypeUri.c_str(), "RootType");
                    const Property& prop = rootType.getProperty(inputTypeName.c_str());
                    const Type& inputType = prop.getType();
                    inputDataObject = dataFactory->create(inputType);
                }
                catch (SDORuntimeException&)
                {
                    try
                    {
                        // Create the input wrapper
                        const Type& inputType =
                          dataFactory->getType(inputTypeUri.c_str(), inputTypeName.c_str());
                        inputDataObject = dataFactory->create(inputType);
                    }
                    catch (SDORuntimeException&)
                    {
                        
                        // The input wrapper type is not known, create an open DataObject 
                        inputDataObject = dataFactory->create("http://tempuri.org", "Wrapper");
                    }
                }
                        
                // Go through data object to set the input parameters
                PropertyList pl = inputDataObject->getType().getProperties();
            
                if(pl.size() == 0)
                {
                    if(inputDataObject->getType().isOpenType() && inputDataObject->getType().isDataObjectType())
                    {
                        /*
                         * This code deals with sending xsd:any elements
                         */
                        for (int i=0; i<operation.getNParms(); i++)
                        {
                            ostringstream pname;
                            pname << "param" << (i+1);
                            DataObjectList& l = inputDataObject->getList(pname.str());
                            
                            const Operation::Parameter& parm = operation.getParameter(i);
                            switch(parm.getType())
                            {
                            case Operation::STRING: 
                                {
                                    l.append((*(string*)parm.getValue()).c_str());
                                    break;
                                }
                            case Operation::DATAOBJECT: 
                                {
                                    l.append(*(DataObjectPtr*)parm.getValue());
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
                else {
                    
                    // Each parameter in the operation should be a property on the request dataobject
                    for (unsigned int i=0; i<operation.getNParms(); i++)
                    {
                        const Operation::Parameter& parm = operation.getParameter(i);
                        switch(parm.getType())
                        {
                        case Operation::STRING: 
                            {
                                inputDataObject->setCString(i, (*(string*)parm.getValue()).c_str());
                                break;
                            }
                        case Operation::DATAOBJECT: 
                            {
                                inputDataObject->setDataObject(i, *(DataObjectPtr*)parm.getValue());
                                break;
                            }
                        default:
                            {
                                break;
                            }
                        }
                    }
                }
                
                return inputDataObject;
             }

             void addPart(XMLHelperPtr xmlHelper, string& payload, Operation& operation)
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
                DataObjectPtr inputWrapperDataObject = NULL;
                XMLDocumentPtr theXMLDocument = xmlHelper->load(body.c_str(), NULL);
                if (theXMLDocument != 0)
                {
                    inputWrapperDataObject = theXMLDocument->getRootDataObject();
                }
                if(!inputWrapperDataObject)
                {
                    ostringstream msg;
                    msg << "Could not convert received document to SDO: " << body;                     
                    throwException(ServiceDataException, msg.str().c_str());
                }                    

                // Get the body part
                DataObjectPtr inputDataObject = NULL; 
                PropertyList bpl = inputWrapperDataObject->getInstanceProperties();
                if (bpl.size()!=0)
                {
                    if (bpl[0].isMany())
                    {
                        DataObjectList& parts = inputWrapperDataObject->getList((unsigned int)0);
                        inputDataObject = parts[0];
                    }
                    else
                    {
                        inputDataObject = inputWrapperDataObject->getDataObject(bpl[0]);
                    }
                }
                if (inputDataObject == NULL)
                {
                    ostringstream msg;
                    msg << "Could not convert received document to SDO: " << body;                     
                    throwException(ServiceDataException, msg.str().c_str());
                }
                
                DataObjectPtr* dataObjectData = new DataObjectPtr;
                *dataObjectData = inputDataObject;
                (*dataObjectData)->detach();
                operation.addParameter(dataObjectData);
             }

            const char *rest_set_home(cmd_parms *cmd, void *dummy, 
                                    const char *arg)
            {
                rest_server_config_rec_t *conf = (rest_server_config_rec_t*)ap_get_module_config(
                                    cmd->server->module_config, &sca_rest_module);
                conf->home = apr_pstrdup(cmd->pool, arg);
                return NULL;
            }
            
            const char *rest_set_path(cmd_parms *cmd, void *c, 
                                    const char *arg)
            {
                rest_dir_config_rec_t *conf = (rest_dir_config_rec_t*)c;
                conf->path = apr_pstrdup(cmd->pool, arg);
                return NULL;
            }
            
            const char *rest_set_root(cmd_parms *cmd, void *c, 
                                    const char *arg)
            {
                rest_dir_config_rec_t *conf = (rest_dir_config_rec_t*)c;
                conf->root = apr_pstrdup(cmd->pool, arg);
                return NULL;
            }
            
            const char *rest_set_base_uri(cmd_parms *cmd, void *c, 
                                    const char *arg)
            {
                rest_dir_config_rec_t *conf = (rest_dir_config_rec_t*)c;
                conf->base_uri = apr_pstrdup(cmd->pool, arg);
                return NULL;
            }
            
            const char *rest_set_component(cmd_parms *cmd, void *c, 
                                    const char *arg)
            {
                rest_dir_config_rec_t *conf = (rest_dir_config_rec_t*)c;
                conf->component = apr_pstrdup(cmd->pool, arg);
                return NULL;
            }
            
            const command_rec rest_module_cmds[] = 
            {
                AP_INIT_TAKE1("TuscanyHome", (const char*(*)())tuscany::sca::rest::rest_set_home, NULL, RSRC_CONF,
                              "Tuscany home directory"),
                AP_INIT_TAKE1("TuscanyPath", (const char*(*)())tuscany::sca::rest::rest_set_path, NULL, ACCESS_CONF,
                              "Tuscany SCA composite search path"),
                AP_INIT_TAKE1("TuscanyRoot", (const char*(*)())tuscany::sca::rest::rest_set_root, NULL, ACCESS_CONF,
                              "Tuscany root SCA configuration path"),
                AP_INIT_TAKE1("TuscanyBaseURI", (const char*(*)())tuscany::sca::rest::rest_set_base_uri, NULL, ACCESS_CONF,
                              "Tuscany SCA system base URI"),
                AP_INIT_TAKE1("TuscanyComponent", (const char*(*)())tuscany::sca::rest::rest_set_component, NULL, ACCESS_CONF,
                              "SCA component name"),
                {NULL}
            };
            
            int rest_init(apr_pool_t *p, apr_pool_t *plog, apr_pool_t *ptemp,
                                   server_rec *s)
            {
                return OK;
            }
            
            void rest_child_init(apr_pool_t* p, server_rec* svr_rec)
            {
                rest_server_config_rec_t *conf = (rest_server_config_rec_t*)ap_get_module_config(
                                    svr_rec->module_config, &sca_rest_module); 
            
                if(false)
                {
                    fprintf(stderr, "[Tuscany] Due to one or more errors mod_rest loading"
                                    " failed. Causing apache2 to stop loading\n");
                    exit(APEXIT_CHILDFATAL);
                }
            }
            
            void register_hooks(apr_pool_t *p)
            {
                ap_hook_handler(rest_handler, NULL, NULL, APR_HOOK_MIDDLE);
                ap_hook_post_config(rest_init, NULL, NULL, APR_HOOK_MIDDLE);
                ap_hook_child_init(rest_child_init, NULL, NULL, APR_HOOK_MIDDLE);
            }
            
            void *rest_create_dir_config(apr_pool_t *p, char *dirspec)
            {
                rest_dir_config_rec_t* conf = (rest_dir_config_rec_t* )apr_palloc(p, sizeof(*conf));
                conf->path = "";
                conf->root = "";
                conf->base_uri = "";
                conf->component = "";
                return conf;
            }
            
            void* rest_create_server_config(apr_pool_t *p, server_rec *s)
            {
                rest_server_config_rec_t* conf = (rest_server_config_rec_t* )apr_palloc(p, sizeof(*conf));
                conf->home = "";
                return conf;
            }
            
        } // End namespace rest        
    } // End namespace sca
} // End namespace tuscany

extern "C"
{

    module AP_MODULE_DECLARE_DATA sca_rest_module =
    {
        STANDARD20_MODULE_STUFF,
        tuscany::sca::rest::rest_create_dir_config,         /* dir config */
        NULL,                       /* dir merger --- default is to override */
        tuscany::sca::rest::rest_create_server_config,        /* server config */
        NULL,                       /* merge server config */
        tuscany::sca::rest::rest_module_cmds,         /* command table */
        tuscany::sca::rest::register_hooks              /* register_hooks */
    };
    
}
