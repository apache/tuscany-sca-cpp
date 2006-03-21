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

#include "SCAWSWrapper.h"

#include "commonj/sdo/SDO.h"
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/core/SCAEntryPoint.h"

#include "axis/GDefine.h"
#include "AxisServiceException.h"


using namespace std;
using namespace commonj::sdo;
using namespace tuscany::sca;
AXIS_CPP_NAMESPACE_USE;

/** Construct an SCAWSWrapper.
 */
SCAWSWrapper::SCAWSWrapper()
{ 
}

/** Destruct an SCAWSWrapper.
 */
SCAWSWrapper::~SCAWSWrapper()
{
}


//
// Implementation of WrapperClassHandler interface
//

/** Perform any necessary initialization.
 */
int SCAWSWrapper::init()
{
    return AXIS_SUCCESS;
}

/** Perform an necessary finalization.
 */
int SCAWSWrapper::fini()
{
    return AXIS_SUCCESS;
}

/** Invoke a web service operation.  The invoke method expects SCA specific
 *  properties to have been set into the MessageData by the SCAWSHandler.
 *  Using those properties, the wrapper will invoke the correct SCA Entry
 *  Point.
 *  @param pMsg - pointer to IMessageData.
 *
 *  @see SCAWSHandler
 */
int SCAWSWrapper::invoke(void *pMsg)
{
    
    IMessageData *pIMsg = (IMessageData*)pMsg;
    const AxisChar *operationName = pIMsg->getOperationName();
    
    int axisReturn = invokeService(pIMsg, operationName);
    return axisReturn;
}

/** Handle Faults.
 */
void SCAWSWrapper::onFault(void *pMsg)
{
}


