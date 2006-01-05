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

/* $Rev$ $Date: 2005/12/22 11:33:21 $ */

#if !defined(SCAWSWRAPPER_H_INCLUDED)
#define SCAWSWRAPPER_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "osoa/sca/export.h"
#include <axis/server/WrapperClassHandler.hpp>
#include <axis/IMessageData.hpp>
#include <axis/GDefine.hpp>
#include <axis/AxisWrapperAPI.hpp>
#include <axis/AxisUserAPI.hpp>


AXIS_CPP_NAMESPACE_USE

/** The SCAWSWrapper class is an Axis WrapperClassHandler that acts as a generic service wrapper for
 *  invoking services using the SCARuntime. Any services defined in the deployment descriptor (server.wsdd)
 *  with <parameter name="className" value="<path>\SCAWSWrapper.dll"/> will be invoked by the SCAWSWrapper.
 *  The service must also be configured with a handler (SCAWSHandler) that sets SCA specific properties
 *  from the deployment descriptor into the MessageData that is passed to the wrapper's invoke method.  
 *  The SCAWSWrapper uses the properties to configure an SCA Entry Point and invoke the requested operation
 *  on the service.
 *  
 *  Example:
 * 
 *  <service name="ExampleService" provider="CPP:DOCUMENT" description="Example Service">
 *      <requestFlow>
 *           <handler name="SCAWSHandler" type="C:\Apache2.0.54\Apache2\Axis\handlers\SCAWSHandler.dll">
 *                <parameter name="targetNamespace" value="http://com.exampleservice"/>
 *                <parameter name="scaEntryPoint" value="SubSystem1/ExampleService/ExampleService"/>
 *            </handler>
 *       </requestFlow>
 *       
 *      <parameter name="className" value="C:\Apache2.0.54\Apache2\Axis\webservices\SCAWSWrapper.dll"/>
 *      <parameter name="allowedMethods" value="ExampleMethod "/>
 *  </service>
 *
 *  @see SCAWSHandler
 */
class SCAWSWrapper : public WrapperClassHandler
{
    public:

        /** Construct an SCAWSWrapper.
         */
        SCAWSWrapper();

        /** Destruct an SCAWSWrapper.
         */
        virtual ~SCAWSWrapper();
                    
        //
        // Axis WrapperClassHandler interface.
        //

        /** Perform any necessary initialization.
         */
        int AXISCALL init();

        /** Perform any necessary finalization.
         */
        int AXISCALL fini();

        /** Invoke a web service operation.  The invoke method expects SCA specific
         *  properties to have been set into the MessageData by the SCAWSHandler.
         *  Using those properties, the wrapper will invoke the correct SCA Entry
         *  Point.
         *  @param pMsg - pointer to IMessageData.
         *
         *  @see SCAWSHandler
         */
        int AXISCALL invoke(void* pMsg);

        /** Handle Faults.
         */
        void AXISCALL onFault(void* pMsg);
                    
        /** Binding Style: Document Literal.
         */
        AXIS_BINDING_STYLE AXISCALL getBindingStyle() { return DOC_LITERAL; };

        private:

        // Invoke an SCA Entry Point and return an SDO object as a result.
        int invokeService(IMessageData *pIMsg, 
                          const AxisChar *operationName);
};

#endif // !defined(SCAWSWRAPPER_H_INCLUDED)
