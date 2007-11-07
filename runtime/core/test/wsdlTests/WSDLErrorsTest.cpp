
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

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_binding_input.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    std::cout << "Caught SCA Exception: " << scaE.getMessageText() << std::endl;
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
    std::cout << "Caught SCA Exception: " << scaE.getMessageText() << std::endl;
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
    std::cout << "Caught SCA Exception: " << scaE.getMessageText() << std::endl;
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
    std::cout << "Caught SCA Exception: " << scaE.getMessageText() << std::endl;
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

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_partNames.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    std::cout << "Caught SCA Exception: " << scaE.getMessageText() << std::endl;
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
    std::cout << "Caught SCA Exception: " << scaE.getMessageText() << std::endl;
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

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_soap_address.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    std::cout << "Caught SCA Exception: " << scaE.getMessageText() << std::endl;
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

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_soap_binding.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    std::cout << "Caught SCA Exception: " << scaE.getMessageText() << std::endl;
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

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_soap_body.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    std::cout << "Caught SCA Exception: " << scaE.getMessageText() << std::endl;
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

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/duplicate_soap_operation.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    std::cout << "Caught SCA Exception: " << scaE.getMessageText() << std::endl;
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

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/missing_name_for_part.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    std::cout << "Caught SCA Exception: " << scaE.getMessageText() << std::endl;
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

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/missing_name_for_port.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    std::cout << "Caught SCA Exception: " << scaE.getMessageText() << std::endl;
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

  try
  {
    tuscany::sca::model::WSDLDefinition *wsdl =
      loadWsdl( "wsdlTests/wsdls_erroneous/missing_type_for_part.wsdl" );
    TEST_FAIL( "Wsdl should have thrown an exception" );
  }
  catch( const tuscany::sca::TuscanyRuntimeException &scaE )
  {
    std::cout << "Caught SCA Exception: " << scaE.getMessageText() << std::endl;
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

