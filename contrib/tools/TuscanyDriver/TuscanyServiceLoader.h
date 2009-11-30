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

#ifndef tuscany_sca_toys_service_loader_h
#define tuscany_sca_toys_service_loader_h

#include <string>
#include <sstream>

#include <tuscany/sca/core/SCARuntime.h>
#include <tuscany/sca/model/Composite.h>


namespace tuscany
{
  namespace sca
  {
    namespace toys
    {
      class TuscanyServiceLoader
      {
        public:
          TuscanyServiceLoader( const std::string &installRoot,
                                const std::string &systemRoot,
                                const std::string &systemPath,
                                const std::string &baseURI,
                                const std::string &defaultComponentName,
                                bool showModel = false,
                                bool showWsdl = false );

          ~TuscanyServiceLoader();

          void load();

        private:
          enum DataType
          {
            DATA_WSDL = 0,
            DATA_MODEL,
            DATA_BOTH
          };

          TuscanyServiceLoader();

          void log( std::stringstream &msg, TuscanyServiceLoader::DataType type );
          void log( const std::string &msg, TuscanyServiceLoader::DataType type );
          void getOperationsFromComposite( tuscany::sca::model::Composite *composite );
          void getComponentsFromComposite( tuscany::sca::model::Composite *composite );

          tuscany::sca::SCARuntime *tuscanyRuntime_;

          bool showModel_;
          bool showWsdl_;
          std::string tuscanyInstallRoot_;
          std::string tuscanySystemRoot_;
          std::string tuscanySystemPath_;
          std::string tuscanyBaseURI_;
          std::string tuscanyDefaultComponent_;
      };

    } // namespace toys

  } // namespace sca

} // namespace tuscany

#endif // tuscany_sca_toys_service_loader_h

