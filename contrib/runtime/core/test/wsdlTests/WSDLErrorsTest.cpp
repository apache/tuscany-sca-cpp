
#include <iostream>
#include <string>

#include "SCATestMacros.h"
#include "WSDLErrorsTest.h"

#include <commonj/sdo/SDO.h>
#include <tuscany/sca/core/Exceptions.h>
#include <tuscany/sca/util/File.h>

WSDLErrorsTest::WSDLErrorsTest()
{
}

WSDLErrorsTest::~WSDLErrorsTest()
{
}


  // Each wsdl in the wsdls_erroneous directory will throw a different
  // exception, so we need to check for each individual exception

bool WSDLErrorsTest::testDuplicateWSDLInputOutputBinding()
{
  TEST_TRACE( "Testing erroneous wsdl: duplicate_binding_input.wsdl" );

  // This test fails because of JIRA 1900

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_binding_input.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_FAIL( std::string( "Caught unexpected SCA Exception: " ) + scaE.getMessageText() );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception: " );
  }

  TEST_TRACE( "Testing erroneous wsdl: duplicate_binding_output.wsdl" );

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_binding_output.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_FAIL( std::string( "Caught unexpected SCA Exception: " ) + scaE.getMessageText() );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception: " );
  }

  return true;
}


bool WSDLErrorsTest::testDuplicateWSDLMessagePartNames()
{
  TEST_TRACE( "Testing erroneous wsdl: duplicate_partNames.wsdl" );

  // This test fails because of JIRA 1900

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_partNames.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_FAIL( std::string( "Caught unexpected SCA Exception: " ) + scaE.getMessageText() );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception: " );
  }

  return true;
}

bool WSDLErrorsTest::testDuplicateWSDLMessages()
{
  TEST_TRACE( "Testing erroneous wsdl: duplicate_messages.wsdl" );

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_messages.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_TRACE( std::string( "Caught expected SCA Exception: " ) + scaE.getMessageText() );
    std::string errorText( scaE.getMessageText() );
    TEST_ASSERT_NOT_EQUALS( errorText.find( "message/name" ), std::string::npos );
    TEST_ASSERT_NOT_EQUALS( errorText.find( "cannot be duplicated" ), std::string::npos );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception: " );
  }

  return true;
}

bool WSDLErrorsTest::testDuplicateWSDLPortTypes()
{
  TEST_TRACE( "Testing erroneous wsdl: duplicate_portTypes.wsdl" );

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_portTypes.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_TRACE( std::string( "Caught expected SCA Exception: " ) + scaE.getMessageText() );
    std::string errorText( scaE.getMessageText() );
    TEST_ASSERT_NOT_EQUALS( errorText.find( "portType/name" ), std::string::npos );
    TEST_ASSERT_NOT_EQUALS( errorText.find( "cannot be duplicated" ), std::string::npos );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception: " );
  }

  return true;
}

bool WSDLErrorsTest::testDuplicateWSDLBindings()
{
  TEST_TRACE( "Testing erroneous wsdl: duplicate_bindings.wsdl" );

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_bindings.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_TRACE( std::string( "Caught expected SCA Exception: " ) + scaE.getMessageText() );
    std::string errorText( scaE.getMessageText() );
    TEST_ASSERT_NOT_EQUALS( errorText.find( "binding/name" ), std::string::npos );
    TEST_ASSERT_NOT_EQUALS( errorText.find( "cannot be duplicated" ), std::string::npos );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception: " );
  }

  return true;
}

bool WSDLErrorsTest::testDuplicateWSDLServices()
{
  TEST_TRACE( "Testing erroneous wsdl: duplicate_services.wsdl" );

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_services.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_TRACE( std::string( "Caught expected SCA Exception: " ) + scaE.getMessageText() );
    std::string errorText( scaE.getMessageText() );
    TEST_ASSERT_NOT_EQUALS( errorText.find( "service/name" ), std::string::npos );
    TEST_ASSERT_NOT_EQUALS( errorText.find( "cannot be duplicated" ), std::string::npos );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception: " );
  }

  return true;
}

bool WSDLErrorsTest::testDuplicateSOAPAddress()
{
  TEST_TRACE( "Testing erroneous wsdl: duplicate_soap_address.wsdl" );

  // This test fails due to an SDO SPEC limitation:
  //   According to XSD rules, if a global element xsd:any has
  //   maxOccurs > 1 you can only have a single valued property
  // The multiple SOAP addresses/bindings/bodies/operations should load
  // and I should be able to get the list and throw if list.size() > 1
  // but I cant because SDO says its a single value element

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_soap_address.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_FAIL( std::string( "Caught unexpected SCA Exception: " ) + scaE.getMessageText() );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception: " );
  }

  return true;
}

