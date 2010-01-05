#ifndef sca_tests_wsdl_WSDLErrorsTest_h_
#define sca_tests_wsdl_WSDLErrorsTest_h_

#include "BaseWsdlTest.h"

class WSDLErrorsTest : public BaseWsdlTest
{
  public:

    WSDLErrorsTest();
    ~WSDLErrorsTest();

    bool testDuplicateWSDLInputOutputBinding();
    bool testDuplicateWSDLMessagePartNames();
    bool testDuplicateWSDLMessages();
    bool testDuplicateWSDLPortTypes();
    bool testDuplicateWSDLBindings();
    bool testDuplicateWSDLServices();
    bool testDuplicateSOAPAddress();
    bool testDuplicateSOAPBinding();
    bool testDuplicateSOAPBody();
    bool testDuplicateSOAPOperation();

    bool testMissingPortBinding();
    bool testMissingOperation();
    bool testMissingPortName();
    bool testMissingPortType();
    bool testMissingMessage();
    bool testMissingMessagePartName();
    bool testMissingMessagePartType();
};

#endif
