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
#include <sstream>
#include <list>

#include <tuscany/sca/core/SCARuntime.h>
#include <tuscany/sca/core/Exceptions.h>

#include <tuscany/sca/model/Composite.h>
#include <tuscany/sca/model/CompositeReference.h>
#include <tuscany/sca/model/CompositeService.h>
#include <tuscany/sca/model/Component.h>
#include <tuscany/sca/model/Binding.h>
#include <tuscany/sca/model/Service.h>
#include <tuscany/sca/model/ServiceType.h>
#include <tuscany/sca/model/WSDLDefinition.h>
#include <tuscany/sca/model/WSDLOperation.h>
#include <tuscany/sca/model/WSDLMessagePart.h>

#include <commonj/sdo/SDORuntimeException.h>

#include "TuscanyServiceLoader.h"

namespace tuscany
{
  namespace sca
  {
    namespace toys
    {

      TuscanyServiceLoader::TuscanyServiceLoader( const std::string &installRoot,
                                                  const std::string &systemRoot,
                                                  const std::string &systemPath,
                                                  const std::string &baseURI,
                                                  const std::string &defaultComponent,
                                                  bool showModel,   // defaults false
                                                  bool showWsdl ) : // defaults false
        tuscanyRuntime_(0),
        tuscanyInstallRoot_(installRoot),
        tuscanySystemRoot_(systemRoot),
        tuscanySystemPath_(systemPath),
        tuscanyBaseURI_(baseURI),
        tuscanyDefaultComponent_(defaultComponent),
        showModel_(showModel),
        showWsdl_(showWsdl)
      {
      }

      TuscanyServiceLoader::~TuscanyServiceLoader()
      {
      }

      void TuscanyServiceLoader::load( )
      {
        try
        {
          tuscanyRuntime_ =
            tuscany::sca::SCARuntime::initializeSharedRuntime(
                tuscanyInstallRoot_,
                tuscanySystemRoot_,
                tuscanySystemPath_,
                tuscanyBaseURI_,
                tuscanyDefaultComponent_ );

          tuscany::sca::model::Composite* systemComposite = tuscanyRuntime_->getSystem();

          // The system composite shouldnt ever have WSDL namespaces defined on it,
          // Its basically a container of included composites

          //
          // Iterate through the System included composites
          //
          std::list<std::string> compositeNameList = systemComposite->getIncludedComposites();

          if( compositeNameList.empty() )
          {
            log( "The SCARuntime system composite has no included composites.", DATA_BOTH );
          }

          std::stringstream logMsg;

          std::list<std::string>::const_iterator cNameIter    = compositeNameList.begin();
          std::list<std::string>::const_iterator cNameIterEnd = compositeNameList.end();

          // Iterate through the Composites included in the System Composite
          for( ; cNameIter != cNameIterEnd; ++cNameIter )
          {
            tuscany::sca::model::Composite *includedComposite =
              systemComposite->findIncludedComposite( *cNameIter );

            if( 0 == includedComposite )
            {
              logMsg << "Skipping NULL System Included composite: " << *cNameIter;
              log( logMsg, DATA_BOTH );

              continue;
            }

            logMsg << "System Included composite: " << includedComposite->getName();
            log( logMsg, DATA_BOTH );

            if( showWsdl_ )
            {
              log( "WSDL Data:", DATA_WSDL );
              getOperationsFromComposite( includedComposite );
            }

            if( showModel_ )
            {
              log( "MODEL Data:", DATA_WSDL );
              getComponentsFromComposite( includedComposite );
            }

            log( "\nServices loaded correctly.\n", DATA_BOTH );
          }
        }
        catch (const tuscany::sca::TuscanyRuntimeException &tuscanyE)
        {
          std::cerr << "Tuscany Runtime Exception: " << tuscanyE.getMessageText() << std::endl;
        }
        catch (const commonj::sdo::SDORuntimeException &sdoE)
        {
          std::cerr << "SDO Runtime Exception: " << sdoE.getMessageText() << std::endl;
        }
        catch (const std::exception &stdE)
        {
          std::cerr << "Standard Exception: " << stdE.what() << std::endl;
        }
      }

