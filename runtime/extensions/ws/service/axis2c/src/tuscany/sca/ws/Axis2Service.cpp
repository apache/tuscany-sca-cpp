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
#pragma warning(disable: 4091)
#endif

#include <axis2_svc_skeleton.h>
#include <axis2_array_list.h>
#include <axis2_log_default.h>
#include <axis2_error_default.h>
#include <axiom.h>
#include <stdio.h>

#include <sdo_axiom.h>
using commonj::sdo::DataObjectPtr;
using commonj::sdo::DataFactoryPtr;
using commonj::sdo::DataObjectList;
using commonj::sdo::PropertyList;
using commonj::sdo_axiom::AxiomHelper;

#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/util/Logging.h"
#include "WSServiceProxy.h"
#include "model/WSReferenceBinding.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/CompositeService.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/model/WSDLDefinition.h"
#include "tuscany/sca/model/WSDLOperation.h"
#include "tuscany/sca/model/WSDLInterface.h" 
#include "tuscany/sca/model/Interface.h" 
#include "tuscany/sca/core/TuscanyRuntime.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/util/Utils.h"
#include "Axis2Utils.h"

using namespace tuscany::sca;
using namespace tuscany::sca::ws;
using namespace tuscany::sca::model;

int AXIS2_CALL
Axis2Service_free(axis2_svc_skeleton_t *svc_skeleton,
          const axis2_env_t *env);

/*
 * This method invokes the right service method 
 */
axiom_node_t* AXIS2_CALL 
Axis2Service_invoke(axis2_svc_skeleton_t *svc_skeleton,
            const axis2_env_t *env,
            axiom_node_t *node,
            axis2_msg_ctx_t *msg_ctx);

int AXIS2_CALL Axis2Service_init(axis2_svc_skeleton_t *svc_skeleton,
                        const axis2_env_t *env);

axis2_svc_skeleton_t *
axis2_Axis2Service_create(axis2_env_t *env)
{
    axis2_svc_skeleton_t *svc_skeleton = NULL;
    svc_skeleton = (axis2_svc_skeleton_t *) AXIS2_MALLOC((env)->allocator, 
        sizeof(axis2_svc_skeleton_t));

    
    svc_skeleton->ops = (axis2_svc_skeleton_ops_t *) AXIS2_MALLOC(
        (env)->allocator, sizeof(axis2_svc_skeleton_ops_t));

    svc_skeleton->func_array = NULL;

    svc_skeleton->ops->free = Axis2Service_free;
    svc_skeleton->ops->init = Axis2Service_init;
    svc_skeleton->ops->invoke = Axis2Service_invoke;
    /*svc_skeleton->ops->on_fault = Axis2Service_on_fault;*/

    return svc_skeleton;
}

