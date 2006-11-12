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
#include "commonj/sdo/SDO.h"


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
                const char *name = "TuscanyRuntimeException",
                severity_level sev = Severe,
                const char* msg_text = "");
            
            TuscanyRuntimeException(const TuscanyRuntimeException& c);
            TuscanyRuntimeException(const SDORuntimeException& c);
            
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
            
            /**
             * A snapshot of the stack when the exception was constructed
             */
#if defined(WIN32)  || defined (_WINDOWS)
#else
            int stacktrace_size;
            char** stacktrace_symbols;
#endif
            
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

            SystemConfigurationException(
                const char *name,
                severity_level sev,
                const char* msg_text)
                : TuscanyRuntimeException(name, sev, msg_text)
            {
            }
            
            SystemConfigurationException(const SDORuntimeException& c)
                : TuscanyRuntimeException(c)
            {
            }
        private:
        };
        
        /**
         * Indicates a problem while invoking a service.
         */
        class SCA_API ServiceInvocationException: public TuscanyRuntimeException
        {
        public:
            ServiceInvocationException(const char* msg)
                : TuscanyRuntimeException("ServiceInvocationException", Severe, msg)
            {
            }

            ServiceInvocationException(
                const char *name,
                severity_level sev,
                const char* msg_text)
                : TuscanyRuntimeException(name, sev, msg_text)
            {
            }
            
            ServiceInvocationException(const SDORuntimeException& c)
                : TuscanyRuntimeException(c)
            {
            }

        private:
        };
        
        /**
         * Indicates a problem while working with service data.
         */
        class SCA_API ServiceDataException: public TuscanyRuntimeException
        {
        public:
            ServiceDataException(const char* msg)
                : TuscanyRuntimeException("ServiceDataException", Severe,
                msg)
            {
            }

            ServiceDataException(
                const char *name,
                severity_level sev,
                const char* msg_text)
                : TuscanyRuntimeException(name, sev, msg_text)
            {
            }
            
            ServiceDataException(const SDORuntimeException& c)
                : TuscanyRuntimeException(c)
            {
            }
        private:
        };
        
    } // End namespace sca
} // End namespace tuscany


/**
  * =========================================================================
  * Macro  - throwException
  *
  * adds the current file name, line number and function name to the exception.
  * then throws the exception.
  * The parameter 'function_name' should be the name of the function throwing
  * this exception.
  * The parameter 'type' is the class of exception to throw and must be a
  * SDORuntimeException or a class derived from SDORuntimeException.
  * The parameter 'parameter' is the construction parameter for the exception
  * =========================================================================
*/

#if defined(WIN32)  || defined (_WINDOWS)
#define throwException(type, parameter)  \
{\
 type __TuscanyThrownException__(parameter); \
 __TuscanyThrownException__.setLocation(__FILE__,__LINE__,__FUNCTION__); \
 throw __TuscanyThrownException__;\
}
#else
#define throwException(type, parameter)  \
{\
 type __TuscanyThrownException__(parameter); \
 __TuscanyThrownException__.setLocation(__FILE__,__LINE__,__PRETTY_FUNCTION__); \
 throw __TuscanyThrownException__;\
}
#endif

/** 
    =========================================================================
  * Macro  - rethrowException
  *
  * adds the current file name, line number and function name to the exception.
  * then re-throws the exception.
  * The parameter 'function_name' should be the name of the function throwing
  * this exception.
  * =========================================================================
*/
#if defined(WIN32)  || defined (_WINDOWS)
#define rethrowException(exception)  \
{\
 (exception).setLocation(__FILE__,__LINE__,__FUNCTION__); \
 throw (exception);\
}
#else
#define rethrowException(exception)  \
{\
 (exception).setLocation(__FILE__,__LINE__,__PRETTY_FUNCTION__); \
 throw (exception);\
}
#endif

/**
  * =========================================================================
  * Macro  - handleException
  *
  * adds the current file name, line number and function name to the exception.
  * Writes an exception trace entry then continues.
  * The parameter 'function_name' should be the name of the function handling
  * this exception.
  * =========================================================================
*/
#if defined(WIN32)  || defined (_WINDOWS)
#define handleException(__PRETTY_FUNCTION__, exception)  \
{\
 (exception).setLocation(__FILE__,__LINE__,__FUNCTION__); \
}
#else
#define handleException(__PRETTY_FUNCTION__, exception)  \
{\
 (exception).setLocation(__FILE__,__LINE__,__PRETTY_FUNCTION__); \
}
#endif

#endif // tuscany_sca_util_exceptions_h
