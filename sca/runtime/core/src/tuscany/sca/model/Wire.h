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

#ifndef tuscany_sca_model_wire_h
#define tuscany_sca_model_wire_h

#include <string>
using std::string;

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            /**
             * Information about a wire in the model.
             */
            class Wire
            {
            public: 
                /**
                 * Constructor.
                 * @param source The source of the wire. Either the component and 
                 * reference name (optional) or an entry point.
                 * @param target The target of the wire. Either a component and service
                 * service name (optional) or an external sevice.
                 */
                Wire(const std::string& source, const std::string& target);

                /**
                 * Destructor.
                 */
                virtual ~Wire();

                /**
                 * Get the component name defined by the source of the wire.
                 * @return The component name which is the source of the wire.
                 */
                const std::string& getSourceComponent() {return sourceComponent;}

                /**
                 * Get the reference name defined by the source of the wire.
                 * @return The reference name which is the source of the wire.
                 */
                const std::string& getSourceReference() {return sourceReference;}

                /**
                 * Get the target uri defined by the target of the wire.
                 * @return The target uri which is the source of the wire.
                 */
                const std::string& getTarget() {return target;}
             private:
                /**
                 * The source component of the wire.
                 */
                string sourceComponent;

                /**
                 * The source reference of the wire.
                 */
                string sourceReference;

                /**
                 * The target uri of the wire.
                 */
                 string target;
            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_wire_h

