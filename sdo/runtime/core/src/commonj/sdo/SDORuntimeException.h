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

/* $Rev$ $Date: 2005/12/22 16:54:15 $ */

#if !defined SDO_EXCEPTIONS_H
#define SDO_EXCEPTIONS_H

#include "commonj/sdo/export.h"


#include <string>
#include <iostream>



/** SDORuntimeException - Exception base class.
 *
 *
 * Description :
 * -------------
 *   SDO exception class definition
 *
 *   This is intended as a base class.
 *
 * Parent Classes :
 * ----------------
 *   none
 *
 * Public Member functions:
 * ------------------------
 *
 *   Constructors :
 *     SDORuntimeExceptionm(severity_level sev=severe,
 *                          const string &msg_text="");
 *
 *       creates the exception object. Parameters are:
 *         sev: the severity of the exception (Error, Severe)
 *         msg_text:  descriptive message_text
 *
 *     copy constructor not necessary
 *
 *   Destructor :
 *       virtual ~SDORuntimeException();
 *
 *   Assignment operator:
 *       not necessary
 *
 *   void setSeverity(severity_level sev);
 *     set the exception severity
 *
 *   void setMessageText(const char* msg_text);
 *     set the message text associated with the exception. (This text should
 *     come from a message catalog as it may be written to stdout/stderr at
 *     some point)
 *
 *   void setExceptionLocation(const char* file,
 *                             unsigned long line,
 *                             const char* function="");
 *
 *     adds the location of the exception. Parameters
 *       file: the filename from where the exception is being created
 *       (from __FILE__)
 *       line: the line number from where the exception is being created
 *       (from __LINE__)
 *       function: The function name from where the exception is being created
 *
 *   severity_level getSeverity() const;
 *     returns the severity of the exception
 *
 *   const char* getMessageText() const;
 *     returns the message text that describes this exception.
 *
 *   const tring& getFileName() const;
 *     returns the file name from the location information in the exception
 *
 *   unsigned long getLineNumber() const;
 *     returns the line number from the location information in the exception
 *
 *   const char* getFunctionName() const;
 *     returns the function name from the location information in the exception
 *
 * Private Data Members :
 * ----------------------
 *   severity_level   severity;         * Severity of exception
 *   string           message_text;     * Message associated with exception
 *   string           file_name;        * File name (from __FILE__)
 *   unsigned long    line_number;      * Line number (from __LINE__)
 *   string           function_name;    * Function name
 *
 */

class  SDORuntimeException
{
  public:
    // Constructor
    enum severity_level
    {
      Normal,
      Warning,
      Error,
      Severe
    };

    SDO_API SDORuntimeException(const char *name="SDORuntimeException",
                 severity_level sev=Severe,
                 const char* msg_text="");

    SDO_API SDORuntimeException(const SDORuntimeException& c);

    // Destructor
    virtual  SDO_API ~SDORuntimeException();

    /**  getEClassName() get the exception class
     *
     * returns the name of the exception class, which will be
     * a subclass of SDORuntimeException
     */

    SDO_API const char* getEClassName() const;

    /**  getSeverity returns a level from severity_levels
     *
     * Return severity
     * from the enumerator severity_levels, which is:
     *
     * Normal,Warning, Error,Severe
      */

    SDO_API severity_level getSeverity() const;

    /**
     * Return message text associated with exception
     */

    SDO_API const char* getMessageText() const;

    /**  getFileName() the file where the exception occurred
     *
     *     Return file name where exception was raised
     */

    SDO_API const char* getFileName() const;

    /**  getLineNumber gives the line where the excepion occurred
     *
     * Return line number where exception was raised
     */

    SDO_API unsigned long getLineNumber() const;

    /**  getFunctionName give the name of the raising function
     *
     * Return function name where exception was raised
     */

    SDO_API const char* getFunctionName() const;


    /**  setSeverity
     *
     * sets an exception severity
     */

