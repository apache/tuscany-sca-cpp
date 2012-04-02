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
namespace scribestatus {

const int status(const string& host, const int port) {
    // Connect to Scribe
    scribe::Scribe& sc = *(new (gc_new<scribe::Scribe>()) scribe::Scribe(host, port));

    // Get its status
    const failable<string> fs = scribe::status(sc);

    // Interpret and display results
    if (!hasContent(fs)) {
        cerr << reason(fs) << endl;
        return 2;
    }
    const string s = content(fs);
    cout << s << endl;
    if (s == "ALIVE") 
        return 0;
    if (s == "STARTING" || s == "STOPPING" || s == "WARNING") 
        return 1;
    return 2;
}

}
}

int main(const int argc, const char** argv) {
    return tuscany::scribestatus::status(argc < 2? "localhost" : argv[1], argc < 3? 1463 : atoi(argv[2]));
}

