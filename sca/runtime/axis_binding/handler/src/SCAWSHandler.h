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

#if !defined(SCAWSHANDLER_H_INCLUDED)
#define SCAWSHANDLER_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <axis/Handler.hpp>

/** The SCAWSHandler class is an Axis Handler that retrieves properties set
 *  in the deployment descriptor (server.wsdd) and sets them into the 
 *  MessageData for use by the SCAWSWrapper which invokes SCA Entry Points.
 *
 *  @see SCAWSWrapper
 */
AXIS_CPP_NAMESPACE_USE

class SCAWSHandler : public Handler
{
    public:
        /** Construct an SCAWSHandler.
         */
        SCAWSHandler(void);

        /** Destruct an SCAWSHandler.
         */
        virtual ~SCAWSHandler(void);

        // Axis WrapperClassHandler interface.

        /** Perform an necessary initialization.
         */
        int AXISCALL init();

        /** Perform any necessary finalization.
         */
        int AXISCALL fini();

        /** Get the SCA specific properties that were set in the deployment
         *  descriptor (server.wsdd) and set them into the MessageData where 
         *  they can be retrieved by the SCAWSWrapper to invoke the correct
         *  SCA Entry Point.
         *  @param pvIMsg - pointer to IMessageData
         *
         *  @see SCAWSWrapper
         */
        int AXISCALL invoke(void* pvIMsg);

        /** Handle Faults.
         */
        void AXISCALL onFault(void* pvIMsg);

    private:

};


#endif // !defined(SCAWSHANDLER_H_INCLUDED)