      // private
      void TuscanyServiceLoader::getOperationsFromComposite( tuscany::sca::model::Composite *composite )
      {
        std::stringstream logMsg;

        std::list<std::string> namespaceList = composite->getWSDLNamespaces();
        if( namespaceList.empty() )
        {
          logMsg << "Composite has no WSDLs to process: " << composite->getName();
          log( logMsg, DATA_WSDL );

          return;
        }

        std::list<std::string>::const_iterator nsIter    = namespaceList.begin();
        std::list<std::string>::const_iterator nsIterEnd = namespaceList.end();

        // Iterate through the composite namespaces
        for( ; nsIter != nsIterEnd; ++nsIter )
        {
          tuscany::sca::model::WSDLDefinition *wsdlDef = composite->findWSDLDefinition( *nsIter );
          if( 0 == wsdlDef )
          {
            logMsg << "Skipping NULL WSDLDefinition for WSDL namespace: " << *nsIter;
            log( logMsg, DATA_WSDL );

            continue;
          }

          logMsg << "\t WSDL namespace: " << *nsIter;
          log( logMsg, DATA_WSDL );

          std::list<std::string> ptNameList = wsdlDef->getPortTypes();
          if( ptNameList.empty() )
          {
            log( "Skipping WSDLDefinition with no PortTypes defined", DATA_WSDL );

            continue;
          }

          std::list<std::string>::const_iterator ptIter    = ptNameList.begin();
          std::list<std::string>::const_iterator ptIterEnd = ptNameList.end();

          // Iterate through the PortTypes in the WSDLDefinition
          for( ; ptIter != ptIterEnd; ++ptIter )
          {
            logMsg << "\t\t PortType: " << *ptIter;
            log( logMsg, DATA_WSDL );

            std::list<std::string> operNameList = wsdlDef->getOperations( *ptIter );
            if( operNameList.empty() )
            {
              log( "Skipping WSDL PortType with no operations defined", DATA_WSDL );

              continue;
            }

            std::list<std::string>::const_iterator operNameIter    = operNameList.begin();
            std::list<std::string>::const_iterator operNameIterEnd = operNameList.end();

            // Iterate through the Operations in the PortType
            for( ; operNameIter != operNameIterEnd; ++operNameIter )
            {
              const tuscany::sca::model::WSDLOperation &wsdlOper =
                wsdlDef->findOperation( *ptIter, *operNameIter );

              logMsg
                << "\t\t\t Operation Info: "
                << "\n\t\t\t\t OperationName:     " << wsdlOper.getOperationName()
                << "\n\t\t\t\t SOAP Action:       " << wsdlOper.getSoapAction()
                << "\n\t\t\t\t Endpoint:          " << wsdlOper.getEndpoint()
                << "\n\t\t\t\t SOAP version:      " << wsdlOper.getSoapVersion()
                << "\n\t\t\t\t Document Style:    " << wsdlOper.isDocumentStyle()
                << "\n\t\t\t\t Wrapped Style:     " << wsdlOper.isWrappedStyle()
                << "\n\t\t\t\t In Encoded Style:  " << wsdlOper.isInputEncoded()
                << "\n\t\t\t\t Out Encoded Style: " << wsdlOper.isOutputEncoded()
                << "\n\t\t\t\t InputMsgURI:       " << wsdlOper.getInputMessageUri()
                << "\n\t\t\t\t InputMsgName:      " << wsdlOper.getInputMessageName()
                << "\n\t\t\t\t OutputMsgURI:      " << wsdlOper.getOutputMessageUri()
                << "\n\t\t\t\t OutputMsgName:     " << wsdlOper.getOutputMessageName();
              log( logMsg, DATA_WSDL );

              std::list<std::string> partList = wsdlOper.getInputMessagePartNames();
              std::list<std::string>::const_iterator partListIter = partList.begin();
              std::list<std::string>::const_iterator partListIterEnd = partList.end();
              for( ; partListIter != partListIterEnd; ++partListIter )
              {
                tuscany::sca::model::WSDLMessagePart part =
                  wsdlOper.getInputMessagePart( *partListIter );
                logMsg
                  << "\t\t\t\t Input Message Part: "
                  << "\n\t\t\t\t\t Name: " << part.getPartName()
                  << "\n\t\t\t\t\t Type: " << part.getPartType()
                  << "\n\t\t\t\t\t URI:  " << part.getPartUri();
                log( logMsg, DATA_WSDL );
              }

              partList = wsdlOper.getOutputMessagePartNames();
              partListIter = partList.begin();
              partListIterEnd = partList.end();
              for( ; partListIter != partListIterEnd; ++partListIter )
              {
                tuscany::sca::model::WSDLMessagePart part =
                  wsdlOper.getOutputMessagePart( *partListIter );
                logMsg
                  << "\t\t\t\t Output Message Part: "
                  << "\n\t\t\t\t\t Name: " << part.getPartName()
                  << "\n\t\t\t\t\t Type: " << part.getPartType()
                  << "\n\t\t\t\t\t URI:  " << part.getPartUri();
                log( logMsg, DATA_WSDL );
              }
            }
          }
        }
      }

