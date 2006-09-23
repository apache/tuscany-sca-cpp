/*
 *
 *  Copyright 2005 The Apache Software Foundation or its licensors, as applicable.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/* $Rev$ $Date$ */

#ifndef _SDO_AXIOM_H_
#define _SDO_AXIOM_H_

#include "sdo_axiom_export.h"

#include "axiom.h"
//#include "axis2_om_document.h"
//#include "axis2_om_stax_builder.h"


#include "commonj/sdo/SDO.h"



namespace commonj
{
    using namespace sdo;
    namespace sdo_axiom
    {
            
/**
 * AxiomHelper writes a data object tree to XML
 */
        class AxiomHelper
        {
            
        public:
            
            SDO_AXIOM_API AxiomHelper();
            
            SDO_AXIOM_API virtual ~AxiomHelper();

            SDO_AXIOM_API static AxiomHelper* getHelper();

            SDO_AXIOM_API static void releaseHelper(AxiomHelper* ax);

            SDO_AXIOM_API axiom_document_t*  toAxiomDoc(DataObjectPtr dob,
                const char* targetNamespaceURI = "", const char* elementName = "");
            SDO_AXIOM_API axiom_node_t*      toAxiomNode(DataObjectPtr dob,
                const char* targetNamespaceURI = "", const char* elementName = "");

            SDO_AXIOM_API DataObjectPtr  toSdo(axiom_document_t* doc, DataFactoryPtr factory, const char* targetNamespaceURI=0);
            SDO_AXIOM_API DataObjectPtr  toSdo(axiom_node_t* root_node, DataFactoryPtr factory, const char* targetNamespaceURI=0);

            SDO_AXIOM_API axis2_env_t* getEnv();

            SDO_AXIOM_API void output(axiom_document_t* document);


        protected:
        private:

            void deleteEnv();

            void createEnv();

            static int axiswritercount;

            axis2_env_t *the_env;
            axis2_allocator_t *the_allocator;

            

        };
    } // End - namespace sdo_axiom
} // End - namespace commonj


#endif //_SDO_AXIOM_H_
