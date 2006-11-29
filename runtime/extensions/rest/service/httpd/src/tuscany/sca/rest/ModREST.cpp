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

#include "apr_strings.h"
#include "apr_fnmatch.h"
#include "apr_strings.h"
#include "apr_lib.h"

#define APR_WANT_STRFUNC
#include "apr_want.h"

#include "ap_config.h"
#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_request.h"
#include "http_protocol.h"
#include "http_log.h"
#include "http_main.h"
#include "util_script.h"

#include "mod_core.h"

#include "tuscany/sca/util/Exceptions.h"
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
#include "tuscany/sca/model/WSDLInterface.h" 
#include "tuscany/sca/model/Interface.h" 
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/util/Utils.h"

#include "commonj/sdo/SDO.h"

#include <sstream>

using namespace commonj::sdo;

using namespace tuscany::sca;
using namespace tuscany::sca::rest;
using namespace tuscany::sca::model;


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
                char * component;
            } rest_dir_config_rec_t;
            
            CompositeService* initializeSCARuntime(
                const char* home, const char* root, const char* path, const char *component, const char* service);
            
            DataObjectPtr createPayload(DataFactoryPtr dataFactory,
                Operation& operation, const WSDLOperation& wsdlOperation);

            int logHeaders(void* request, const char* key, const char* value);

            /**
             * Initialize the SCA runtime
             */
            CompositeService* initializeSCARuntime(const char* home, const char* root, const char* path, const char *component, const char* service)
            {
                logentry();
                loginfo("Root: %s, path: %s, component: %s, service: %s", root, path, component, service);
                
                try
                {
                    bool restart = false;
                    bool resolve = false;
            
                    string systemRoot = SCARuntime::getSystemRoot();
                    if (systemRoot != root)
                    {
                        systemRoot = root;
                        restart = true;
                        resolve = true;
                    }
            
                    string systemPath = SCARuntime::getSystemPath();
                    if (systemPath != path)
                    {
                        systemPath = path;
                        restart = true;
                        resolve = true;
                    }
            
                    string componentName = SCARuntime::getDefaultComponentName();
                    if (componentName != component)
                    {
                        componentName = component;
                        resolve = true;
                    }
                    
                    string serviceName = service;
            
                    SCARuntime* scaRuntime;
                    if (restart)
                    {
                        loginfo("Starting SCA runtime");
                        SCARuntime::releaseInstance();
                        SCARuntime::setSystemRoot(systemRoot);
                        SCARuntime::setSystemPath(systemPath);
                        SCARuntime::setDefaultComponentName(componentName);
                        scaRuntime = SCARuntime::getInstance();
                    }
                    else if (resolve)
                    {
                        loginfo("Starting SCA runtime");
                        SCARuntime::releaseInstance();
                        SCARuntime::setSystemRoot(systemRoot);
                        SCARuntime::setSystemPath(systemPath);
                        SCARuntime::setDefaultComponentName(componentName);
                        scaRuntime = SCARuntime::getInstance();
                    }
                    else
                    {
                        scaRuntime = SCARuntime::getInstance();
                    }
            
                    loginfo("Resolving composite: %s, service: %s", componentName.c_str(), serviceName.c_str());
                    Component* compositeComponent = scaRuntime->getDefaultComponent();
                    if (compositeComponent == NULL)
                    {
                        string msg = "Component not found " + componentName;
                        throwException(SystemConfigurationException, msg.c_str());
                    }
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
                    
                    int content_length = request->remaining;
                    if (request->read_chunked == true && content_length == 0)
                    {
                        content_length = -1;
                        request->chunked = true;
                    }
                
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
                    
                    const char* protocol = request->protocol;
                    if (protocol)
                    {
                        loginfo("Protocol: %s", protocol);
                        if (printRequest)
                        {
                            ap_rprintf(request, "<p>Protocol: %s", protocol);
                        }
                    }
                    
                    const char* method = request->method;
                    if (method)
                    {
                        loginfo("HTTP method: %s", method);
                        if (printRequest)
                        {
                            ap_rprintf(request, "<p>HTTP method: %s", method);
                        }
                    }
                
                    const int method_number = request->method_number;
                    loginfo("HTTP method number: %d", method_number);
                    if (printRequest)
                    {
                        ap_rprintf(request, "<p>HTTP method number: %d", method_number);
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
                
                    const char* content_encoding = request->content_encoding;
                    if (content_encoding)
                    {
                        loginfo("Content encoding: %s", content_encoding);
                        if (printRequest)
                        {
                            ap_rprintf(request, "<p>Content encoding: %s", content_encoding);
                        }
                    }
                
                    if (printRequest)
                    {
                        ap_rputs("<p>", request);
                    }
                    apr_table_do(logHeaders, request, request->headers_in, NULL);
                    
                    const char* uri = request->uri;
                    if (uri)
                    {
                        loginfo("URI: %s", uri);
                        if (printRequest)
                        {
                            ap_rprintf(request, "<p>URI: %s", uri);
                        }
                    }
                    
                    const char* path_info = request->path_info;
                    if (path_info)
                    {
                        loginfo("Path info: %s", path_info);
                        if (printRequest)
                        {
                            ap_rprintf(request, "<p>Path info: %s", path_info);
                        }
                    }
                    
                    const char* queryArgs = request->args;
                    if (queryArgs)
                    {
                        loginfo("Args: %s", queryArgs);
                        if (printRequest)
                        {
                            ap_rprintf(request, "<p>Args: %s", queryArgs);
                        }
                    }
                    
                    if (printRequest)
                    {
                        ap_rputs("</body></html>", request);
                    }
                    
                    if (method_number == M_GET)
                    {
                        // Handle an HTTP GET
                        
                        // Extract the operation name, service and component names from
                        // the HTTP URI path
                        string path;
                        if (strlen(path_info) != 0 && *path_info == '/')
                        {
                            path = path_info + 1;
                        }
                        else
                        {
                            path = path_info;
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
                                    server_conf->home, dir_conf->root, dir_conf->path, component.c_str(), service.c_str());
                                    
                        if(!compositeService)
                        {
                            throwException(SystemConfigurationException,
                                "Failed to initialize SCA runtime, could not initialize CompositeService");
                        }
            
                        DataFactoryPtr dataFactory = compositeService->getComposite()->getDataFactory();
                        if (dataFactory == 0)
                        {
                            throwException(SystemConfigurationException,
                                "Failed to initialize SCA runtime, could not get DataFactory");
                        }
            
                        // Get the REST binding
                        Composite* composite = compositeService->getComposite();
                        Reference* reference = compositeService->getReference();
                        RESTReferenceBinding* binding = (RESTReferenceBinding*)reference->getBinding();
                        Interface* iface = reference->getType()->getInterface();
    
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
                            wsdlNamespace = compositeService->getName();
                            wsdlOperation = WSDLOperation();
                            wsdlOperation.setOperationName(op_name.c_str());
                            wsdlOperation.setSoapAction(wsdlNamespace+ "#" +op_name);
                            wsdlOperation.setEndpoint("");
                            wsdlOperation.setSoapVersion(WSDLOperation::SOAP11);
                            wsdlOperation.setDocumentStyle(true);
                            wsdlOperation.setWrappedStyle(true);
                            wsdlOperation.setEncoded(false);
                            wsdlOperation.setInputType(string("http://tempuri.org") + "#" + op_name);
                            wsdlOperation.setOutputType(string("http://tempuri.org") + "#" + op_name + "Response");
                        }
                        
                        //  Get the REST proxy
                        RESTServiceProxy* proxy = (RESTServiceProxy*)binding->getServiceProxy();
            
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
                        if (queryArgs)
                        {
                            string query = queryArgs;
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
                        
                        // Send the response back to the client
                        if (outputDataObject == NULL)
                        {
                            loginfo("Sending empty response");

                            // Set the content type
                            ap_set_content_type(request, "text/xml");
                        
                            return OK;
                        }
                        
                        // Send the output DataObject
                        char *str;
                        if (iface!=NULL &&
                            iface->getInterfaceTypeQName() == RESTInterface::typeQName)
                        {
                            // Pure REST, send the response document
                            XMLHelperPtr xm = HelperProvider::getXMLHelper(dataFactory);
                            DataObjectList& l = outputDataObject->getList("return");
                            DataObjectPtr resourceDataObject = l[0];
                            XMLDocumentPtr doc = xm->createDocument(
                                resourceDataObject,
                                resourceDataObject->getType().getURI(),
                                resourceDataObject->getType().getName());
                           doc->setXMLDeclaration(false);
                           str = xm->save(doc);
                        }
                        else
                        {
                            // Command style, send the response wrapper element
                            XMLHelperPtr xm = HelperProvider::getXMLHelper(dataFactory);
                            XMLDocumentPtr doc = xm->createDocument(
                                outputDataObject,
                                wsdlOperation.getOutputTypeUri().c_str(), 
                                wsdlOperation.getOutputTypeName().c_str());
                           doc->setXMLDeclaration(false);
                           str = xm->save(doc);
                        }
                       
                        // Set the content type
                        ap_set_content_type(request, "text/xml");
                    
                        loginfo("Sending response: %s", str);
                       ap_rputs(str, request);
                       
                       return OK;
                    }
                    else if (method_number == M_POST)
                    {
                    }
                    else if (method_number == M_PUT)
                    {
                    }
                    else if (method_number == M_DELETE)
                    {
                    }
                    else
                    {
                        if (method)
                        {
                            ostringstream msg;
                            msg << "Unsupported HTTP method: %s" << method;
                            throwException(ServiceInvocationException, msg.str().c_str());
                        }
                        else
                        {
                            throwException(ServiceInvocationException, "Unsupported HTTP method");
                        }
                    }
                    
                    return OK;
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
                try
                {
                    
                    // Create the input wrapper
                    const Type& rootType = dataFactory->getType(wsdlOperation.getInputTypeUri().c_str(), "RootType");
                    const Property& prop = rootType.getProperty(wsdlOperation.getInputTypeName().c_str());
                    const Type& inputType = prop.getType();
                    inputDataObject = dataFactory->create(inputType);
                }
                catch (SDORuntimeException&)
                {
                    try
                    {
                        // Create the input wrapper
                        const Type& inputType = dataFactory->getType(wsdlOperation.getInputTypeUri().c_str(), 
                            wsdlOperation.getInputTypeName().c_str());
                        inputDataObject = dataFactory->create(inputType);
                    }
                    catch (SDORuntimeException&)
                    {
                        
                        // The input wrapper type is not known, create an open DataObject 
                        inputDataObject = dataFactory->create(Type::SDOTypeNamespaceURI, "OpenDataObject");
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
                            l.append((*(string*)parm.getValue()).c_str());
                        }
                    }
                }
                else {
                    
                    // Each parameter in the operation should be a property on the request dataobject
                    for (int i=0; i<operation.getNParms(); i++)
                    {
                        const Operation::Parameter& parm = operation.getParameter(i);
                        inputDataObject->setCString(i, (*(string*)parm.getValue()).c_str());
                    }
                }
                
                return inputDataObject;
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
            
            const char *rest_set_component(cmd_parms *cmd, void *c, 
                                    const char *arg)
            {
                rest_dir_config_rec_t *conf = (rest_dir_config_rec_t*)c;
                conf->component = apr_pstrdup(cmd->pool, arg);
                return NULL;
            }
            
            const command_rec rest_module_cmds[] = 
            {
                AP_INIT_TAKE1("TuscanyHome", (const char*(*)())rest_set_home, NULL, RSRC_CONF,
                              "Tuscany home directory"),
                AP_INIT_TAKE1("TuscanyPath", (const char*(*)())rest_set_path, NULL, ACCESS_CONF,
                              "Tuscany SCA composite search path"),
                AP_INIT_TAKE1("TuscanyRoot", (const char*(*)())rest_set_root, NULL, ACCESS_CONF,
                              "Tuscany root SCA configuration path"),
                AP_INIT_TAKE1("TuscanyComponent", (const char*(*)())rest_set_component, NULL, ACCESS_CONF,
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
        rest_create_dir_config,         /* dir config */
        NULL,                       /* dir merger --- default is to override */
        rest_create_server_config,        /* server config */
        NULL,                       /* merge server config */
        rest_module_cmds,         /* command table */
        register_hooks              /* register_hooks */
    };
    
}
