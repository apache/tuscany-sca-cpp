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

#include "sdo_axiom.h"

using namespace commonj::sdo;


namespace commonj
{
    namespace sdo_axiom
    {

        int AxiomHelper::axiswritercount = 0;

        AxiomHelper* AxiomHelper::getHelper()
        {
            return new AxiomHelper();
        }

        void AxiomHelper::releaseHelper(AxiomHelper* h)
        {
            if (h) delete h;
        }

        void AxiomHelper::deleteEnv()
        {
            if (the_env)axis2_env_free(the_env);
        }

        void AxiomHelper::createEnv()
        {
    
            the_allocator = axis2_allocator_init (NULL);
            if (the_allocator == NULL) return;

            the_env = axis2_env_create(the_allocator); 

            if (the_env == 0) return;

            return;
        }


        AxiomHelper::AxiomHelper()
        {
            createEnv();
        }

        AxiomHelper::~AxiomHelper()
        {
            deleteEnv();
        }

        axis2_om_document_t* AxiomHelper::toAxiom(DataObjectPtr dob)
        {

            DataFactory* df = dob->getDataFactory();
            XSDHelperPtr xs = HelperProvider::getXSDHelper(df);
            XMLHelperPtr xm = HelperProvider::getXMLHelper(df);
            if (!the_env)
            {
                cout << "No Axis Environment" << endl;
                return 0;
            }
           char * str = 
                xm->save(
                dob,
                dob->getType().getURI(),
                dob->getType().getName());

            // if (str) cout << str << endl;

            axis2_xml_reader_t * reader =  
            axis2_xml_reader_create_for_buffer(&the_env,
                                  (const axis2_char_t *)str,
                                  strlen(str),
                                  (const axis2_char_t *)"UTF-8");
        
            if (!reader)
            {
                cout << "No Axis Reader" << endl;
                return 0;
            }

            axis2_om_stax_builder_t* builder = 
                axis2_om_stax_builder_create(&the_env, reader);
        
            if (!builder)
            {
                cout << "No Axis Builder" << endl;
                AXIS2_XML_READER_FREE(reader, &the_env);
                return 0;
            }
        
            axis2_om_document_t* document = 
                AXIS2_OM_STAX_BUILDER_GET_DOCUMENT(builder, &the_env);
        
            if (!document)
            {
                cout << "No Axis Document" << endl;
                AXIS2_OM_STAX_BUILDER_FREE(builder, &the_env);
                return 0;
            }
    
            axis2_om_node_t* root_node = 
                AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT(document, &the_env);
            if (!root_node)
            {
                cout << "No Root Element in the document" << endl;
                AXIS2_OM_STAX_BUILDER_FREE(builder, &the_env);
                return 0;
            }
            

            AXIS2_OM_DOCUMENT_BUILD_ALL(document, &the_env);

            return document;
        }


        void AxiomHelper::output(axis2_om_document_t* document)
        {
        
            if (!the_env)
            {
                cout << "No Axis Environment" << endl;
                return;
            }

            axis2_xml_writer_t* writer = axis2_xml_writer_create_for_memory(
                &the_env, NULL, AXIS2_TRUE, 0);
    
            axis2_om_output_t* output = axis2_om_output_create(&the_env, writer);
    
            axis2_om_node_t* root_node = 
                AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT(document, &the_env);
 
            if (!root_node)
            {
                cout << "No Root Element in the document" << endl;
                AXIS2_OM_OUTPUT_FREE(output, &the_env);
                return;
            }

            AXIS2_OM_NODE_SERIALIZE(root_node, &the_env, output);
    
            axis2_char_t* buffer = AXIS2_XML_WRITER_GET_XML(writer, &the_env);

            printf("Output XML:n %s ", buffer);

            AXIS2_OM_OUTPUT_FREE(output, &the_env);
       
            return;          
        }
    }
}

