#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#    
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.

import unittest

# JSON-RPC test case

def echo(x, ref):
    e1 = ref("echo", x)
    e2 = ref.echo(x)
    assert e1 == e2
    return e1

# ATOMPub test case

def get(id, ref):
    return ref.get(id);

def post(collection, item, ref):
    return ref.post(collection, item)

def put(id, item, ref):
    return ref.put(id, item)

def patch(id, item, ref):
    return ref.patch(id, item)

def delete(id, ref):
    return ref.delete(id)
