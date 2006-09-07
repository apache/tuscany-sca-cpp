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
using commonj::sdo_axiom::AxiomHelper;

#include "tuscany/sca/util/Exceptions.h"
#include "WSServiceProxy.h"
#include "model/WSReferenceBinding.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/CompositeService.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/WSDLDefinition.h"
#include "tuscany/sca/model/WSDLOperation.h"
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
static string serviceName = "";
static TuscanyRuntime* tuscanyRuntime = NULL;
static CompositeService* compositeService = NULL;

void initTuscanyRuntime(const axis2_env_t *env, const char* root, const char* service)
{
    AXIS2_LOG_INFO((env)->log, "Axis2Service init : %s $s\n", root, service);
    try
    {

        bool newInitParams = false;

        if(systemRoot != root)
        {
            systemRoot = root;
            newInitParams = true;
        }

        if(serviceName != service)
        {
            serviceName = service;
            newInitParams = true;
        }
        
        // service is of the form "component name"/"composite service name"
        string defaultComponentName, compositeServiceName;
        Utils::rTokeniseString("/", service, defaultComponentName, compositeServiceName);

        if(tuscanyRuntime == NULL)
        {
            AXIS2_LOG_INFO((env)->log, "Creating new Tuscany runtime\n");
            tuscanyRuntime = new TuscanyRuntime(defaultComponentName, systemRoot);
            tuscanyRuntime->start();
        }
        else if(tuscanyRuntime != NULL && newInitParams)
        {
            AXIS2_LOG_INFO((env)->log, "Restarting Tuscany runtime\n");
            tuscanyRuntime->stop();
            tuscanyRuntime->setDefaultComponentName(defaultComponentName);
            tuscanyRuntime->setSystemRoot(systemRoot);
            tuscanyRuntime->start();
        }

        if(compositeService == NULL)
        {
            Composite* composite = (Composite*)SCARuntime::getInstance()->getDefaultComponent()->getType();
            compositeService = (CompositeService*)composite->findComponent(compositeServiceName);
        }
        else
        {
            if(newInitParams)
            {
                Composite* composite = SCARuntime::getInstance()->getDefaultComponent()->getComposite();
                compositeService = (CompositeService*)composite->findComponent(compositeServiceName);
            }
        }
    }
    catch(SystemConfigurationException &ex)
    {   
        AXIS2_LOG_ERROR((env)->log, AXIS2_LOG_SI,  "SystemConfigurationException has been caught: %s\n", ex.getMessageText());
    }
    catch(TuscanyRuntimeException &ex)
    {   
        AXIS2_LOG_ERROR((env)->log, AXIS2_LOG_SI,  "%s has been caught: %s\n", ex.getEClassName(), ex.getMessageText());
    }  
}


/*
 * This method invokes the right service method 
 */
