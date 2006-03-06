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

/* $Rev$ $Date$ */

#include "SCAWSHandler.h"
#include <axis/Handler.hpp>
#include <string>

AXIS_CPP_NAMESPACE_USE
using std::string;

/** Construct an SCAWSHandler.
 */
SCAWSHandler::SCAWSHandler()
{ 
    return; 
}

/** Destruct an SCAWSHandler.
 */
SCAWSHandler::~SCAWSHandler()
{
}


/** Perform any necessary initialization.
 */
int SCAWSHandler::init()
{
    return AXIS_SUCCESS;
}

/** Perform any necessary finalization.
 */
int SCAWSHandler::fini()
{
    return AXIS_SUCCESS;
}

/** Get the SCA specific properties that were set in the deployment
 *  descriptor (server.wsdd) and set them into the MessageData where 
 *  they can be retrieved by the SCAWSWrapper to invoke the correct
 *  SCA Entry Point.
 *  @param pvIMsg - pointer to IMessageData
 *
 *  @see SCAWSWrapper
 */
int SCAWSHandler::invoke(void *pvIMsg)
{    
    IMessageData *pIMsg = (IMessageData*) pvIMsg;

    if(pIMsg->isPastPivot()) 
    {
        // This is a response.
    } 
    else 
    {
        // This is a request
        
        //
        // Get the SCA specific properties from the handler.
        //

        // NOTE: WE CANNOT MAKE THE SIMPLE 'GETOPTION' CALLS BECAUSE THE 
        // STD::MAP CODE WILL CAUSE AN ACCESS VIOLATION IN AXISSERVER.DLL.
        // THE GETOPTION METHOD IN HANDLER.HPP NEEDS TO BE MOVED INTO
        // AXISSERVER.DLL AND BE EXPORTED.
        //const string& strTargetNamespace = getOption("targetNamespace");
        //const string& strSCAEntryPoint = getOption("scaEntryPoint");

        // HACK
        // BECAUSE WE CANNOT MAKE ANY MAP CALLS THAT REQUIRE PARAMETERS AND
        // CANNOT SEEM TO ITERATE OVER THE MAP IN A FOR LOOP EITHER WITHOUT
        // TRAPPING, WE RESORT TO READING THE FIRST AND LAST PROPERTIES. 
        // THIS IS ACCEPTABLE AS A WORKAROUND FOR NOW BECAUSE WE ONLY
        // HAVE TWO PROPERTIES.
        string strName;
        string strValue;
        // Read the first property in the map and set its name and value
        // into the IMessageData.
        map<string, string>::const_iterator it = m_pOption->begin();
        strName = (*it).first;
        strValue = (*it).second;
        pIMsg->setProperty(strName.c_str(), strValue.c_str());
        // Read the last property in the map and set its name and value
        // into the IMessageData.
        map<string, string>::const_reverse_iterator itr = m_pOption->rbegin();
        strName = (*itr).first;
        strValue = (*itr).second;
        pIMsg->setProperty(strName.c_str(), strValue.c_str());
        // END HACK
    }

    return AXIS_SUCCESS;
}

/** Handle Faults.
 */
void SCAWSHandler::onFault(void *pvIMsg)
{

}

//
// These functions are exported from the SCAWSHandler DLL and are called by the Axis Engine
// to create/destroy instances of the SCAWSHandler class.
//
extern "C" 
{
STORAGE_CLASS_INFO
int GetClassInstance(BasicHandler **inst)
{
    *inst = new BasicHandler();
    
    SCAWSHandler* pSCAWSHandler = new SCAWSHandler();
    (*inst)->_functions = 0;
    if (pSCAWSHandler)
    {
        (*inst)->_object = pSCAWSHandler;
        return pSCAWSHandler->init();
    }
    
    return AXIS_FAIL;
}

STORAGE_CLASS_INFO
int DestroyInstance(BasicHandler *inst)
{
    if (inst)
    {
        Handler* pH = static_cast<Handler*>(inst->_object);
        pH->fini();
        delete pH;
        delete inst;
        return AXIS_SUCCESS;
    }
    return AXIS_FAIL;
}
} // extern "C"
