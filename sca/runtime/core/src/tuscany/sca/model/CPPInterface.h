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

/* $Rev$ $Date: 2005/12/22 11:33:21 $ */

#ifndef tuscany_sca_model_cppinterface_h
#define tuscany_sca_model_cppinterface_h

#include "tuscany/sca/model/Interface.h"


#include <map>
using std::map;
#include <string>
using std::string;

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            /**
             * Holds information about an interface described using a C++
             * header file.
             */
            class CPPInterface : public Interface
            {
                
            public:    
                /**
                 * Constuctor.
                 * @param header Name of the header file containing the class that
                 * describes the interface.
                 * @param className Name of the class in the header file that 
                 * describes the interface. 
                 * @param scope The scope of the interface (stateless or composite).
                 * @param remotable True if the interface is remotable.
                 */
                CPPInterface(
                    const string& header,
                    const string& className,
                    const string& scope,
                    bool remotable);  

                /**
                 * Destructor.
                 */
                virtual ~CPPInterface();
                
                /**
                 * Return the type of the interface.
                 * @return Always returns CPP.
                 */
                virtual Type getInterfaceType() {return CPP;}

                /**
                 * Get the name of the header file.
                 * @return The name of the header file containing the definition of the
                 * interface.
                 */
                const string& getHeader() {return header;}

                /**
                 * Return the name of the header file without the extension.
                 * @return Header file name without any extension.
                 */
                const string& getHeaderStub() {return headerStub;}

                /**
                 * Get the name of the class.
                 * @return The name of the class defining the interface.
                 */
                const string& getClass() {return className;}

                /**
                 * Scope of interface.
                 */
                enum SCOPE
                {
                    COMPOSITE,
                    STATELESS
                };

                /**
                 * Get the scope of the interface.
                 * @return The scope of the interface.
                 */
                 SCOPE getScope() {return scope;}

                /**
                 * Return whether the interface is remotable or local.
                 * @return True if the interface is remotable, otherwise false.
                 */
                bool  getRemotable() {return remotable;}

           private:
                /**
                 * Name of the header file containing the definition of the interface.
                 */
                string header;

                /**
                 * Name of the header file without the extension.
                 */
                string headerStub;

                /**
                 * Name of the class in the header file.
                 */
                string className;

                /**
                 * Scope of the interface.
                 */
                SCOPE scope;

                /**
                 * Remotable interface or not.
                 */
                bool remotable;
            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_cppinterface_h

