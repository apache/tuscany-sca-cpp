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
# Search through nested lists of name value pairs
def get(r, p, l):
    def isList(v):
        if getattr(v, '__iter__', False) == False:
            return False
        if isinstance(v, basestring) or isinstance(v, dict):
            return False
        return True

    def isAssoc(v):
        return isList(v) and len(v) >= 2 and isinstance(v[0], basestring) and v[0][0:1] == "'"

    def lookup(pv, lv):
        if lv == ():
            return ()
        if isAssoc(lv):
            lv = (lv,)

        # Check if list element is a name value pair assoc
        a = lv[0]
        if not isAssoc(a):
            # Lookup children if any and rest of list
            if isList(a):
                return lookup(pv, a) + lookup(pv, lv[1:])
            return lookup(pv, lv[1:])

        # Path segment match
        an = "'" + a[0][2:] if a[0][0:2] == "'@" else a[0]
        if an == pv[0]:
            # Found leaf, return it and lookup rest of the list
            if len(pv) == 1:
                return (a,) + lookup(pv, lv[1:])
            # Continue to lookup children if any plus rest of the list
            return lookup(pv[1:], a[1:]) + lookup(pv, lv[1:])

        # No match, lookup any children and rest of the list
        if (isList(a[1])):
            return lookup(pv, a[1]) + lookup(pv, a[2:]) + lookup(pv, lv[1:])
        return lookup(pv, a[2:]) + lookup(pv, lv[1:])

    def qsymbol(x):
        if not isinstance(x, basestring):
            return x
        return x if x[0:1] == "'" else "'" + x

    # Support path as a list or a dot separated string
    lv = l.get(r)
    pv = p.get(r)
    if (isList(pv)):
        return lookup(map(qsymbol, pv), () if lv is None else lv)

    spv = map(qsymbol, qsymbol(pv)[1:].split('.'))
    return lookup(spv, () if lv is None else lv)

