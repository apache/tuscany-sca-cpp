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

#include "BaseWsdlTest.h"

#include <tuscany/sca/model/WSDLDefinition.h>
#include <commonj/sdo/SDO.h>

BaseWsdlTest::BaseWsdlTest()
{
  // TODO: Should we possibly make this an arg to this class instead?
  char *installRootVar = getenv( "TUSCANY_SCACPP" );
  installRoot_ = installRootVar;
}

BaseWsdlTest::~BaseWsdlTest()
{
}

void BaseWsdlTest::loadXsdHelper()
{
  if( ! (xsdHelper_ == 0) )
  {
    return;
  }

  try
  {
    xsdHelper_ = commonj::sdo::HelperProvider::getXSDHelper();

    std::string filename = installRoot_ + "/xsd/wsdl_11.xsd";
    xsdHelper_->defineFile(filename.c_str());

    filename = installRoot_ + "/xsd/wsdl_11_http.xsd";
    xsdHelper_->defineFile(filename.c_str());

    filename = installRoot_ + "/xsd/wsdl_11_mime.xsd";
    xsdHelper_->defineFile(filename.c_str());

    filename = installRoot_ + "/xsd/wsdl_11_soap.xsd";
    xsdHelper_->defineFile(filename.c_str());

    filename = installRoot_ + "/xsd/wsdl_11_soap12.xsd";
    xsdHelper_->defineFile(filename.c_str());
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    std::cerr
      << "BaseWsdlTest::loadXsdHelper() SDO runtime Exception: "
      << sdoE.getMessageText()
      << std::endl;

    throw;
  }
}

void BaseWsdlTest::loadXmlHelper()
{
  if( ! (xmlHelper_ == 0) )
  {
    return;
  }

  if( xsdHelper_ == 0 )
  {
    loadXsdHelper();
  }

  try
  {
    xmlHelper_ = commonj::sdo::HelperProvider::getXMLHelper( xsdHelper_->getDataFactory() );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    std::cerr
      << "BaseWsdlTest::loadXmlHelper() SDO runtime Exception: "
      << sdoE.getMessageText()
      << std::endl;

    throw;
  }
}

tuscany::sca::model::WSDLDefinition *
BaseWsdlTest::loadWsdl( const std::string &wsdlPath )
{
  loadXmlHelper();

  tuscany::sca::model::WSDLDefinition *wsdl = 0;

  try
  {
    commonj::sdo::XMLDocumentPtr doc = xmlHelper_->loadFile( wsdlPath );
    wsdl = new tuscany::sca::model::WSDLDefinition( doc->getRootDataObject() );
//tuscany::sca::util::Utils::printDO(doc->getRootDataObject()); // TODO remove this
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    std::cerr
      << "BaseWsdlTest::loadWsdl() SDO runtime Exception: "
      << sdoE.getMessageText()
      << std::endl;

    throw;
  }

  return wsdl;
}

