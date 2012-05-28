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

/**
 * A utility that logs stdin into a scribe log.
 */

#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"

#undef debug
#define debug(...)
#include "scribe.hpp"

namespace tuscany {
namespace scribecat {

int cat(const string& host, const string& category, const string& type) {
    // Connect to Scribe
    scribe::Scribe& sc = *(new (gc_new<scribe::Scribe>()) scribe::Scribe("localhost", 1464));

    // Read lines from stdin and log them
    char buf[8193];
    for (;;) {
        gc_scoped_pool();

        // Write line prefix
        ostringstream os;
        if (length(type) != 0)
            os << "[" << logTime() << "] [" << type << "] ";
        const string prefix = str(os);
        const int pl = length(prefix);
        strcpy(buf, c_str(prefix));

        // Read log line
        const char* s = fgets(buf + pl, 8192 - pl, stdin);
        if (s == NULL)
            return 0;

        // Remove trailing '\n'
        const size_t l = strlen(s);
        if (l > 0)
            buf[pl + l - 1] = '\0';

        // Log the line
        const failable<bool> val = scribe::log(buf, host, category, sc);
        if (!hasContent(val))
            return 1;
    }
}

}
}

int main(const int argc, const char** argv) {
    return tuscany::scribecat::cat(argc < 2? "localhost" : argv[1], argc < 3? "default" : argv[2], argc < 4? "" : argv[3]);
}

