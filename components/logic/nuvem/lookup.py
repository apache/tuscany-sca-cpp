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

# Return a list of name value pairs that match a name
def get(r, n, l):
    def isList(v):
        if getattr(v, '__iter__', False) == False:
            return False
        if isinstance(v, basestring) or isinstance(v, dict):
            return False
        return True

    def isAssoc(v):
        return isList(v) and len(v) == 2 and isinstance(v[0], basestring) and v[0][0:1] == "'"

    def lookup(nv, lv):
        if lv == ():
            return ()

        # Check if list element is a name value pair assoc
        a = lv[0]
        if not isAssoc(a):
            return lookup(nv, lv[1:])

        # Got a match, return it and lookup rest of the list
        an = "'" + a[0][2:] if a[0][0:2] == "'@" else a[0]
        if an == nv:
            return (a,) + lookup(nv, lv[1:])

        # No match, lookup rest of the list
        return lookup(nv, lv[1:])

    def qsymbol(x):
        if not isinstance(x, basestring):
            return x
        return x if x[0:1] == "'" else "'" + x

    nv = n.get(r)
    lv = l.get(r)
    return lookup(qsymbol(nv), () if lv is None else lv)

