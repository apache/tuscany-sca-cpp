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
 * Test HTTP client functions.
 */

#include "stream.hpp"
#include "string.hpp"
#include "client-test.hpp"

int main(const int argc, const char** argv) {
    tuscany::cout << "Testing..." << tuscany::endl;
    tuscany::server::testURI = argc > 1? argv[1] : "https://jane:jane@www.example.com:8453";

    tuscany::server::testServer();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}