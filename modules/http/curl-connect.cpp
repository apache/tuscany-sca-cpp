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
 * HTTP connect command line test tool.
 */

#include <assert.h>
#include <stdio.h>
#include "stream.hpp"
#include "string.hpp"
#include "perf.hpp"
#include "http.hpp"

namespace tuscany {
namespace http {

const bool testConnect(const string& url, const string& ca = "", const string& cert = "", const string& key = "") {
    gc_scoped_pool p;

    CURLSession cs(ca, cert, key);
    const failable<bool> crc = connect(url, cs);
    assert(hasContent(crc));

    apr_pollset_t* pollset;
    apr_status_t cprc = apr_pollset_create(&pollset, 2, pool(p), 0);
    assert(cprc == APR_SUCCESS);
    apr_socket_t* csock = sock(0, p);
    const apr_pollfd_t* cpollfd = pollfd(csock, APR_POLLIN | APR_POLLERR | APR_POLLNVAL | APR_POLLHUP, p);
    apr_pollset_add(pollset, cpollfd);
    apr_socket_t* tsock = sock(cs);
    const apr_pollfd_t* tpollfd = pollfd(tsock, APR_POLLIN | APR_POLLERR | APR_POLLNVAL | APR_POLLHUP, p);
    apr_pollset_add(pollset, tpollfd);

    const apr_pollfd_t* pollfds;
    apr_int32_t pollcount;
    for(;;) {
        apr_status_t pollrc = apr_pollset_poll(pollset, -1, &pollcount, &pollfds);
        assert(pollrc == APR_SUCCESS);

        for (; pollcount > 0; pollcount--, pollfds++) {
            if (pollfds->rtnevents & APR_POLLIN) {
                char data[8192];
                if (pollfds->desc.s == csock) {
                    const size_t rl = ::read(0, data, sizeof(data));
                    if (rl == (size_t)-1)
                        return false;
                    if (rl > 0) {
                        const failable<bool> src = http::send(data, rl, cs);
                        assert(hasContent(src));
                    }
                }
                else {
                    const failable<size_t> frl = http::recv(data, sizeof(data), cs);
                    assert(hasContent(frl));
                    const size_t rl = content(frl);
                    if (rl == 0)
                        return true;
                    const size_t wl = ::write(0, data, rl);
                    assert(wl == rl);
                }
                continue;
            }
            assert(!(pollfds->rtnevents & (APR_POLLERR | APR_POLLHUP | APR_POLLNVAL)));
        }
    }
    return true;
}

}
}

int main(unused const int argc, const char** argv) {
    if (argc > 2)
        tuscany::http::testConnect(tuscany::string(argv[1]), tuscany::string(argv[2]), tuscany::string(argv[3]), tuscany::string(argv[4]));
    else
        tuscany::http::testConnect(tuscany::string(argv[1]));
    return 0;
}