int AXIS2_CALL
Axis2Service_init(axis2_svc_skeleton_t *svc_skeleton,
                        const axis2_env_t *env)
{
    // This method never seems to be called - an old Axis2C artifact?

    svc_skeleton->func_array = axis2_array_list_create(env, 0);
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
Axis2Service_free(axis2_svc_skeleton_t *svc_skeleton,
            const axis2_env_t *env)
{
    /*if(svc_skeleton->func_array)
    {
        AXIS2_ARRAY_LIST_FREE(svc_skeleton->func_array, env);
        svc_skeleton->func_array = NULL;
    }*/

    if(svc_skeleton->ops)
    {
        AXIS2_FREE((env)->allocator, svc_skeleton->ops);
        svc_skeleton->ops = NULL;
    }
    
    if(svc_skeleton)
    {
        AXIS2_FREE((env)->allocator, svc_skeleton);
        svc_skeleton = NULL;
    }
    return AXIS2_SUCCESS; 
}

/**
 * Initialize the Tuscany runtime
 */
 
static string systemRoot = "";
static string componentName = "";
static string serviceName = "";
static TuscanyRuntime* tuscanyRuntime = NULL;
static CompositeService* compositeService = NULL;

void initTuscanyRuntime(const axis2_env_t *env, const char* root, const char *component, const char* service)
{
    logentry();
    loginfo("Root: %s, component: %s, service: %s", root, component, service);
    
    try
    {

        bool restart = false;
        bool resolve = false;

        if (systemRoot != root)
        {
            systemRoot = root;
            restart = true;
            resolve = true;
        }

        if (componentName != component)
        {
            componentName = component;
            resolve = true;
        }
        
        if (serviceName != service)
        {
            serviceName = service;
            resolve = true;
        }
        
        if (tuscanyRuntime == NULL)
        {
            loginfo("Creating new Tuscany runtime");
            tuscanyRuntime = new TuscanyRuntime(componentName, systemRoot);
            tuscanyRuntime->start();
        }
        else if (restart)
        {
            loginfo("Restarting Tuscany runtime");
            tuscanyRuntime->stop();
            tuscanyRuntime->setDefaultComponentName(componentName);
            tuscanyRuntime->setSystemRoot(systemRoot);
            tuscanyRuntime->start();
        }
        else if (resolve)
        {
            loginfo("Refreshing Tuscany runtime");
            tuscanyRuntime->stop();
            tuscanyRuntime->setDefaultComponentName(componentName);
            tuscanyRuntime->setSystemRoot(systemRoot);
            tuscanyRuntime->start();
        }

        if (compositeService == NULL)
        {
            loginfo("Resolving composite: %s, service: %s", componentName.c_str(), serviceName.c_str());
            Component* component = SCARuntime::getInstance()->getDefaultComponent();
            if (component == NULL)
            {
                string msg = "Component not found " + componentName;
                throwException(SystemConfigurationException, msg.c_str());
            }
            Composite* composite = (Composite*)component->getType();
            compositeService = (CompositeService*)composite->findComponent(serviceName);
            if (compositeService == NULL)
            {
                string msg = "Composite service not found " + serviceName;
                throwException(SystemConfigurationException, msg.c_str());
            }
        }
        else
        {
            if (resolve)
            {
                loginfo("Switching to composite: %s, service: %s", componentName.c_str(), serviceName.c_str());
                Component* component = SCARuntime::getInstance()->getDefaultComponent();
                if (component == NULL)
                {
                    string msg = "Component not found " + componentName;
                    throwException(SystemConfigurationException, msg.c_str());
                }
                Composite* composite = (Composite*)component->getType();
                compositeService = (CompositeService*)composite->findComponent(serviceName);
                if (compositeService == NULL)
                {
                    string msg = "Composite service not found " + serviceName;
                    throwException(SystemConfigurationException, msg.c_str());
                }
            }
            else
            {
                loginfo("Reusing composite: %s, service: %s", componentName.c_str(), serviceName.c_str());
            }
        }
    }
    catch(TuscanyRuntimeException &ex)
    {
        logerror("Failed to initialize Tuscany runtime: %s", (const char*)ex);
        throw;
    }  
}


/*
 * This method invokes the target service method 
 */
axiom_node_t* AXIS2_CALL
Axis2Service_invoke(axis2_svc_skeleton_t *svc_skeleton,
            const axis2_env_t *env,
            axiom_node_t *node,
            axis2_msg_ctx_t *msg_ctx)
{
    logentry();

    try
    {    
        if (node)
        {
            if (AXIOM_NODE_GET_NODE_TYPE(node, env) == AXIOM_ELEMENT)
            {
                axiom_element_t *element = NULL;
                element = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(node, env);
                if (element)
                {
                    string op_name = "";
                    
                    axis2_bool_t rest = AXIS2_MSG_CTX_GET_DOING_REST(msg_ctx, env);
                    if (rest)
                    {
                        axis2_endpoint_ref_t *endpoint_ref = AXIS2_MSG_CTX_GET_FROM(msg_ctx, env);
                        if (endpoint_ref)
                        {
                            const axis2_char_t *addr = AXIS2_ENDPOINT_REF_GET_ADDRESS(endpoint_ref, env);
                            if (addr)
                            {
                                // REST request, the op name is the last segment of the path
                                string raddress = addr;
                                string path;
                                string query;
                                Utils::tokeniseString("?", raddress, path, query);
                                string uri;
                                Utils::rTokeniseString("/", path, uri, op_name);
                            }
                        }
                    }
                    else
                    {
                        // SOAP request                
                        // Get the operation name from the root element name, this is correct for DocLit Wrapped style
                        op_name = AXIOM_ELEMENT_GET_LOCALNAME(element, env);
                    }
                    
                    if (op_name != "")
    				{
                        // Get the Tuscany system root and composite service name from the Axis2
                        // service parameters 
                        char* rootParam = Axis2Utils::getAxisServiceParameterValue(env, msg_ctx, "TuscanySystemRoot");
                        if (rootParam == NULL)
                            rootParam = "";
                        
                        char* serviceParam = Axis2Utils::getAxisServiceParameterValue(env, msg_ctx, "TuscanyService");
                        if (serviceParam != NULL)
                        {
                            loginfo("System root: %s, service name: %s, operation name: %s", rootParam, serviceParam, op_name.c_str());
        
                            // Service is of the form "component name"/"composite service name"
                            string component, service;
                            Utils::rTokeniseString("/", serviceParam, component, service);
                    
                            initTuscanyRuntime(env, rootParam, component.c_str(), service.c_str());
                        }
                        else {
                            
                            // Use the default system root and component, the service is
                            // derived from the target address
                            axis2_endpoint_ref_t *endpoint_ref = NULL;
                            endpoint_ref = AXIS2_MSG_CTX_GET_FROM(msg_ctx, env);
                            string address = AXIS2_ENDPOINT_REF_GET_ADDRESS(endpoint_ref, env);
                            
                            axis2_bool_t isrest = AXIS2_MSG_CTX_GET_DOING_REST(msg_ctx, env);
                            string path;
                            if (isrest)
                            {
                                string op;
                                Utils::rTokeniseString("/", address, path, op);
                            }
                            else
                            {
                                path = address;
                            }
    
                            string path2;                        
                            string service;
                            Utils::rTokeniseString("/", path, path2, service);
                            
                            string path3;
                            string component;
                            Utils::rTokeniseString("/", path2, path3, component);
                            if (component == "services")
                            {
                                component = "";
                            }
                            
                            loginfo("System root: %s, component name: %s, service name: %s, operation name: %s",
                                rootParam, component.c_str(), service.c_str(), op_name.c_str());
                            
                            initTuscanyRuntime(env, rootParam, component.c_str(), service.c_str());
                        }
    
                        if(!compositeService)
                        {
                   		    logerror("Failed to initialize Tuscany runtime, could not initialize CompositeService");
                            return 0;
                        }
    
                        DataFactoryPtr dataFactory = compositeService->getComposite()->getDataFactory();
                        if (dataFactory == 0)
                        {
                            logerror("Failed to initialize Tuscany runtime, could not get DataFactory");
                            return 0;
                        }
    
                        // Get the WS binding and the WSDL operation
                        Composite* composite = compositeService->getComposite();
                        Reference* reference = compositeService->getReference();
                        WSReferenceBinding* binding = (WSReferenceBinding*)reference->getBinding();
                        WSDLOperation wsdlOperation;
                         
                        string wsdlNamespace = binding->getWSDLNamespaceURL();
                        if (wsdlNamespace != "")
                        {
                            
                            // Lookup the WSDL model from the composite, keyed on the namespace 
                            // (the wsdl will have been loaded at startup)
                            WSDLDefinition* wsdlDefinition = composite->findWSDLDefinition(wsdlNamespace);
                            if (wsdlDefinition == 0)
                            {
                                string msg = "WSDL not found for: " + wsdlNamespace;
                                throwException(SystemConfigurationException, msg.c_str());
                            }
                            
                            // Find the target operation in the wsdl port type.
                            try {
                                wsdlOperation =  wsdlDefinition->findOperation(
                                    binding->getServiceName(),
                                    binding->getEndpointName(),
                                    op_name.c_str());
                            }
                            catch(SystemConfigurationException& ex)
                            {   
                                throw;
                            }
                            
                        }
                        else
                        {
                            Interface* iface = reference->getType()->getInterface();
                            if (iface != NULL &&
                                iface->getInterfaceTypeQName() == WSDLInterface::typeQName)
                            {
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
                                    catch(SystemConfigurationException& ex)
                                    {   
                                        throw;
                                    }
                                }
                            }
                        }
                        
                        if (wsdlNamespace == "")
                        {
                            // Create a default document literal wrapped WSDL operation
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
    
                        // Convert the input AXIOM node to an SDO DataObject
                        axiom_node_t* body = AXIOM_NODE_GET_PARENT(node, env);
                        char* str = NULL;
                        str = AXIOM_NODE_TO_STRING(body, env);
                        if (str)
                        {
                            loginfo("Received request Axis2 OM: %s", str);
                        }
    
                        // Convert the SOAP body to an SDO DataObject
                        AxiomHelper* axiomHelper = AxiomHelper::getHelper();
                        DataObjectPtr inputBodyDataObject = NULL;
    
                        try
                        {
                            inputBodyDataObject = axiomHelper->toSdo(body, dataFactory);
                        }
                        catch(SDORuntimeException &ex)
                        {
                            throw;
                        }
    
                        if(!inputBodyDataObject)
                        {
                            string msg = "Could not convert request Axis2 OM to SDO";
                            throwException(ServiceInvocationException, msg.c_str());
                        }                    
    
                        // Get the first body part representing the doc-lit-wrapped wrapper element
                        DataObjectPtr inputDataObject = NULL; 
                        PropertyList bpl = inputBodyDataObject->getInstanceProperties();
                        if (bpl.size()!=0)
                        {
                            if (bpl[0].isMany())
                            {
                                DataObjectList& parts = inputBodyDataObject->getList((unsigned int)0);
                                inputDataObject = parts[0];
                            }
                            else
                            {
                                inputDataObject = inputBodyDataObject->getDataObject(bpl[0]);
                            }
                        }
                        if (inputDataObject == NULL)
                        {
                            string msg = "Could not convert Axis2 body part to SDO";
                            throwException(ServiceInvocationException, msg.c_str());
                        }
    
                        //  Dispatch to the WS proxy
                        WSServiceProxy* proxy = (WSServiceProxy*)binding->getServiceProxy();
    
                        DataObjectPtr outputDataObject = proxy->invoke(wsdlOperation, inputDataObject);
                        
                        if(!outputDataObject)
                        {
                            return 0;
                        }
                        
                        // Convert the output DataObject to an Axiom node
                        axiom_node_t* outputNode = axiomHelper->toAxiomNode(outputDataObject,
                            wsdlOperation.getOutputTypeUri().c_str(), wsdlOperation.getOutputTypeName().c_str());
    
                        AxiomHelper::releaseHelper(axiomHelper);                                                
                        
                        str = AXIOM_NODE_TO_STRING(outputNode, env);
                        if (str)
                        {
                            loginfo("Sending response Axis2 OM : %s", str);
                        }
                            
    					return outputNode;
                    }
                }
            }
        }
        
    	string msg = "Invalid parameters in Axis2 request OM";
        throwException(ServiceInvocationException, msg.c_str());

    }
    catch(TuscanyRuntimeException& ex)
    {
        logerror("Failed to process Web service invocation: %s", (const char*)ex);
    }        
    return 0;
}


extern "C"
{                                                                                   

/**
 * Following block distinguish the exposed part of the dll.
 */

AXIS2_EXPORT int axis2_get_instance(axis2_svc_skeleton **inst,
                        axis2_env_t *env)
{
    *inst = axis2_Axis2Service_create(env);
    if(!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int axis2_remove_instance(axis2_svc_skeleton_t *inst,
                            axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;
	if (inst)
	{
        status = AXIS2_SVC_SKELETON_FREE(inst, env);
    }
    return status;
}
}