// Invoke an operation on an SCA Entry Point.
int SCAWSWrapper::invokeService(IMessageData *pIMsg, 
                                const AxisChar *operationName)
{
    int axisReturn = AXIS_SUCCESS;
    
    // Get the SoapSerializer.
    IWrapperSoapSerializer* pIWSSZ;
    pIMsg->getSoapSerializer(&pIWSSZ);
    if (!pIWSSZ)
    {
        return AXIS_FAIL;
    }

    // Get the SoapDeSerializer.
    IWrapperSoapDeSerializer* pIWSDZ;
    pIMsg->getSoapDeSerializer(&pIWSDZ);
    if (!pIWSDZ)
    {
        return AXIS_FAIL;
    }

    // Target Namespace was set into the MessageData by the SCAWSHandler.
    const AxisChar *targetNamespace = (const AxisChar *)pIMsg->getProperty("targetNamespace");

    // Make sure we have the correct message.
    if (AXIS_SUCCESS != pIWSDZ->checkMessageBody(operationName, targetNamespace))
    {
        return AXIS_FAIL;
    }

    try
    {
        //
        // Create the SCA EntryPoint
        //
        const AxisChar *scaEntryPointName = (const AxisChar *)pIMsg->getProperty("scaEntryPoint");
        SCAEntryPoint entrypoint(scaEntryPointName);        

        // Get the DataFactory which has Types loaded from WSDLs
        DataFactoryPtr dataFactory = entrypoint.getDataFactory();
    
        //
        // Get the Soap body and create an SDO request object from it.
        //
        AnyType *soapAny = pIWSDZ->getAnyObject();

        // The Doc Literal soap message does not include the root element (operation name) when
        // we use getAnyObject. Axis calls this element the 'soap method' but there is no 
        // 'getSoapMethod' method on the deserializer - we need to wrap the soap body with an
        // element named for the operation name.
                
        string soapBody("<");
        soapBody.append(operationName);
        soapBody.append(" ");
        soapBody.append("xmlns");
        soapBody.append("=\"");
        soapBody.append(targetNamespace);
        soapBody.append("\">");
        for (int i=0; i < soapAny->_size ; i++)
        {
            // The soap body is stored in the _array member of AnyType.
            soapBody.append(soapAny->_array[i]);
        }
        soapBody.append("</");
        soapBody.append(operationName);
        soapBody.append(">");

        //
        // Create the SDO request object from the soap body.
        //
        XMLHelperPtr xmlHelper = HelperProvider::getXMLHelper(dataFactory);
        XMLDocumentPtr xmlDoc = xmlHelper->load(soapBody.c_str(), targetNamespace);
        DataObjectPtr requestSDO = xmlDoc->getRootDataObject();

        //
        // Invoke the operation on the SCA EntryPoint.
        //
        DataObjectPtr responseSDO = entrypoint.invoke(operationName, requestSDO);
        
        //
        // Serialize the responseSDO into a SOAP response.
        //
        // Get the name of the response SDO. The entry point was invoked using an
        // SDOStub and the stub makes sure that the response includes the root 
        // element (operation response name).
        
        const Type &responseType = responseSDO->getType();
        const char *operationResponseName = responseType.getName();

        // Get the first child of the root element - this is the soap body 
        // to return in the response. The body will be wrapped with the root
        // element (operation response name) when we call the Axis method 
        // 'createSoapmMethod'.
        DataObjectPtr soapBodySDO = responseSDO->getDataObject((unsigned int)0);
        const Type& soapBodyType = soapBodySDO->getType();
        const char *soapBodyName = soapBodyType.getName();
        // Convert the soap body DataObject into XML.
        XMLDocumentPtr responseDoc =  xmlHelper->createDocument(soapBodySDO, 
                                                                targetNamespace,
                                                                soapBodyName);
        responseDoc->setXMLDeclaration(false);
        char *responseXML = xmlHelper->save(responseDoc);
    
        //
        // Serialize the response
        //
        // Wrap the soap body with the root element (operation response name).
        pIWSSZ->createSoapMethod(operationResponseName, targetNamespace);
        // Add the XML response document (soap body) as an AnyType.
        AnyType *soapAnyResponse = new AnyType();
        soapAnyResponse->_size = 1;
        soapAnyResponse->_array = new char*[1];
        soapAnyResponse->_array[0] = strdup(responseXML);

        pIWSSZ->addOutputAnyObject(soapAnyResponse);
        
    }
    catch(ServiceRuntimeException e)
    {
    	
        // Throw a useful fault
        string faultCode = string("Server.TuscanySCA.") + string(e.getEClassName());
        string faultString = string(e.getEClassName()) + string(":") + string (e.getMessageText());
        
        pIWSSZ->createSoapFault("ServiceRuntimeException","tempURI", faultCode.c_str(), faultString.c_str());
        throw AxisServiceException();
    }
    catch(SDORuntimeException e)
    {
    	// Throw a useful fault
        string faultCode = string("Server.TuscanySDO.") + string(e.getEClassName());
        string faultString = string(e.getEClassName()) + string(":") + string (e.getMessageText());
        
        pIWSSZ->createSoapFault("SDORuntimeException","tempURI", faultCode.c_str(), faultString.c_str());
        throw AxisServiceException();
    }


    return axisReturn;
}

//
// These functions are exported from the SCAWSWrapper DLL and are called by the Axis Engine
// to create/destroy instances of the service wrapper class.
//
extern "C" 
{
STORAGE_CLASS_INFO
int GetClassInstance(BasicHandler **inst)
{
    *inst = new BasicHandler();
    WrapperClassHandler* pWCH = new SCAWSWrapper();
    (*inst)->_functions = 0;
    if (pWCH)
    {
        (*inst)->_object = pWCH;
        return pWCH->init();
    }
    return AXIS_FAIL;
}
STORAGE_CLASS_INFO 
int DestroyInstance(BasicHandler *inst)
{
    if (inst)
    {
        WrapperClassHandler* pWCH = reinterpret_cast<WrapperClassHandler*>(inst);
        pWCH->fini();
        delete pWCH;
        delete inst;
        return AXIS_SUCCESS;
    }
    return AXIS_FAIL;
}
} // extern "C"



