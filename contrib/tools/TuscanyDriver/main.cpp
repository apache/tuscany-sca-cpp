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

#include <iostream>
#include <string>

#include <tuscany/sca/core/Exceptions.h>
#include <commonj/sdo/SDORuntimeException.h>

#include "TuscanyServiceLoader.h"

struct InputArgs
{
  std::string installRoot_;
  std::string systemRoot_;
  std::string systemPath_;
  std::string baseURI_;
  std::string defaultComponentName_;
  bool showWsdl_;
  bool showModel_;

  InputArgs() :
    showWsdl_(false),
    showModel_(false)
  {}
};

const std::string ARG_INSTALL_ROOT        = "-ir";
const std::string ARG_SYSTEM_ROOT         = "-sr";
const std::string ARG_SYSTEM_PATH         = "-sp";
const std::string ARG_BASE_URI            = "-uri";
const std::string ARG_DEFAULT_COMPONENT   = "-dc";
const std::string ARG_VERBOSE             = "-v";
const std::string ARG_SHOW_MODEL          = "-model";
const std::string ARG_SHOW_WSDL           = "-wsdl";
const std::string ARG_HELP_H              = "-h";
const std::string ARG_HELP_HELP           = "-help";
const std::string ARG_HELP_QMARK          = "-?";


void
printUsage()
{
  std::cout
    << "\nUsage\ntuscanyDriver\n\t"
    << ARG_INSTALL_ROOT         << " Mandatory: Installation root where extensions are located: ${TUSCANY_SCACPP}\n\t"
    << ARG_SYSTEM_ROOT          << " Mandatory: System root where projects are located: ${TUSCANY_SCACPP}/samples\n\t"
    << ARG_SYSTEM_PATH          << " Optional: System path\n\t"
    << ARG_BASE_URI             << " Optional: Base URI\n\t"
    << ARG_DEFAULT_COMPONENT    << " Optional: Default Component name\n\t"
    << ARG_SHOW_MODEL           << " Optional: Display SCA Model Hierarchy\n\t"
    << ARG_SHOW_WSDL            << " Optional: Display WSDL information\n\t"
    << ARG_VERBOSE              << " Optional: Same as specifying both: "
                                << ARG_SHOW_MODEL << " and " << ARG_SHOW_WSDL
    << std::endl;
}

bool
parseArgs( int argc, char *argv[], InputArgs &input )
{
  if( argc < 5 )
  {
    std::cerr << "\nInvalid number of input arguments: " << argc << std::endl;
    printUsage();
    return false;
  }

  for( int i = 1; i < argc; i++ )
  {
    if( argv[i] == ARG_INSTALL_ROOT )
    {
      input.installRoot_ = argv[++i];
    }
    else if( argv[i] == ARG_SYSTEM_ROOT )
    {
      input.systemRoot_ = argv[++i];
    }
    else if( argv[i] == ARG_SYSTEM_PATH )
    {
      input.systemPath_ = argv[++i];
    }
    else if( argv[i] == ARG_BASE_URI )
    {
      input.baseURI_ = argv[++i];
    }
    else if( argv[i] == ARG_DEFAULT_COMPONENT )
    {
      input.defaultComponentName_ = argv[++i];
    }
    else if( argv[i] == ARG_VERBOSE )
    {
      input.showWsdl_  = true;
      input.showModel_ = true;
    }
    else if( argv[i] == ARG_SHOW_MODEL )
    {
      input.showModel_ = true;
    }
    else if( argv[i] == ARG_SHOW_WSDL )
    {
      input.showWsdl_  = true;
    }
    else if( argv[i] == ARG_HELP_H ||
             argv[i] == ARG_HELP_QMARK ||
             argv[i] == ARG_HELP_HELP )
    {
      printUsage();
      return false;
    }
    else
    {
      std::cerr << "\nUnrecognized argument: " << argv[i];
      printUsage();
      return false;
    }
  }

  if( input.installRoot_.empty() )
  {
    std::cerr << "\nMissing mandatory argument: Install root " << ARG_INSTALL_ROOT << std::endl;
    return false;
  }

  if( input.systemRoot_.empty() )
  {
    std::cerr << "\nMissing mandatory argument: System root " << ARG_SYSTEM_ROOT << std::endl;
    return false;
  }

  return true;
}

int
main(int argc, char* argv[])
{
  try
  {
    InputArgs input;
    if( ! parseArgs( argc, argv, input ) )
    {
      return 1;
    }

    tuscany::sca::toys::TuscanyServiceLoader
      tuscany( input.installRoot_,
               input.systemRoot_,
               input.systemPath_,
               input.baseURI_,
               input.defaultComponentName_,
               input.showModel_,
               input.showWsdl_ );

    tuscany.load( );
  }
  catch (...)
  {
    std::cerr << "Caught unknown exception." << std::endl;
    return 1;
  }

  return 0;
}

