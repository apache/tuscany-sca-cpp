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

def get(r, fmt, args):
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

    def trimq(x):
        if not isinstance(x, basestring):
            return x
        return x[1:] if x[0:1] == "'" else x

    l = args.get(r)
    lv = () if l is None else l
    la = map(trimq, filter(lambda x: not isAssoc(x), lv))
    ka = dict(map(lambda x: (x[0][1:], x[1]), filter(lambda x: isAssoc(x), lv)))

    vfmt = fmt.get(r)
    return ('' if vfmt is None else str(vfmt)).format(*la, **ka)

