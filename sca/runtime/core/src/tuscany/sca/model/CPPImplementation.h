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

#ifndef tuscany_sca_model_cppimplementation_h
#define tuscany_sca_model_cppimplementation_h
#include "tuscany/sca/model/Implementation.h"

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
             * Holds informatio about an SCA implementation written in C++
             */
            class CPPImplementation : public Implementation
            {
                
            public:        
                /**
                 * Constructor.
                 * @param dllName Name of the shared library.
                 * @param header Name of the header file that contains the class declaring the 
                 * implementation class.
                 * @param className Name of the class in the header file (could be a blank string
                 * if this is not specified).
                 */
                CPPImplementation(const string& dllName, const string& header, const string& className);
                
                /**
                 * Destructor
                 */
                virtual ~CPPImplementation();
                            
                /**
                 * Return the implementation type.
                 * @return Always returns CPP.
                 */
                virtual const std::string getImplementationType() {return "cpp";}

                virtual std::string getComponentTypeFileName() {return getHeaderPath() + getHeaderStub();}   
                /**
                 * Returns the name of the shared library.
                 * @return The name of the shared library.
                 */
                const string& getLibrary() {return library;}

                /**
                 * Get the name of the header file.
                 * @return Name of the header file.
                 */
                const string& getHeader() {return header;}

                /**
                 * Get the header file name without the extension.
                 * @return The name of the header file without any extension.
                 */
                const string& getHeaderStub() {return headerStub;}

                /**
                 * Get the header path.
                 * @return The pathe element of the header.
                 */
                const string& getHeaderPath() {return headerPath;}

                /**
                 * Get the name of the class.
                 * @return The class name if specified.
                 */
                const string& getClass() {return className;}
            private:
                /**
                 * Name of the shared library.
                 */
                string library;

                /**
                 * Name of the header file describing the interface.
                 */
                string header;

                /**
                 * Name of the header file without the extension.
                 */
                string headerStub;

                /**
                 * Path element of the header.
                 */
                string headerPath;

                /**
                 * Name of the class in the header file declaring the implementation.
                 * May be an empty string if not set in the SCDL file.
                 */
                string className;
            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_cppimplementation_h

