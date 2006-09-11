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

#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/util/Logging.h"
using namespace std;

namespace tuscany
{
    namespace sca
    {
        
        // ========================================================================
        // Constructor
        // ========================================================================
        TuscanyRuntimeException :: TuscanyRuntimeException(const char* name,
            severity_level sev,
            const char* msg_text)
            : severity(sev), location_set(0)
        {
            class_name = new char[strlen(name) + 1];
            strcpy(class_name,name);
            message_text = new char[strlen(msg_text)+1];
            strcpy(message_text,msg_text);
            LOGERROR_2(1, "%s raised: %s", class_name, message_text);
            
        } // end TuscanyRuntimeException constuctor
        
        // ========================================================================
        // Constructor
        // ========================================================================
        TuscanyRuntimeException ::  TuscanyRuntimeException(const TuscanyRuntimeException& c)
            : 
        severity(c.getSeverity()), location_set(c.location_set)
            
        {
            class_name = new char[strlen(c.getEClassName()) + 1];
            strcpy(class_name, c.getEClassName());
            message_text = new char[strlen(c.getMessageText())+1];
            strcpy(message_text,c.getMessageText());
            for (int i=0;i<c.location_set;i++)
            {
                locations[i].file = new char[strlen(c.locations[i].file) + 1];
                strcpy(locations[i].file,c.locations[i].file);
                locations[i].line = c.locations[i].line;
                locations[i].function = new char[strlen(c.locations[i].function) + 1];
                strcpy(locations[i].function, c.locations[i].function);
            }
            LOGERROR_2(1, "%s raised: %s", class_name, message_text);
        }
        
        // ========================================================================
        // Destructor
        // ========================================================================
        TuscanyRuntimeException :: ~TuscanyRuntimeException()
        {
            if (class_name) delete class_name;
            if (message_text) delete message_text;
            for (int i=0;i<location_set;i++)
            {
                if (locations[i].file) delete locations[i].file;
                if (locations[i].function) delete locations[i].function;
            }
            
        } // end TuscanyRuntimeException destructor
        
        // ========================================================================
        // Return class name of this exception
        // ========================================================================
        const char* TuscanyRuntimeException :: getEClassName() const
        {
            return class_name;
        } // end getClassName()
        
        // ========================================================================
        // Return severity
        // ========================================================================
        TuscanyRuntimeException::severity_level TuscanyRuntimeException :: getSeverity() const
        {
            return severity;
        } // end getSeverity()
        
        // ========================================================================
        // Return message text associated with exception
        // ========================================================================
        const char* TuscanyRuntimeException :: getMessageText() const
        {
            return message_text;
        } // end getMessageText()
        
        // ========================================================================
        // Return file name where exception was raised
        // ========================================================================
        const char* TuscanyRuntimeException :: getFileName() const
        {
            return locations[0].file;
        } // end getFileName()
        
        // ========================================================================
        // Return line number where exception was raised
        // ========================================================================
        unsigned long TuscanyRuntimeException :: getLineNumber() const
        {
            return locations[0].line;
        } // end getLineNumber()
        
        // ========================================================================
        // Return function name where exception was raised
        // ========================================================================
        const char* TuscanyRuntimeException :: getFunctionName() const
        {
            return locations[0].function;
        } // end getFunctionName()
        
        
        // ========================================================================
        // set severity of exception
        // ========================================================================
        void TuscanyRuntimeException :: setSeverity(severity_level sev)
        {
            severity = sev;
        } // end setSeverity(severity_level sev) const
        
        // ========================================================================
        // set message text associated with exception
        // ========================================================================
        void TuscanyRuntimeException :: setMessageText(const char* msg_text)
        {
            if (message_text != 0) delete message_text;
            message_text = new char[strlen(msg_text) + 1];
            strcpy(message_text,msg_text);
        } // end setMessageText(const string &msg_text) const
        
        // ========================================================================
        // set location of most recent throw/handling of the exception
        // ========================================================================
        void TuscanyRuntimeException :: setLocation(const char* file,    
            unsigned long line,       
            const char* function)
        {
            if (location_set < num_locations)
            {
                locations[location_set].file = new char[strlen(file) + 1];
                strcpy(locations[location_set].file,file);
                locations[location_set].line = line;
                locations[location_set].function = new char[strlen(function) + 1];
                strcpy(locations[location_set].function,function);
                
                location_set++;
            }
        } // end setLocation()
        
        
        // ========================================================================
        // print self
        // ========================================================================
        ostream& TuscanyRuntimeException :: PrintSelf(ostream &os) const
        { 
            
            os << "Exception object :" << endl;
            os << " class:           " << class_name << endl;
            os << " description:     " << message_text << endl;
            if (location_set != 0)
            {
                os << " file name:       " << locations[0].file << endl;
                char lineNumber[100];
                sprintf(lineNumber, "%lu",locations[0].line);
                os << " line number:     " << lineNumber << endl;
                os << " function:        " << locations[0].function << endl;
                os << " location history:" << endl;
                
                int i=1;
                while (i < location_set)
                {
                    os << "  " <<  i << ")" << endl;
                    os << "   file:          " << locations[i].file << endl;
                    os << "   line:          " << locations[i].line << endl;
                    os << "   function:      " << locations[i].function << endl;
                    i++;
                }
            }
            return os;
        } // end ostream operator <<
        
        // ========================================================================
        // ostream operator <<
        // ========================================================================
        SCA_API ostream& operator<< (ostream &os, const TuscanyRuntimeException &except)
        {
            return except.PrintSelf(os);
        } // end ostream operator <<
        
        
    } // End namespace sca
} // End namespace tuscany
