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
using namespace commonj::sdo;
using namespace commonj::sdo_axiom;

#include "tuscany/sca/util/Utils.h"
using namespace tuscany::sca;

#include "tuscany/sca/ws/WSServiceProxy.h"
#include "Axis2Utils.h"
using namespace tuscany::sca::ws;


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

WSServiceProxy* compositeServiceProxy = NULL;

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

    if(compositeServiceProxy == NULL)
    {
        compositeServiceProxy = WSServiceProxy::getInstance();
    }

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
                    char* systemRoot = Axis2Utils::getAxisServiceParameterValue(env, msg_ctx, "TuscanySystemRoot");
                    char* fullCompositeServiceTypeName = Axis2Utils::getAxisServiceParameterValue(env, msg_ctx, "TuscanyCompositeServiceType");

                    AXIS2_LOG_INFO((env)->log, "Axis2Service invoke called with system root: %s entrypoint name: %s operation name: %s", systemRoot, fullCompositeServiceTypeName, op_name);

                    //LOGINFO_2(4, "Axis2Service invoke called with system root: %s and entrypoint name: %s", systemRoot, fullCompositeServiceTypeName);
                    //LOGINFO_1(4, "Axis2Service invoke called with operation", op_name);
                    compositeServiceProxy->init(systemRoot, fullCompositeServiceTypeName);

                    //Utils::printTypes(compositeServiceProxy->getDataFactory());

                    AxiomHelper* axiomHelper = AxiomHelper::getHelper();
                    if (compositeServiceProxy->getDataFactory() == 0)
                    {
                		AXIS2_LOG_ERROR((env)->log, AXIS2_LOG_SI, "Axis2Service_invoke: CompositeServiceType has no SCA implementation");
                        return 0;
                    }

                    char* om_str = NULL;
                    om_str = AXIOM_NODE_TO_STRING(node, env);
                    if (om_str)
                    {
                        AXIS2_LOG_INFO((env)->log, "Axis2Service invoke has request OM: %s\n", om_str);
                    }

                    DataObjectPtr inputDataObject = axiomHelper->toSdo(node, compositeServiceProxy->getDataFactory());

                    //printf("Axis2ServiceType inputDataObject: (%d)\n", inputDataObject);

                    if(!inputDataObject)
                    {
                        AXIS2_LOG_ERROR((env)->log, AXIS2_LOG_SI, "Axis2Service_invoke: Could not convert received Axiom node to SDO");
                        //LOGERROR(0, "Axis2Service_invoke: Failure whilst invoking CompositeServiceType");
                        /** TODO: return a SOAP fault here */
                        return 0;
                    }                    
                    //std::cout << "Axis2ServiceType inputDataObject:" << inputDataObject;
                    //Utils::printDO(inputDataObject);			
                    //std::cout << "Axis2ServiceType inputDataObject printed\n";
                    
                    DataObjectPtr outputDataObject = compositeServiceProxy->invoke(op_name, inputDataObject);

                    //std::cout << "Axis2ServiceType outputDataObject:" << outputDataObject;
                    //Utils::printDO(outputDataObject);			
                    //std::cout << "Axis2ServiceType outputDataObject printed\n";
                    
                    if(!outputDataObject)
                    {
                		AXIS2_LOG_ERROR((env)->log, AXIS2_LOG_SI, "Axis2Service_invoke: Failure whilst invoking CompositeServiceType");
                        //LOGERROR(0, "Axis2Service_invoke: Failure whilst invoking CompositeServiceType");
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
