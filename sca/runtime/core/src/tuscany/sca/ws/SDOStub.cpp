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

#include "SDOStub.h"
#include "commonj/sdo/HelperProvider.h"
#include "osoa/sca/export.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/util/Utils.h"


// For Axis stub code
#include <axis/AxisWrapperAPI.hpp>
#include <axis/client/Stub.hpp>
#include <axis/OtherFaultException.hpp>
#include <axis/ISoapFault.hpp>

using namespace std;

AXIS_CPP_NAMESPACE_USE

using namespace commonj::sdo;
using namespace tuscany::sca;


namespace tuscany
{
namespace sca
{
    namespace ws
    {
        /** Construct an SDOStub using the default HTTP 1.1 transport.
         */
        SDOStub::SDOStub() 
                : Stub(" ", APTHTTP1_1),
                  m_strEndpoint(""), 
                  m_strTargetNamespace(""),
                  m_strOperationName(""),
                  m_strOperationResponseName(""),
                  m_strSoapAction("")
        {
        }


        /** Construct an SDOStub using the default HTTP 1.1 transport 
         *  and set the web service endpoint URI.
         *  @param endpoint - web service endpoint URI
         */
        SDOStub::SDOStub(const char* endpoint)
                : Stub(endpoint, APTHTTP1_1), 
                  m_strEndpoint(endpoint),
                  m_strTargetNamespace(""),
                  m_strOperationName(""),
                  m_strOperationResponseName(""),
                  m_strSoapAction("")
        {
        }


        /** Destruct the SDOStub
         */
        SDOStub::~SDOStub()
        {
        }

        /** Set the web service endpoint URI.
         *  The endpoint is the value of the soap:address element in the WSDL.
         *  @param endpoint - web service endpoint URI.
         */
        void SDOStub::setEndpoint(const char* endpoint)
        {
            m_strEndpoint = endpoint;
            // Stub::setEndPoint
            setEndPoint(endpoint);
        }

        /** Get the web service endpoint URI. 
         *  @return web service endpoint URI.
         */
        const char* SDOStub::getEndpoint()
        {
            return m_strEndpoint.c_str();
        }

        /** Set the target namespace for elements and operations.
         *  @param targetNamespace - the target namespace.
          */
        void SDOStub::setTargetNamespace(const char* targetNamespace)
        {
            m_strTargetNamespace = targetNamespace;
        }

        /** Get the target namespace.
         *  @return - target namespace.
         */
        const char* SDOStub::getTargetNamesapce()
        {
            return m_strTargetNamespace.c_str();
        }

        /** Set the name of the operation to invoke. 
         *  @param operationName - operation name
         */
        void SDOStub::setOperationName(const char* operationName)
        {
            m_strOperationName = operationName;
        }

        /** Get the operation name.
         *  @return operation name.
         */
        const char* SDOStub::getOperationName()
        {
            return m_strOperationName.c_str();
        }
        
        /** Set the name of the element that is returned as the result of a soap method.
         *  @param operationResponseName - name of the response element.
         */
        void SDOStub::setOperationResponseName(const char* operationResponseName)
        {
            m_strOperationResponseName = operationResponseName;
        }

        /** Get the name of the element that is returned as the result of a soap method.
         *  @return operation response name.
         */
        const char* SDOStub::getOperationResponseName()
        {
            return m_strOperationResponseName.c_str();
        }

        /** Set the SOAPAction header.
         *  @param soapAction - the soap action.
         */
        void SDOStub::setSoapAction(const char* soapAction)
        {
            m_strSoapAction = soapAction;
        }
        
        /** Get the SoapAction header.
         *  @return soap action.
         */
        const char* SDOStub::getSoapAction()
        {
            return m_strSoapAction.c_str();
        }