    SDO_API void setSeverity(severity_level sev);

    /**  setMessageText
     *
     * set the message text associated with exception
     */

    SDO_API void setMessageText(const char* msg_text);


    /**  setExceptionLocation
     *
     * set exception location
     */

    SDO_API void setExceptionLocation(const char* file,
                                unsigned long line,
                                const char* function="");

    SDO_API void setLocation(const char* file,
                                unsigned long line,
                                const char* function="");

    SDO_API void trace(const char* text="%1:\n  %3 %4 %2");

    SDO_API virtual std::ostream& PrintSelf(std::ostream &os) const;

    /**  operator << to write the exception
     *
     * ostream operator <<
     */

    SDO_API friend std::ostream& operator<< (std::ostream &os, const SDORuntimeException &except);
 
  protected:

  private:

    char*        class_name;

    severity_level   severity;         // Severity of exception

    char*        message_text;     // Description of exception

    //* Location where the exception was last thrown*handled

    class location
    {
      public:
        char*            file;       //* File name (from __FILE__)
        unsigned long   line;       //* Line number (from __LINE__)
        char*            function;   //* Function name
    };

    // Array of locations
    enum {num_locations=5};
    location         locations[num_locations];

    // Index into locations array
    int              location_set;


}; // End SDORuntimeException class definition

/** 
 ***************************************************************************
 *
 * SDOOutOfMemoryException - Exception for no-storage
 *
 * ***************************************************************************
 */

class SDOOutOfMemoryException: public SDORuntimeException
{
  public:
    SDOOutOfMemoryException(unsigned long size)
      : SDORuntimeException("SDOOutOfMemoryException", Severe,
                     "Unable to obtain storage of length " + size)
    {
    }
  private:
}; // End SDOOutOfMemory class definition

/** 
 ***************************************************************************
 *
 * SDONullPointerException - Exception for no-storage
 *
 * ***************************************************************************
 */

class SDONullPointerException: public SDORuntimeException
{
  public:
    SDONullPointerException(const char* param)
      : SDORuntimeException("SDONullPointerException", Warning,
                     "The referenced object is null ")
    {
    }
  private:
}; // End SDOOutOfMemory class definition
/**
***************************************************************************
*
* SDOPathNotFoundException - Exception for bad path
*
****************************************************************************
*/
class SDOPathNotFoundException: public SDORuntimeException
{
  public:
    SDOPathNotFoundException(const char* path)
      : SDORuntimeException("SDOPathNotFoundException", Warning,
                     path)
    {
    }
  private:
}; // End SDOPathNotFoundException class definition

/**
***************************************************************************
*
* SDOPropertyNotFoundException - Exception for property not found
*
****************************************************************************
*/

class SDOPropertyNotFoundException: public SDORuntimeException
{
  public:
    SDOPropertyNotFoundException(const char* name)
      : SDORuntimeException("SDOPropertyNotFoundException", Warning,
                      name)
    {
    }
  private:
}; // End SDOPropertyNotFoundException class definition

/** 
***************************************************************************
*
*  SDOTypeNotFoundException - Exception for type not found.
*
****************************************************************************
*/
class SDOTypeNotFoundException: public SDORuntimeException
{
  public:
    SDOTypeNotFoundException(const char* name)
      : SDORuntimeException("SDOTypeNotFoundException", Warning,
                     name)
    {
    }
  private:
}; // End SDOTypeNotFoundException class definition

/**
***************************************************************************
*
* SDOFileNotFoundException - Exception for file not found
*
****************************************************************************
*/

