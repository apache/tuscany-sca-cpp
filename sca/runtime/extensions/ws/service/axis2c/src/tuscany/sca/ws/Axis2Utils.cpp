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
#include "Axis2Utils.h"

#include <axis2_svc_ctx.h>
#include <axis2_defines.h>

#include "tuscany/sca/util/Logging.h"

using namespace tuscany::sca;
using namespace tuscany::sca::ws;


/* Static utility method to retrieve service parameters from the service.xml file
 * Get an odd linking error if this method (or the header's it requires) are in
 * a different class.
 */
char* Axis2Utils::getAxisServiceParameterValue(const axis2_env_t *env, axis2_msg_ctx_t *msg_ctx, char* parameterName)
{
    struct axis2_svc *svc = NULL;
    struct axis2_op_ctx *op_ctx = NULL;
    struct axis2_svc_ctx *svc_ctx = NULL;
    axis2_param_t *param = NULL;
    char* paramValue = NULL;

    LOGENTRY(1, "Axis2Utils::getAxisServiceParameterValue");

    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    svc_ctx = AXIS2_OP_CTX_GET_PARENT(op_ctx, env);
    svc = AXIS2_SVC_CTX_GET_SVC(svc_ctx, env);
    if(NULL == svc)
    {
        return NULL;
    }

    param = AXIS2_SVC_GET_PARAM(svc, env, parameterName);
    if(!param)
    {
		LOGERROR_1(1,"Axis parameter %s cannot be found", parameterName);
    }
    else
    {
	    paramValue = (char*) AXIS2_PARAM_GET_VALUE(param, env);
    }

    LOGEXIT(1, "Axis2Utils::getAxisServiceParameterValue");
	return paramValue;
}

