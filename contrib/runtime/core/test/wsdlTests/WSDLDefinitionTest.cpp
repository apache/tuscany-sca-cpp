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

/* $Rev: 583039 $ $Date: 2007-10-08 20:49:57 -0700 (Mon, 08 Oct 2007) $ */

#include <iostream>

#include "SCATestMacros.h"
#include "WSDLDefinitionTest.h"

WSDLDefinitionTest::WSDLDefinitionTest()
{
}

WSDLDefinitionTest::~WSDLDefinitionTest()
{
}

bool WSDLDefinitionTest::testSimple()
{
  tuscany::sca::model::WSDLDefinition *wsdl =
    loadWsdl( "wsdlTests/wsdls_correct/simple.wsdl" );

  TEST_ASSERT_EQUALS(
      "http://www.tuscany.com/tests/simple.wsdl",
      wsdl->getNamespace() );

  // Get the operation based on portType, operName
  tuscany::sca::model::WSDLOperation wsdlOp1 =
    wsdl->findOperation( "simplePortType", "simpleOperation" );
  TEST_ASSERT_EQUALS( "simpleOperation", wsdlOp1.getOperationName() );

  // Get the operation based on service, portName, operName
  tuscany::sca::model::WSDLOperation wsdlOp2 =
    wsdl->findOperation( "simpleService", "simplePort", "simpleOperation" );
  TEST_ASSERT_EQUALS( "simpleOperation", wsdlOp2.getOperationName() );

  // Check the operation names
  std::list<std::string> operNames = wsdl->getOperations( "simplePortType" );
  TEST_ASSERT_EQUALS( 1, operNames.size() );
  TEST_ASSERT_EQUALS( "simpleOperation", operNames.front() );

  // Check the port type names
  std::list<std::string> portTypeNames = wsdl->getPortTypes();
  TEST_ASSERT_EQUALS( 1, portTypeNames.size() );
  TEST_ASSERT_EQUALS( "simplePortType", portTypeNames.front() );

  // Everything passed
  return true;
}