class SDOFileNotFoundException: public SDORuntimeException
{
  public:
    SDOFileNotFoundException(const char* name)
      : SDORuntimeException("SDOFileNotFoundException", Warning,
                     name)
    {
    }
  private:
}; // End SDOFileNotFoundException class definition
/** 
***************************************************************************
*
* SDOPropertyNotSetException - Exception for asking for the value of an 
* unset/undefaulted prop
*
****************************************************************************
*/
class SDOPropertyNotSetException: public SDORuntimeException
{
  public:
    SDOPropertyNotSetException(const char* name)
      : SDORuntimeException("SDOPropertyNotSetException", Warning,
                     name)
    {
    }
  private:
}; // End SDOPropertyNotSetException class definition
/** 
***************************************************************************
*
* SDOUnsupportedOperationException - Invalid action or unimplemented method.
*
****************************************************************************
*/
class SDOUnsupportedOperationException: public SDORuntimeException
{
  public:
    SDOUnsupportedOperationException(const char* name)
      : SDORuntimeException("SDOUnsupportedOperationException", Warning,
                      name)
    {
    }
  private:
}; // End SDOTypeNotFoundException class definition

/**
***************************************************************************
*
* SDOInvalidConversionException - Invalid conversion - cannot convert to type.
*
****************************************************************************
*/
class SDOInvalidConversionException: public SDORuntimeException
{
  public:
    SDOInvalidConversionException(const char* name)
      : SDORuntimeException("SDOInvalidConversionException", Warning,
                      name)
    {
    }
  private:
}; // End SDOTypeNotFoundException class definition
/** 
***************************************************************************
*
* SDOIllegalArgumentException - Invalid argument passed (null name?).
*
****************************************************************************
*/
class SDOIllegalArgumentException: public SDORuntimeException
{
  public:
    SDOIllegalArgumentException(const char* name)
      : SDORuntimeException("SDOIllegalArgumentException", Warning,
                      name)
    {
    }
  private:
}; // End SDOTypeNotFoundException class definition
/**
****************************************************************************
*
* SDOIndexOutOfRangeException - element index not in a list.
*
****************************************************************************
*/
class SDOIndexOutOfRangeException: public SDORuntimeException
{
  public:
    SDOIndexOutOfRangeException(const char* name)
      : SDORuntimeException("SDOIndexOutOfRangeException", Warning,
                      name)
    {
    }
  private:
}; // End SDOTypeNotFoundException class definition

/** 
***************************************************************************
*
* SDOXMLParserException - XMLParser error
*
****************************************************************************
*/
class SDOXMLParserException: public SDORuntimeException
{
  public:
    SDOXMLParserException(const char* name)
      : SDORuntimeException("SDOXMLParserException", Warning,
                      name)
    {
    }
  private:
}; // End SDOTypeNotFoundException class definition

/* 
***************************************************************************
*
* Macro definitions
*
****************************************************************************
*/
/**
  * =========================================================================
  * Macro  - SDO_THROW_EXCEPTION
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

  #define SDO_THROW_EXCEPTION(function_name, type, parameter )  \
    {\
     type ex(parameter); \
     ex.setExceptionLocation(__FILE__,__LINE__,#function_name); \
     throw ex;\
    }

/** 
    =========================================================================
  * Macro  - SDO_RETHROW_EXCEPTION
  *
  * adds the current file name, line number and function name to the exception.
  * then re-throws the exception.
  * The parameter 'function_name' should be the name of the function throwing
  * this exception.
  * =========================================================================
*/
  #define SDO_RETHROW_EXCEPTION(function_name, exception)  \
     (exception).setLocation(__FILE__,__LINE__,#function_name); \
     throw;

/**
  * =========================================================================
  * Macro  - SDO_HANDLE_EXCEPTION
  *
  * adds the current file name, line number and function name to the exception.
  * Writes an exception trace entry then continues.
  * The parameter 'function_name' should be the name of the function handling
  * this exception.
  * =========================================================================
*/
  #define SDO_HANDLE_EXCEPTION(function_name, exception)  \
    {\
     (exception).setLocation(__FILE__,__LINE__,#function_name); \
     (exception).Trace("Exception handled by %1: \n  %3 %4 %2");\
    }

#endif
