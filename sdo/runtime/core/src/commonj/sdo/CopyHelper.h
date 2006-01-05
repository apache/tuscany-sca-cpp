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

/* $Rev$ $Date: 2005/12/22 16:54:14 $ */

#ifndef _COPYHELPER_H_
#define _COPYHELPER_H_


#include "commonj/sdo/DataObject.h"
#include "commonj/sdo/RefCountingPointer.h"

namespace commonj{
namespace sdo{

    /** CopyHelper provides static copying helper functions.
     *
     * CopyHelper provides shallow and deep copy of data objects.
     * copyShallow() copies the DataType members of the data object.
     * copy() copies all the members and recurses downwards though
     * the data graph
     */
    
class CopyHelper
{
    public:

    /** CopyHelper provides static copying helper functions.
     *
     * CopyHelper provides shallow and deep copy of data objects.
     * copyShallow() copies the DataType members of the data object.
     * copy() copies all the members and recurses downwards though
     * the data graph
     */
    static SDO_API DataObjectPtr copyShallow(DataObjectPtr dataObject);
    
    /** CopyHelper provides static copying helper functions.
     *
     * CopyHelper provides shallow and deep copy of data objects.
     * copyShallow() copies the DataType members of the data object.
     * copy() copies all the members and recurses downwards though
     * the data graph
     */
    static SDO_API DataObjectPtr copy(DataObjectPtr dataObject);

    private:
    static void transferlist(DataObjectList& to, DataObjectList& from, Type::Types t);
    static void transferitem(DataObjectPtr   to, DataObjectPtr   from, const Property& p);
    static DataObjectPtr internalCopy(DataObjectPtr dataObject, bool fullCopy);

};
};
};

#endif //_COPYHELPER_H_
