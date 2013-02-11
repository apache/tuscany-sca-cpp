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

def get(r, n, m):
    import random
    mv = m.get(r)
    nv = n.get(r)

    def isList(v):
        if getattr(v, '__iter__', False) == False:
            return False
        if isinstance(v, basestring) or isinstance(v, dict):
            return False
        return True

    if isList(mv):
        return tuple(map(lambda i: random.choice(mv), tuple(range(0, int(0 if nv is None else nv)))))

    return tuple(map(lambda i: random.random() * (1.0 if mv is None else mv), tuple(range(0, int(0 if nv is None else nv)))))

