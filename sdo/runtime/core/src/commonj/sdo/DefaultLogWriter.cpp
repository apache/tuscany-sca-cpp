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

#include "commonj/sdo/disable_warn.h"
#include "commonj/sdo/DefaultLogWriter.h"

#include <iostream>
using namespace std;

namespace commonj
{
    namespace sdo
    {

        DefaultLogWriter::DefaultLogWriter()
        {
            formatindent = 0;
        }

        DefaultLogWriter::~DefaultLogWriter()
        {
        }

        void DefaultLogWriter::log(int indent, int level, const char* msg)
        {
            if (indent < 0) formatindent += indent;
            for (int i=0; i < formatindent; i++)
            {
                cout << "  ";
            }
            cout << msg <<endl;
            if (indent > 0) formatindent += indent;
        }

    } // End namespace sca
} // End namespace commonj

