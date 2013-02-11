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

def get(r, n, a):
    nv = n.get(r)
    ntv = nv[1:] if nv[0:1] == "'" else nv
    av = a.get(r)

    content = filter(lambda e: e[0] == "'content", av[0])
    account = filter(lambda e: e[0] == "'account", () if len(content) == 0 else content[0])
    keys = filter(lambda e: e[0] == "'keys", () if len(account) == 0 else account[0])
    key = filter(lambda e: e[0] == "'key", () if len(keys) == 0 else keys[0])
    ekey = () if len(key) == 0 else key[0]
    akey = () if len(ekey) < 2 else ekey[1]

    def namedkey(k):
        return len(filter(lambda e: e[0] == "'@name" and e[1] == ntv, k)) != 0

    def keyvalue(k):
        return filter(lambda e: e[0] == "'@value", k)

    kv = map(keyvalue, filter(namedkey, akey))
    return None if len(kv) == 0 else kv[0][0][1]

