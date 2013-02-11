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

def get(r, address, args):
    def isList(v):
        if getattr(v, '__iter__', False) == False:
            return False
        if isinstance(v, basestring) or isinstance(v, dict):
            return False
        return True

    def isAssoc(v):
        if not isList(v):
            return False
        if len(v) != 2:
            return False
        if isinstance(v[0], basestring) and v[0][0:1] == "'":
            return True
        return False

    l = args.get(r)
    lv = () if l is None else l
    la = map(lambda x: str(x), filter(lambda x: not isAssoc(x), lv))
    ka = map(lambda x: '='.join((x[0][1:], str(x[1]))), filter(lambda x: isAssoc(x), lv))

    va = address.get(r)
    return ('' if va is None else str(va)) + '/'.join(la) + ('?' if len(ka) != 0 else '') + '&'.join(ka)

