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

#include "commonj/sdo/SAX2Namespaces.h"


namespace commonj
{
    namespace sdo
    {
        
        SAX2Namespaces::SAX2Namespaces()
        {
        }
        
        SAX2Namespaces::SAX2Namespaces(int nb_namespaces, const xmlChar** namespaces)
        {
            for (int i=0; i<nb_namespaces*2; i+=2)
            {
                namespaceMap[namespaces[i]] = namespaces[i+1];        
            }
        }
        
        SAX2Namespaces::~SAX2Namespaces()
        {    
        }
        
        const SDOXMLString* SAX2Namespaces::find(const SDOXMLString& prefix) const
        {    
            NAMESPACE_MAP::const_iterator nsIter;
            nsIter = namespaceMap.find(prefix);
            if(nsIter == namespaceMap.end())
            {
                return 0;
            }
            
            return &(nsIter->second);
            
        }        

        const SDOXMLString* SAX2Namespaces::findPrefix(const SDOXMLString& uri) const
        {    
            NAMESPACE_MAP::const_iterator nsIter;
            for (nsIter = namespaceMap.begin(); nsIter != namespaceMap.end(); nsIter++)
            {
                if (nsIter->second.equals(uri))
                    return &(nsIter->first);
            }
            return 0;
        }        

        void SAX2Namespaces::merge(const SAX2Namespaces& inspaces)
        {
            NAMESPACE_MAP::const_iterator nsIter;
            for (nsIter = inspaces.namespaceMap.begin(); 
                 nsIter != inspaces.namespaceMap.end(); nsIter++)
            {
                // add will not overwrite if it already exists
                namespaceMap[nsIter->first] = nsIter->second;
            }
            return;
        }



        void SAX2Namespaces::add(const SDOXMLString& prefix, const SDOXMLString& uri)
        {
            namespaceMap[prefix] = uri;
        }

        void SAX2Namespaces::empty()
        {
            namespaceMap.empty();
        }

    } // End - namespace sdo
} // End - namespace commonj