        /** Invoke the operation on the web service.
         *  @param requestDO - SDO containing the input parameters for the operation.
         *  @param dataFactory - DataFactory for de/serialzing the SDOs.
         *  @return DataObjectPtr - SDO containg the response from the web service.
         */
        DataObjectPtr SDOStub::invoke(DataObjectPtr requestDO, DataFactoryPtr dataFactory)
        {
        	LOGENTRY(2, "SDOStub::invoke");
        	
            // SDO return object
            DataObjectPtr pReturn;

            // For faults
            //const char* pcCmplxFaultName = NULL;

            //  Get an XMLHelper
            XMLHelperPtr xmlHelper = HelperProvider::getXMLHelper(dataFactory);


            try
            {
                // Initialize the call structure for Doc Literal request/response.
                if (AXIS_SUCCESS != m_pCall->initialize(CPP_DOC_PROVIDER))
                {
                    return pReturn; // Return if there is a failure
                }

                // Set the SoapAction if it is not already set.
                if (NULL == m_pCall->getTransportProperty("SOAPAction",false))
                {
                    m_pCall->setTransportProperty(SOAPACTION_HEADER , m_strSoapAction.c_str());
                }

                // Set the Soap Version.
                m_pCall->setSOAPVersion(SOAP_VER_1_1);

                // Set the Operation (operationName and targetNamespace)
                m_pCall->setOperation(m_strOperationName.c_str(), m_strTargetNamespace.c_str());

                //includeSecure(); 
                //applyUserPreferences(); 

                //
                // Serialize the DataObject.
                //

                // DEBUG
                XMLDocumentPtr requestDoc = 
                    xmlHelper->createDocument(requestDO, 
                                              m_strTargetNamespace.c_str(),
                                              m_strOperationName.c_str());
                requestDoc->setXMLDeclaration(false);
                char *requestXML = xmlHelper->save(requestDoc);
                LOGINFO_1(2, "SDOStub::invoke: Request XML=%s", requestXML);

                // Loop through the top level data object, serializing individual properties.
                // We cannot simply serialize the request DataObject to an XML string and add 
                // the XML as an AnyType parameter because Axis will wrap the request with an 
                // element named for the operationName (Doc Literal/Wrapped), duplicating the
                // top-level element.
                PropertyList properties = requestDO->getInstanceProperties();
                for (int i=0; i < properties.size() ; i++)
                {
                    Property& property = properties[i];
                    const char *propertyName = property.getName();
                    
                    // If the property is not set, don't bother adding it as a parameter.
                    if (!requestDO->isValid(property)) continue;

                    switch (property.getTypeEnum())
                    {
                       // Simple string property - add it as an XSD_STRING parameter.
                       case Type::StringType:
                            {
                                if (!property.isMany())
                                {
                                    string strPrefixAndParamName;
                                    const char *namespacePrefix = 
                                        m_pCall->getNamespacePrefix(m_strTargetNamespace.c_str());
                                    strPrefixAndParamName.append(namespacePrefix);
                                    strPrefixAndParamName.append(":");
                                    strPrefixAndParamName.append(propertyName);
                                    const char *paramValue = requestDO->getCString(i);
                                    m_pCall->addParameter((void *)paramValue, 
                                                          strPrefixAndParamName.c_str(), 
                                                          XSD_STRING);
                                }
                            }
                            break;

                        // DataObject property.
                        case Type::DataObjectType:
                            {
                                const char *dobXML = NULL; 
                                if (property.isMany())
                                {
                                    // Many valued property - get the list.
                                    DataObjectList& dobList = requestDO->getList(property);
                                    for (int j=0; j< dobList.size(); j++)
                                    {
                                        DataObjectPtr dob = dobList[j];
                                        if (dob != 0)
                                        {
                                            // Create an XML string from the DataObject.
                                            XMLDocumentPtr doc = 
                                            xmlHelper->createDocument(dob, 
                                                                        m_strTargetNamespace.c_str(),
                                                                        propertyName);
                                            doc->setXMLDeclaration(false);
                                            dobXML = xmlHelper->save(doc);

                                            // Add the XML to the call as an AnyType.
                                            AnyType* pAny = new AnyType();
                                            pAny->_size = 1;
                                            pAny->_array = new char*[1];
                                            pAny->_array[0] = strdup(dobXML);
                                            m_pCall->addAnyObject(pAny);
                                        }
                                    } 
                                } // if isMany
                                else
                                {
                                    // Single valued property.
                                    DataObjectPtr dob = requestDO->getDataObject(property);
                                    if (dob != 0)
                                    {
                                        // Create an XML string from the DataObject.
                                        XMLDocumentPtr doc = 
                                            xmlHelper->createDocument(dob,
                                                                        m_strTargetNamespace.c_str(),
                                                                        propertyName);
                                        doc->setXMLDeclaration(false);
                                        dobXML = xmlHelper->save(doc);

                                        // Add the XML to the call as an AnyType.
                                        AnyType* pAny = new AnyType();
                                        pAny->_size = 1;
                                        pAny->_array = new char*[1];
                                        pAny->_array[0] = strdup(dobXML);
                                        m_pCall->addAnyObject(pAny);
                                    }
                                } 
                             }
                            break;

                        default:
                            break;
                    }
                }

                // Invoke the operation.
                if (AXIS_SUCCESS == m_pCall->invoke())
                {
                    // Make sure the response element is what we expect; ie that the element matches
                    // the operation response name set on the stub and its targetNamespace matches
                    // the targetNamespace set on the stub.
                    if (AXIS_SUCCESS == m_pCall->checkMessage(m_strOperationResponseName.c_str(), 
                                                              m_strTargetNamespace.c_str()))
                    {
                        LOGINFO(2, "SDOStub::invoke: invoke returned AXIS_SUCCESS");
                        
                        // Deserialize the soap response.
                        string strResponse = getSoapResponse();
                        
                        // Create an XMLDocument from the soap reponse
                        XMLDocumentPtr returnDoc = 
                            xmlHelper->load(strResponse.c_str(), m_strTargetNamespace.c_str());
                        // Get the root DataObject to return as the result.
                        pReturn = returnDoc->getRootDataObject();
                        
                    }
                }
                else
                {
                    LOGINFO_2(2, "SDOStub::invoke: checkMessage failed. Expected: %s \' element with namespace \' %s \'.",  
                              m_strOperationResponseName.c_str(),
                              m_strTargetNamespace.c_str());
                }


                // Invoked the operation successfully - uninitialize the Call object.
                m_pCall->unInitialize();
                
            }
            catch (AxisException& e)
            {
                // Get the exception code.
                int iExceptionCode = e.getExceptionCode();
                LOGINFO_2(0, "SDOStub::invoke Caught AxisException: %d code: %s", iExceptionCode, e.what());
                if (AXISC_NODE_VALUE_MISMATCH_EXCEPTION != iExceptionCode)
                {
                    m_pCall->unInitialize();
                     throw ServiceRuntimeException("AxisException", ServiceRuntimeException::Error, e.what());
                }

                // Get the details of the SoapFault.
                ISoapFault* pSoapFault = (ISoapFault*) m_pCall->checkFault("Fault", m_strEndpoint.c_str());

                if (pSoapFault)
                {
                    const char *detail = pSoapFault->getSimpleFaultDetail();
                    bool deleteDetail=false;

                    if (NULL==detail || 0==strlen(detail))
                    {
                        detail=m_pCall->getFaultAsXMLString();

                        
                        if (NULL==detail)
                        {
                            detail="";
                        }
                        else
                        {
                            deleteDetail=true;
                        }
                    }

                    OtherFaultException ofe(pSoapFault->getFaultcode(),
                                            pSoapFault->getFaultstring(), 
                                            pSoapFault->getFaultactor(),
                                            detail, iExceptionCode);

                    if (deleteDetail && NULL!=detail)
                    {
                        delete [] const_cast<char*>(detail);
                    }

                    m_pCall->unInitialize();
                    throw ofe;
                }
                else
                {
                    m_pCall->unInitialize();
                    throw ServiceRuntimeException("AxisException", ServiceRuntimeException::Error, e.what());
                }
            }
            catch(SDORuntimeException sdoE)
            {
                throw ServiceRuntimeException("SDORuntimeException", ServiceRuntimeException::Error, sdoE.getMessageText());
            }

        	LOGEXIT(2, "SDOStub::invoke");
            return pReturn;

        }

        // Deserialize the soap response from the web service.
        string SDOStub::getSoapResponse()
        {
            // Get the response as an XML string.
            AnyType* soapAny = m_pCall->getAnyObject();

            // The Doc Literal response does not include the top level
            // response element if we use getAnyObject - we need to wrap
            // the response with the response element.
            string strResponse("<");
            strResponse.append(m_strOperationResponseName);
            strResponse.append(" xmlns=\"");
            strResponse.append(m_strTargetNamespace);
            strResponse.append("\">");
            
            for (int j=0; j < soapAny->_size ; j++)
            {
                // DEBUG
                cout << "SDOStub::getSoapResponse: soapAny->_array[" << j << "]=" << endl;
                cout << soapAny->_array[j] << endl;
                cout << endl;
                // DEBUG
                
                strResponse.append(soapAny->_array[j]);
            }
            
            strResponse.append("</");
            strResponse.append(m_strOperationResponseName);
            strResponse.append(">");

            return strResponse;
        }

    } // end namespace ws
} // end namespace sca
} // end namespace tuscany

