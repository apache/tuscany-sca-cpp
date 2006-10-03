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

#ifndef tuscany_sca_util_exceptions_h
#define tuscany_sca_util_exceptions_h


#include "tuscany/sca/export.h"

#include <ostream>


namespace tuscany
{
    namespace sca
    {        
        /**
         * Top level exception to represent all the exceptions that may be 
         * thrown by an SCA runtime implementation.
         */
        class SCA_API TuscanyRuntimeException
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
            TuscanyRuntimeException(
                const char *name="TuscanyRuntimeException",
                severity_level sev=Severe,
                const char* msg_text="");
            
            TuscanyRuntimeException(const TuscanyRuntimeException& c);
            
            // Destructor
            virtual  ~TuscanyRuntimeException();
            
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
            SCA_API friend std::ostream& operator<< (std::ostream &os, const TuscanyRuntimeException &except);
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
             * The current location (index into TuscanyRuntimeException#location).
             */
            int              location_set;
            
            
        }; // End TuscanyRuntimeException class definition
        

        /**
         * Indicates a problem in the consistency of the SCA model provided to the
         * Tuscany runtime.
         */
        class SCA_API SystemConfigurationException: public TuscanyRuntimeException
        {
        public:
            SystemConfigurationException(const char* msg)
                : TuscanyRuntimeException("SystemConfigurationException", Severe,
                msg)
            {
            }
        private:
        };
        
    } // End namespace sca
} // End namespace tuscany
#endif // tuscany_sca_util_exceptions_h
