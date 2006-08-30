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

#ifndef tuscany_sca_extension_ws_axis2utils_h
#define tuscany_sca_extension_ws_axis2utils_h

#include <axis2_env.h>
#include <axis2_msg_ctx.h>


namespace tuscany
{
    namespace sca
    {
        namespace ws
        {
        
            class Axis2Utils
            {
            public:
                static char* getAxisServiceParameterValue(const axis2_env_t *env, axis2_msg_ctx_t *msg_ctx, char* parameterName);                
            };
            
        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_extension_ws_axis2utils_h
