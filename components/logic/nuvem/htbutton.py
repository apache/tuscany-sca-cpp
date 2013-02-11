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

import string

def get(r, value):
    v = value.get(r)

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

    def mkbutton(v):
        vl = filter(lambda x: not isAssoc(x), v)
        al = (filter(lambda x: isAssoc(x) and x[0] == "'htattrs", v) + (("'htattrs",()),))[0][1]
        l = (() if len(vl) == 0 else (("'id", vl[0]), ("'value", vl[0])) if len(vl) == 1 else (("'id", vl[0]), ("'value", vl[1]))) + al
        if len(l) == 0:
            return ''
        satts = string.join(map(lambda x: x[0][1:] + '="' + x[1] + '"', filter(lambda x: x[0] == "'id", l)), ' ')
        batts = string.join(map(lambda x: x[0][1:] + '="' + x[1] + '"', filter(lambda x: x[0] != "'id", l)), ' ')
        return '<SPAN {0} class="button"><INPUT type="button" {1} class="graybutton"/></SPAN>'.format(satts, batts)

    return mkbutton(()) if v is None else mkbutton(v) if isList(v) and not isAssoc(v) else mkbutton((v,))

