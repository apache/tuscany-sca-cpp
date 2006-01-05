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

#ifndef osoa_sca_serviceruntimeexception_h
#define osoa_sca_serviceruntimeexception_h

#include "osoa/sca/export.h"

#include <ostream>

namespace osoa
{
    namespace sca
    {
        
        /**
         * Top level exception to represent all the exceptions that may be 
         * thrown by an SCA runtime implementation.
         */
        class SCA_API ServiceRuntimeException
        {
        public:
            /**
             * Represents the possible severity levels for an exception.
             */
            enum severity_level
            {
                Normal,    
                    Warning,
                    Error,
                    Severe
            };
            
            /**
             * Constructor.
             * @param name Class name of the exception.
             * @param sev Severity level.
             * @param msg_text Detailed description of the exception.
             */
            ServiceRuntimeException(
                const char *name="ServiceRuntimeException",
                severity_level sev=Severe,
                const char* msg_text="");
            
            ServiceRuntimeException(const ServiceRuntimeException& c);
            
            // Destructor
            virtual  ~ServiceRuntimeException();
            
            /**
             * Return class name of this exception.
             */
            const char* getEClassName() const;
            
            /**
             * Return severity.
             */
            severity_level getSeverity() const;
            
            /**
             * Return message text associated with exception.
             */
            const char* getMessageText() const;
            
            /*
             * Return file name where the exception was raised.
             */
            const char* getFileName() const;
            
            /**
             * Return line number where the exception was raised.
             */
            unsigned long getLineNumber() const;
            
            /**
             * Return function name where the exception was raised.
             */
            const char* getFunctionName() const;
            
            /**
             * Set the exception severity.
             */
            void setSeverity(severity_level sev);
            
            /**
             * Set the message text associated with exception.
             */
            void setMessageText(const char* msg_text);
                    
            /**
             * Set the location where the exception was raised.
             * @param file Name of the file.
             * @param line Line number in the file.
             * @param function Name of the function.
             */
            void setLocation(const char* file,
                unsigned long line,
                const char* function="");
            
            /**
             * Append exception details to ostream.
             */
            virtual std::ostream& PrintSelf(std::ostream &os) const;
            
            /**
             * Operator to send exceptions details to a stream.
             */
            SCA_API friend std::ostream& operator<< (std::ostream &os, const ServiceRuntimeException &except);
        protected:
            
        private:
            /**
             * Class name of the exception.
             */ 
            char*        class_name;
            
            /**
             * Severity level of the exception.
             */
            severity_level   severity;
            
            /**
             * Description of the exception.
             */
            char*        message_text;     // Description of exception
            
            /**
             * Location where the exception was thrown or handled and thrown.
             */            
            class location
            {
            public:
                char*            file;       // File name (from __FILE__)
                unsigned long   line;       // Line number (from __LINE__)
                char*            function;   // Function name
            };
            
            
            enum {num_locations=5};
            /**
             * Array of locations where the exception has been handled and thrown.
             */
            location         locations[num_locations];
            
            /**
             * The current location (index into ServiceRuntimeException#location).
             */
            int              location_set;
            
            
        }; // End ServiceRuntimeException class definition
        

        /**
         * A remotable service is currently unavailable. It is possible that a retry
         * may resolve this exception.
         */
        class SCA_API ServiceUnavailableException: public ServiceRuntimeException
        {
        public:
            ServiceUnavailableException(const char* serviceName)
                : ServiceRuntimeException("ServiceUnavailableException", Warning,
                serviceName)
            {
            }
        private:
        }; // End ServiceUnavailableException class definition


        /**
         * The target of a wire cannot be found, or the reference has not been
         * configured.
         */
        class ServiceNotFoundException: public ServiceRuntimeException
        {
        public:
            ServiceNotFoundException(const char* msg)
                : ServiceRuntimeException("ServiceNotFoundException", Error,
                msg)
            {
            }
        private:
        }; // End ServiceNotFoundException class definition


        /**
         * There is no current component (for example, if a non-SCA component
         * tries to get the current ComponentContext).
         */
        class ComponentContextException: public ServiceRuntimeException
        {
        public:
            ComponentContextException(const char* msg)
                : ServiceRuntimeException("ComponentContextException", Error,
                msg)
            {
            }
        private:
        }; // End ComponentContextException class definition

        /**
         * Unable to find the specified entry point in the module.
         */
        class EntryPointNotFoundException: public ServiceRuntimeException
        {
        public:
            EntryPointNotFoundException(const char* msg)
                : ServiceRuntimeException("EntryPointNotFoundException", Error,
                msg)
            {
            }
        private:
        }; // End EntryPointNotFoundException class definition

        
        
    } // End namespace sca
} // End namespace osoa

#endif // osoa_sca_serviceruntimeexception_h