bool WSDLErrorsTest::testDuplicateSOAPBinding()
{
  TEST_TRACE( "Testing erroneous wsdl: duplicate_soap_binding.wsdl" );

  // This test fails due to an SDO SPEC limitation:
  //   According to XSD rules, if a global element xsd:any has
  //   maxOccurs > 1 you can only have a single valued property
  // The multiple SOAP addresses/bindings/bodies/operations should load
  // and I should be able to get the list and throw if list.size() > 1
  // but I cant because SDO says its a single value element

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_soap_binding.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_FAIL( std::string( "Caught unexpected SCA Exception: " ) + scaE.getMessageText() );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception: " );
  }

  return true;
}

bool WSDLErrorsTest::testDuplicateSOAPBody()
{
  TEST_TRACE( "Testing erroneous wsdl: duplicate_soap_body.wsdl" );

  // This test fails due to an SDO SPEC limitation:
  //   According to XSD rules, if a global element xsd:any has
  //   maxOccurs > 1 you can only have a single valued property
  // The multiple SOAP addresses/bindings/bodies/operations should load
  // and I should be able to get the list and throw if list.size() > 1
  // but I cant because SDO says its a single value element

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_soap_body.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_FAIL( std::string( "Caught unexpected SCA Exception: " ) + scaE.getMessageText() );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception: " );
  }

  return true;
}

bool WSDLErrorsTest::testDuplicateSOAPOperation()
{
  TEST_TRACE( "Testing erroneous wsdl: duplicate_soap_operation.wsdl" );

  // This test fails due to an SDO SPEC limitation:
  //   According to XSD rules, if a global element xsd:any has
  //   maxOccurs > 1 you can only have a single valued property
  // The multiple SOAP addresses/bindings/bodies/operations should load
  // and I should be able to get the list and throw if list.size() > 1
  // but I cant because SDO says its a single value element

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_soap_operation.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_FAIL( std::string( "Caught unexpected SCA Exception: " ) + scaE.getMessageText() );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception: " );
  }

  return true;
}

bool WSDLErrorsTest::testMissingPortBinding()
{
  TEST_TRACE( "Testing erroneous wsdl: missing_binding_for_port.wsdl" );

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/missing_binding_for_port.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_TRACE( std::string( "Caught expected SCA Exception: " ) + scaE.getMessageText() );
    std::string errorText( scaE.getMessageText() );
    TEST_ASSERT_NOT_EQUALS( errorText.find( "Unable to find binding" ), std::string::npos );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception" );
  }

  return true;
}

bool WSDLErrorsTest::testMissingOperation()
{
  TEST_TRACE( "Testing erroneous wsdl: missing_operation.wsdl" );

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/missing_operation.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_TRACE( std::string( "Caught expected SCA Exception: " ) + scaE.getMessageText() );
    std::string errorText( scaE.getMessageText() );
    TEST_ASSERT_NOT_EQUALS( errorText.find( "Unable to find PortType operation" ), std::string::npos );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception" );
  }

  return true;
}

bool WSDLErrorsTest::testMissingPortType()
{
  TEST_TRACE( "Testing erroneous wsdl: missing_portType.wsdl" );

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/missing_portType.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_TRACE( std::string( "Caught expected SCA Exception: " ) + scaE.getMessageText() );
    std::string errorText( scaE.getMessageText() );
    TEST_ASSERT_NOT_EQUALS( errorText.find( "Unable to find PortType" ), std::string::npos );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception" );
  }

  return true;
}

bool WSDLErrorsTest::testMissingMessage()
{
  TEST_TRACE( "Testing erroneous wsdl: missing_message.wsdl" );

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/missing_message.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_TRACE( std::string( "Caught expected SCA Exception: " ) + scaE.getMessageText() );
    std::string errorText( scaE.getMessageText() );
    TEST_ASSERT_NOT_EQUALS( errorText.find( "unable to find PortType input message" ), std::string::npos );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception" );
  }

  return true;
}

bool WSDLErrorsTest::testMissingMessagePartName()
{
  TEST_TRACE( "Testing erroneous wsdl: missing_name_for_part.wsdl" );

  // This test fails because of JIRA 1901

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/missing_name_for_part.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_FAIL( std::string( "Caught unexpected SCA Exception: " ) + scaE.getMessageText() );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception: " );
  }

  return true;
}

bool WSDLErrorsTest::testMissingPortName()
{
  TEST_TRACE( "Testing erroneous wsdl: missing_name_for_port.wsdl" );

  // This test fails because of JIRA 1901

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/missing_name_for_port.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_FAIL( std::string( "Caught unexpected SCA Exception: " ) + scaE.getMessageText() );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception: " );
  }

  return true;
}

bool WSDLErrorsTest::testMissingMessagePartType()
{
  TEST_TRACE( "Testing erroneous wsdl: missing_type_for_part.wsdl" );

  // This test fails because of JIRA 1901

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/missing_type_for_part.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    TEST_FAIL( std::string( "Caught unexpected SCA Exception: " ) + scaE.getMessageText() );
  }
  catch( const commonj::sdo::SDORuntimeException &sdoE )
  {
    TEST_FAIL( std::string( "Caught unexpected SDO Exception: " ) + sdoE.getMessageText() );
  }
  catch( ... )
  {
    TEST_FAIL( "Caught unexpected unknown Exception: " );
  }

  return true;
}

