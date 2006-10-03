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

#include "commonj/sdo/SDORuntimeException.h"

#include <stdio.h>
              
// ========================================================================
// Constructor
// ========================================================================
SDORuntimeException :: SDORuntimeException(const char* name,
                             severity_level sev,
                             const char* msg_text)
              : severity(sev),location_set(0)
{
    class_name = new char[strlen(name) + 1];
    strcpy(class_name,name);
    message_text = new char[strlen(msg_text)+1];
    strcpy(message_text,msg_text);

} // end SDORuntimeException constuctor
  
// ========================================================================
// Constructor
// ========================================================================
SDORuntimeException ::  SDORuntimeException(const SDORuntimeException& c)
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
}
           
// ========================================================================
// Destructor
// ========================================================================
SDORuntimeException :: ~SDORuntimeException()
{
    if (class_name) delete class_name;
    if (message_text) delete message_text;
    for (int i=0;i<location_set;i++)
    {
        if (locations[i].file) delete locations[i].file;
        if (locations[i].function) delete locations[i].function;
    }

} // end SDORuntimeException destructor
                    
// ========================================================================
// Return class name of this exception
// ========================================================================
const char* SDORuntimeException :: getEClassName() const
{
  return class_name;
} // end getClassName()

// ========================================================================
// Return severity
// ========================================================================
SDORuntimeException::severity_level SDORuntimeException :: getSeverity() const
{
  return severity;
} // end getSeverity()

// ========================================================================
// Return message text associated with exception
// ========================================================================
const char* SDORuntimeException :: getMessageText() const
{
  return message_text;
} // end getMessageText()

// ========================================================================
// Return file name where exception was raised
// ========================================================================
const char* SDORuntimeException :: getFileName() const
{
  return locations[0].file;
} // end getFileName()

// ========================================================================
// Return line number where exception was raised
// ========================================================================
unsigned long SDORuntimeException :: getLineNumber() const
{
  return locations[0].line;
} // end getLineNumber()

// ========================================================================
// Return function name where exception was raised
// ========================================================================
const char* SDORuntimeException :: getFunctionName() const
{
  return locations[0].function;
} // end getFunctionName()


// ========================================================================
// set severity of exception
// ========================================================================
void SDORuntimeException :: setSeverity(severity_level sev)
{
  severity = sev;
} // end setSeverity(severity_level sev) const

// ========================================================================
// set message text associated with exception
// ========================================================================
void SDORuntimeException :: setMessageText(const char* msg_text)
{
  if (message_text != 0) delete message_text;
  message_text = new char[strlen(msg_text) + 1];
  strcpy(message_text,msg_text);
} // end setMessageText(const string &msg_text) const
                    
// ========================================================================
// set exception location
// ========================================================================
void SDORuntimeException :: setExceptionLocation(const char* file,    
                                            unsigned long line,       
                                            const char* function)
{
  // Only set this information once
  if (location_set == 0)
  {
    locations[0].file = new char[strlen(file)+1];
    strcpy(locations[0].file,file);
    locations[0].line = line;
    locations[0].function = new char[strlen(function) + 1];
    strcpy(locations[0].function,function);
    location_set = 1;
  }
} // end setLocation()

// ========================================================================
// set location of most recent throw/handling of the exception
// ========================================================================
void SDORuntimeException :: setLocation(const char* file,    
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
// Trace the exception  
// ========================================================================
void SDORuntimeException :: trace(const char* text)
{
    /* TODO */
  // TRACE_STREAM(0, SDORuntimeException::trace, " ", (*this));
} // end Trace()

// ========================================================================
// print self
// ========================================================================
std::ostream& SDORuntimeException :: PrintSelf(std::ostream &os) const
{ 
  char lineNumber[100];
  sprintf(lineNumber, "%d",locations[0].line);

  os << "Exception object :" << std::endl;
  os << " class:           " << class_name << std::endl;
  os << " file name:       " << locations[0].file << std::endl;
  os << " line number:     " << lineNumber << std::endl;
  os << " function:        " << locations[0].function << std::endl;
  os << " description:     " << message_text << std::endl;
  os << " location history:" << std::endl;

  // TODO - commented out - a two-location rethrow causes an access
  // violation here.
  //int i=1;
  //while (i < location_set)
  //{
  //  os << "  " <<  i << ")" << std::endl;
  //  os << "   file:          " << locations[i].file << std::endl;
  //  os << "   line:          " << locations[i].line << std::endl;
  //  os << "   function:      " << locations[i].function << std::endl;
  //  i++;
  //}
  return os;
} // end ostream operator <<
    
// ========================================================================
// ostream operator <<
// ========================================================================
SDO_API std::ostream& operator<< (std::ostream &os, const SDORuntimeException &except)
{
  return except.PrintSelf(os);
} // end ostream operator <<
    
