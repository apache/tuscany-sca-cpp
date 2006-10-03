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

#ifndef _PARSER_ERROR_SETTER_H_
#define _PARSER_ERROR_SETTER_H_

#include "commonj/sdo/export.h"

#include "commonj/sdo/disable_warn.h"

#include <vector>

namespace commonj{
namespace sdo{

/**
 * The ParserErrorSetter builds a list of all the errors which 
 * occurred during a parse, so they can be displayed for the
 * user of an XSDHelper or XMLHelper
 */

class ParserErrorSetter
{
public:
    virtual ~ParserErrorSetter();
    virtual void setError(const char* message) = 0;
    virtual void clearErrors() = 0;
};
};
};

#endif
