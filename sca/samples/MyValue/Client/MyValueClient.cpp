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

#pragma warning(disable: 4786)

#include "commonj/sdo/Logger.h"
#include "commonj/sdo/SDO.h"
#include "commonj/sdo/HelperProvider.h"
#include "osoa/sca/sca.h"
#include "tuscany/sca/util/Logger.h"
#include "tuscany/sca/util/Utils.h"
#include <iostream>

using namespace std;
using namespace osoa::sca;
using namespace tuscany::sca;
using namespace commonj::sdo;
using namespace tuscany::sca::ws;

void usage();

//
// MyValueClient is a sample application that invokes operations on the 
// MyValueService Web Service.
//
int main(int argc, char* argv[])
{

	//
	// Schema file for the types used by MyValueService
	//
	char *pszXSD;

	//
	// The WebService endpoint uri.
	//
	char *pszEndpoint;
	
	//
	// TargetNamespace
	//
	const char *pszTargetNamespace;
    
	//
	// SOAPAction
	//
	// Axis 1.5: SoapAction must be <serviceName>#<operationName>. 
	const char *pcszSoapAction = "MyValueService#getMyValue";

	//
	// Operation Name
	//
	const char *pcszOperationName = "getMyValue";

	//
	// Operation Response Name
	//
	const char *pcszOperationResponseName = "getMyValueResponse";


	// Get the input parameters.
	if (argc == 3)
	{
		// Schema file.
		pszXSD = argv[1];
		// Web Service endpoint.
		pszEndpoint = argv[2];
	}
	else
	{
		usage();
	}

	// The current version of SDO does not support the embedded schema in the wsdl file,
	// so we have to load an external schema.
	XSDHelperPtr xsdHelper = HelperProvider::getXSDHelper();

	try 
	{
		cout << "define file: " << pszXSD <<endl;
		pszTargetNamespace = xsdHelper->defineFile(pszXSD);
	}
	catch (SDORuntimeException ex)
	{
		cout << ex << endl;
		exit(1);
	}
		cout << "define file done: " << pszTargetNamespace <<endl;


	// Create an SDO for the request
	DataFactoryPtr factory = xsdHelper->getDataFactory();
    DataObjectPtr requestDO = 0;

	try 
	{
		// Create the root element for the operation and then add the parameters
		// we need to invoke the operation.
		requestDO = factory->create(pszTargetNamespace, pcszOperationName);
		requestDO->setCString("customerID", "12345");
	}
	catch (SDORuntimeException e)
	{
		cout << "SDORuntimeException: " << e << endl;
		exit(1);
	}
	
	//
	// Create an SDOStub that will use the request SDO to invoke the operation.
	//
	SDOStub *stub = new SDOStub();
	stub->setEndpoint(pszEndpoint);
	stub->setOperationName(pcszOperationName);
	stub->setTargetNamespace(pszTargetNamespace);
	stub->setOperationResponseName(pcszOperationResponseName);
	stub->setSoapAction(pcszSoapAction);
	// DEBUG - Don't let the soap transport timeout while debugging.
	stub->setTransportTimeout(0);
	// DEBUG
			
	try 
	{
        // Invoke the operation.
    	DataObjectPtr responseDO = stub->invoke(requestDO, factory);
		// Print the response.
		if (responseDO != 0)
		{
			cout << "MyValueClient: response DataObject = " << endl;
			Utils::printDO(responseDO);
			cout << endl;
			float fValue = responseDO->getFloat("myValue");
			cout << "MyValueClient: myValue = " << fValue;
		}
		else
		{
			cout << "MyValueClient: MyValueService did not return a result" << endl;
		}
	} 
	catch (SDORuntimeException& e)
	{
		cout << "SDORuntimeException: " << e << endl;
	}
	catch (ServiceRuntimeException& e)
	{
		cout << "ServiceRuntimeException: " << e << endl;
	}
	catch(exception& e)
	{
		cout << "Exception: " << e.what() << endl;
	}
	

	return 0;
}

//
// Print usage message.
//
void usage()
{
	cout << "MyValueClient.exe: Invoke operations on MyValueService Web Service" << endl;
	cout << "Usage: MyValueClient XSD endpoint" << endl;
	cout << "where  XSD = path and name of XSD file that defines the MyValue types" << endl;
	cout << "       endpoint = web service endpoint" << endl;
	exit(1);
}
