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

/* $Rev: $ $Date: $ */

#ifndef sca_tests_macros_h_
#define sca_tests_macros_h_

#include <iostream>

#define TEST_TRACE( message ) \
    std::cout << "\tTRACE: " << message << std::endl;

#define TEST_ASSERT( val ) \
    if( ! val ) { \
      std::cout << "Test failed in: " << __FILE__ << ": " << __LINE__ << std::endl; \
      return false; \
    }

#define TEST_ASSERT_EQUALS( val1, val2 ) \
    if( val1 != val2 ) { \
      std::cout << "Test failed in: " << __FILE__ << ": " << __LINE__ << std::endl; \
      return false; \
    }

#define TEST_ASSERT_NOT_EQUALS( val1, val2 ) \
    if( val1 == val2 ) { \
      std::cout << "Test failed in: " << __FILE__ << ": " << __LINE__ << std::endl; \
      return false; \
    }

#define TEST_FAIL( message ) \
    std::cout << "Test failed in: " << __FILE__ << ":" << __LINE__ << ": " << message << std::endl; \
    return false;

// For this macro, you must have the following variables defined:
//     int testsPassed = 0;
//     int testsTotal = 0;
//     bool retval = false;
// The macro calls the test and increments the total and passed
// tests and logs accordingly
// 
#define TEST(testName) \
    ++testsTotal; \
    try { \
      std::cout << "\nTest " << testsTotal << ": " << #testName << " ..... " << std::endl; \
      retval = testName; \
      if (retval) { \
        std::cout << "PASSED" << std::endl; \
        ++testsPassed; \
      } \
      else { \
        std::cout << "FAILED" << std::endl; \
      } \
    } \
    catch(const tuscany::sca::TuscanyRuntimeException &scaE) { \
        std::cout << "FAILED (unexpected SCA exception): " << scaE.getMessageText() \
                  << std::endl; \
    } \
    catch(const commonj::sdo::SDORuntimeException &sdoE) { \
        std::cout << "FAILED (unexpected SDO exception): " << sdoE.getMessageText() \
                  << std::endl; \
    } \
    catch(...) { \
        std::cout << "FAILED (unexpected unknown exception)" << std::endl; \
    }

#endif