axiom_node_t* AXIS2_CALL
Axis2Service_invoke(axis2_svc_skeleton_t *svc_skeleton,
            const axis2_env_t *env,
            axiom_node_t *node,
            axis2_msg_ctx_t *msg_ctx)
{
	axiom_node_t *returnNode = node;

    /* Depending on the function name invoke the
     *  corresponding Axis2Service method
     */
    if (node)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(node, env) == AXIOM_ELEMENT)
        {
            axiom_element_t *element = NULL;
            element = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(node, env);
            if (element)
            {
                // This gets the operation name from the root element name - this is correct for DocLit Wrapped style
                axis2_char_t *op_name = AXIOM_ELEMENT_GET_LOCALNAME(element, env);
                if (op_name)
				{
                    char* root = Axis2Utils::getAxisServiceParameterValue(env, msg_ctx, "TuscanySystemRoot");
                    char* service = Axis2Utils::getAxisServiceParameterValue(env, msg_ctx, "TuscanyService");
                    
                    AXIS2_LOG_INFO((env)->log, "Axis2Service invoke called with system root: %s entrypoint name: %s operation name: %s", root, service, op_name);

                    //LOGINFO_2(4, "Axis2Service invoke called with system root: %s and entrypoint name: %s", systemRoot, fullCompositeServiceName);
                    //LOGINFO_1(4, "Axis2Service invoke called with operation", op_name);
                    initTuscanyRuntime(env, root, service);

                    //Utils::printTypes(compositeServiceProxy->getDataFactory());

                    DataFactoryPtr dataFactory = compositeService->getComposite()->getDataFactory();
                    if (dataFactory == 0)
                    {
                		AXIS2_LOG_ERROR((env)->log, AXIS2_LOG_SI, "Axis2Service_invoke: CompositeService has no SCA implementation");
                        return 0;
                    }

                    AxiomHelper* axiomHelper = AxiomHelper::getHelper();
                    char* om_str = NULL;
                    om_str = AXIOM_NODE_TO_STRING(node, env);
                    if (om_str)
                    {
                        AXIS2_LOG_INFO((env)->log, "Axis2Service invoke has request OM: %s\n", om_str);
                    }

                    // Get the WS binding
                    WSReferenceBinding* binding = (WSReferenceBinding*)compositeService->getReference()->getBinding();
                    string portNamespace = binding->getWSDLNamespaceURL();
                    
                    // Lookup the WSDL model from the composite, keyed on the namespace 
                    // (the wsdl will have been loaded at startup)
                    Composite* composite = compositeService->getComposite();
                    WSDLDefinition* wsdlDefinition = composite->findWSDLDefinition(portNamespace);
                    if (wsdlDefinition == 0)
                    {
                        string msg = "WSDL not found for " + portNamespace;
                        throw SystemConfigurationException(msg.c_str());
                    }
                    
                    // Find the target operation in the wsdl port type.
                    const WSDLOperation& wsdlOperation =  wsdlDefinition->findOperation(
                        binding->getServiceName(),
                        binding->getPortName(),
                        op_name);
                    
                    // Get the input namespace
                    const char* inputNamespace = wsdlOperation.getInputTypeUri().c_str();

                    // Convert the input AXIOM node to an SDO DataObject
                    DataObjectPtr inputDataObject = axiomHelper->toSdo(node, dataFactory, inputNamespace);

                    //printf("Axis2ServiceType inputDataObject: (%d)\n", inputDataObject);

                    if(!inputDataObject)
                    {
                        AXIS2_LOG_ERROR((env)->log, AXIS2_LOG_SI, "Axis2Service_invoke: Could not convert received Axiom node to SDO");
                        //LOGERROR(0, "Axis2Service_invoke: Failure whilst invoking CompositeService");
                        /** TODO: return a SOAP fault here */
                        return 0;
                    }                    
                    //std::cout << "Axis2ServiceType inputDataObject:" << inputDataObject;
                    //Utils::printDO(inputDataObject);			
                    //std::cout << "Axis2ServiceType inputDataObject printed\n";
                    
                    //
                    //  Dispatch to the WS proxy
                    //
                    
                    WSServiceProxy* proxy = (WSServiceProxy*)binding->getServiceProxy();
                    DataObjectPtr outputDataObject = proxy->invoke(op_name, inputDataObject);

                    //std::cout << "Axis2ServiceType outputDataObject:" << outputDataObject;
                    //Utils::printDO(outputDataObject);			
                    //std::cout << "Axis2ServiceType outputDataObject printed\n";
                    
                    if(!outputDataObject)
                    {
                		AXIS2_LOG_ERROR((env)->log, AXIS2_LOG_SI, "Axis2Service_invoke: Failure whilst invoking CompositeService");
                        //LOGERROR(0, "Axis2Service_invoke: Failure whilst invoking CompositeService");
                        /** TODO: return a SOAP fault here */
                        return 0;
                    }

					returnNode = axiomHelper->toAxiomNode(outputDataObject);
						
                    om_str = NULL;
                    om_str = AXIOM_NODE_TO_STRING(returnNode, env);
                    if (om_str)
                    {
                        AXIS2_LOG_INFO((env)->log, "Axis2Service invoke has response OM : %s\n", om_str);
                    }
                        
					AxiomHelper::releaseHelper(axiomHelper);												

                	//LOGEXIT(1, "Axis2Service_invoke");						
					return returnNode;
                }
            }
        }
    }
    
	AXIS2_LOG_ERROR((env)->log, AXIS2_LOG_SI, "Axis2Service_invoke: invalid OM parameters in request");
    //LOGERROR(0, "Axis2Service service ERROR: invalid OM parameters in request\n");
    
    /** TODO: return a SOAP fault here */
    return node;
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
    /*if(NULL != *inst)
    {
        status = *inst->init();
    }*/
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
