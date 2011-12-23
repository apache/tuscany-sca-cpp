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

/**
 * Axis2/C service implementation that dispatches requests to SCA Web service components.
 */

#define WANT_HTTPD_LOG 1
#include "value.hpp"
#include "string.hpp"
#include "../../modules/http/httpd.hpp"
#include "axis2.hpp"

namespace tuscany {
namespace webservice {

/**
 * Initialize the service.
 */
int AXIS2_CALL serviceInit(unused axis2_svc_skeleton_t* svc_skeleton, unused const axutil_env_t* env) {
    return AXIS2_SUCCESS;
}

/**
 * Free the service.
 */
int AXIS2_CALL serviceFree(axis2_svc_skeleton_t* svc_skeleton, const axutil_env_t* env) {
    if (svc_skeleton)
        AXIS2_FREE(env->allocator, svc_skeleton);
    return AXIS2_SUCCESS;
}

typedef struct axis2_apache2_out_transport_info {
    axis2_http_out_transport_info_t out_transport_info;
    request_rec *request;
    axis2_char_t *encoding;
} axis2_apache2_out_transport_info_t;

extern "C" {
    extern module axis2_module;
}

/**
 * Service invoke function, called by Axis2/C.
 */
axiom_node_t *AXIS2_CALL serviceInvoke(unused axis2_svc_skeleton_t* svc_skeleton, const axutil_env_t* env, axiom_node_t* node, axis2_msg_ctx_t* msg_ctx) {

    // Check that we have an input node
    if (node == NULL || axiom_node_get_node_type(node, env) != AXIOM_ELEMENT)
        return NULL;
    axiom_element_t *e = (axiom_element_t *) axiom_node_get_data_element(node, env);
    if (e == NULL)
        return NULL;

    // Get the function name
    const char* func = axiom_element_get_localname(e, env);
    if (func == NULL)
        return NULL;

    // Get the target endpoint address
    const axis2_endpoint_ref_t* epr = axis2_msg_ctx_get_from(msg_ctx, env);
    if (epr == NULL)
        return NULL;
    string address = axis2_endpoint_ref_get_address(epr, env);

    // Get the underlying HTTPD request
    axis2_out_transport_info_t* tinfo = axis2_msg_ctx_get_out_transport_info(msg_ctx, env);
    axis2_apache2_out_transport_info_t* httpinfo = (axis2_apache2_out_transport_info_t*)tinfo;
    request_rec* r = httpinfo->request;
    httpdDebugRequest(r, "webservice::serviceInvoke");

    // Parse the request Axiom node and construct request expression
    Axis2Context ax(env);
    const failable<const list<value> > lv = axiomNodeToValues(node, ax);
    if (!hasContent(lv))
        return NULL;
    const value expr = mklist<value>(func, content(lv));
    debug(expr, "webservice::serviceInvoke::expr");

    // Retrieve the target lambda function from the HTTPD request and invoke it
    const value* rv = const_cast<const value*>((value*)ap_get_module_config(r->request_config, &axis2_module));
    cout << "relay: " << rv << endl;
    const lambda<value(const list<value>&)> relay = *rv;
    const value res = relay(expr);
    debug(res, "webservice::serviceInvoke::result");

    // Construct response Axiom node
    const failable<axiom_node_t*> rnode = valuesToAxiomNode(res, ax);
    if (!hasContent(rnode))
        return NULL;
    return content(rnode);
}

/**
 * Return a new service skeleton.
 */
const axis2_svc_skeleton_ops_t serviceOps = {
    serviceInit,
    serviceInvoke,
    NULL,
    serviceFree,
    NULL
};

AXIS2_EXTERN axis2_svc_skeleton_t *AXIS2_CALL serviceSkeleton(const axutil_env_t* env) {
    axis2_svc_skeleton_t* svc_skeleton = (axis2_svc_skeleton_t*)AXIS2_MALLOC(env->allocator, sizeof(axis2_svc_skeleton_t));
    svc_skeleton->ops = &serviceOps;
    svc_skeleton->func_array = NULL;
    return svc_skeleton;
}

}
}

extern "C"
{

/**
 * Axis2/C service entry point functions.
 */
AXIS2_EXPORT int axis2_get_instance(struct axis2_svc_skeleton** inst, const axutil_env_t* env) {
    *inst = tuscany::webservice::serviceSkeleton(env);
    if (inst == NULL)
        return AXIS2_FAILURE;
    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int axis2_remove_instance(axis2_svc_skeleton_t* inst, const axutil_env_t* env) {
    if (inst != NULL)
        return AXIS2_SVC_SKELETON_FREE(inst, env);
    return AXIS2_FAILURE;
}

}
