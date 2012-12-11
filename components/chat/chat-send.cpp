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
 * Test sending a message to an XMPP id.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "list.hpp"
#include "element.hpp"
#include "monad.hpp"
#include "value.hpp"
#include "perf.hpp"
#include "parallel.hpp"
#include "xmpp.hpp"

namespace tuscany {
namespace chat {

const bool sendmsg(const string& jid, const string& pass, const string& to, const string& msg) {
    XMPPClient xc(jid, pass);
    const failable<bool> c = connect(xc);
    assert(hasContent(c));
    const failable<bool> p = post(to, msg, xc);
    assert(hasContent(p));
    return true;
}

}
}

int main(unused const int argc, const char** const argv) {
    tuscany::chat::sendmsg(argv[1], argv[2], argv[3], argv[4]);
    return 0;
}