      // private
      void TuscanyServiceLoader::getComponentsFromComposite( tuscany::sca::model::Composite *composite )
      {
        std::stringstream logMsg;

        std::list<std::string> topLevelComponentList = composite->getComponents();
        if( topLevelComponentList.empty() )
        {
          logMsg << "Top level Composite has no Components to process: " << composite->getName();
          log( logMsg, DATA_MODEL );

          return;
        }

        std::list<std::string>::const_iterator tlclIter    = topLevelComponentList.begin();
        std::list<std::string>::const_iterator tlclIterEnd = topLevelComponentList.end();

        // Iterate through the top level composite components
        for( ; tlclIter != tlclIterEnd; ++tlclIter )
        {
          tuscany::sca::model::Component *topLevelComponent = composite->findComponent( *tlclIter );
          if( 0 == topLevelComponent )
          {
            logMsg << "Skipping NULL Component: " << *tlclIter;
            log( logMsg, DATA_MODEL );

            continue;
          }

          logMsg << "\t Top Level Component: " << topLevelComponent->getName();
          log( logMsg, DATA_MODEL );

          // Top level composites will just include lower composites
          // Get the actual implementation composite
          tuscany::sca::model::Composite *impComposite =
            (tuscany::sca::model::Composite*)topLevelComponent->getType();
          if( 0 == impComposite )
          {
            logMsg << "Couldn't get implemetation composite for top level component: " << *tlclIter;
            log( logMsg, DATA_MODEL );

            continue;
          }

          logMsg << "\t\t Implemetation Composite: " << topLevelComponent->getName();
          log( logMsg, DATA_MODEL );

          std::list<std::string> componentList = impComposite->getComponents();
          if( componentList.empty() )
          {
            log( "\t\t Implemetation Composite has no components", DATA_MODEL );

            continue;
          }

          std::list<std::string>::const_iterator clIter    = componentList.begin();
          std::list<std::string>::const_iterator clIterEnd = componentList.end();

          // Iterate through the components
          for( ; clIter != clIterEnd; ++clIter )
          {

            tuscany::sca::model::Component *component = impComposite->findComponent( *clIter );
            if( 0 == component )
            {
              logMsg << "Skipping NULL Component: " << *clIter;
              log( logMsg, DATA_MODEL );

              continue;
            }

            std::string bindingType;
            std::string bindingURI;
            std::string componentType = "\t\t\t Component: ";

            tuscany::sca::model::CompositeService *compositeService =
              dynamic_cast<tuscany::sca::model::CompositeService*>(component);
            if (compositeService)
            {
              componentType = "\t\t\t Service: ";
              tuscany::sca::model::Reference *reference = compositeService->getReference();
              bindingType = reference->getBinding()->getType();
              bindingURI = reference->getBinding()->getURI();
            }
            else
            {
              tuscany::sca::model::CompositeReference *compositeReference =
                dynamic_cast<tuscany::sca::model::CompositeReference*>(component);
              if (compositeReference)
              {
                componentType = "\t\t\t Reference: ";
                tuscany::sca::model::Service *service = compositeReference->getService();
                bindingType = service->getBinding()->getType();
                bindingURI = service->getBinding()->getURI();

              }
            }

            logMsg << componentType << component->getName();
            log( logMsg, DATA_MODEL );
            logMsg << "\t\t\t\t Component Type: " << component->getType()->getName();
            log( logMsg, DATA_MODEL );

            if (!bindingType.empty())
            {
              logMsg << "\t\t\t\t Binding Type: " << bindingType;
              log( logMsg, DATA_MODEL );
            }
            if (!bindingURI.empty())
            {
              logMsg << "\t\t\t\t Binding URI: " << bindingURI;
              log( logMsg, DATA_MODEL );
            }

            const tuscany::sca::model::Component::SERVICE_MAP &serviceMap = component->getServices();
            if( serviceMap.empty() )
            {

              continue;
            }

            tuscany::sca::model::Component::SERVICE_MAP::const_iterator smIter = serviceMap.begin();
            tuscany::sca::model::Component::SERVICE_MAP::const_iterator smIterEnd = serviceMap.end();

            for( ; smIter != smIterEnd; ++smIter )
            {
              const tuscany::sca::model::Service *service = smIter->second;
  
              if( 0 == service )
              {
                logMsg << "Skipping NULL Service: " << smIter->second;
                log( logMsg, DATA_MODEL );
  
                continue;
              }
  
              // A ServiceType defines the characteristics of the Service
              tuscany::sca::model::ServiceType *serviceType = service->getType();
              logMsg << "\t\t\t\t Service: " << serviceType->getName();
              log( logMsg, DATA_MODEL );
            }
            
            const tuscany::sca::model::Component::REFERENCE_MAP &referenceMap = component->getReferences();
            if( referenceMap.empty() )
            {
              continue;
            }
  
            tuscany::sca::model::Component::REFERENCE_MAP::const_iterator rmIter = referenceMap.begin();
            tuscany::sca::model::Component::REFERENCE_MAP::const_iterator rmIterEnd = referenceMap.end();
  
            for( ; rmIter != rmIterEnd; ++rmIter )
            {
              const tuscany::sca::model::Reference *reference = rmIter->second;
  
              if( 0 == reference )
              {
                logMsg << "Skipping NULL Reference: " << rmIter->second;
                log( logMsg, DATA_MODEL );
  
                continue;
              }
  
              // A ReferenceType defines the characteristics of the Service
              logMsg << "\t\t\t\t Reference: " << reference->getType()->getName();
              log( logMsg, DATA_MODEL );

            }
          }
        }
      }

      // private
      void TuscanyServiceLoader::log( std::stringstream &msg, TuscanyServiceLoader::DataType type )
      {
        log( msg.str(), type );
        msg.str( "" ); // clear it to be able to use the same one again
      }

      // private
      void TuscanyServiceLoader::log( const std::string &msg, TuscanyServiceLoader::DataType type )
      {
        bool doLog = false;

        if( type == DATA_BOTH )
        {
          doLog = true;
        } 
        else if( type == DATA_BOTH && (showModel_ || showWsdl_) )
        {
          doLog = true;
        }
        else if( type == DATA_WSDL && showWsdl_ )
        {
          doLog = true;
        }
        else if( type == DATA_MODEL && showModel_ )
        {
          doLog = true;
        }

        if( doLog )
        {
          std::cout << msg << std::endl;
        }

      }

    } // namespace toys
  } // namespace sca
} // namespace tuscany
