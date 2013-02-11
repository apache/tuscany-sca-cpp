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

def get(r, msec, loop, content):
    import string

    ms = msec.get(r)
    if ms is None or ms == 0:
        return ''
    l = loop.get(r)
    lv = 'infinite' if l is True else '1' if l is None or l is False else '{0:g}'.format(l) 

    def keyframe(f):
        t = f[0]
        v = f[1]

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

        if isList(v):
            return '{0:g}% {{ {1} }}'.format(t, string.join(map(lambda x: (x[0][1:] + ': ' + str(x[1]) + ';') if isAssoc(x) else (str(x) + ';'), v), ' '))

        return '{0:g}% {{ {1} }}'.format(t, v)

    kf = ' '.join(map(keyframe, content.get(r)))

    uid = '{id}'
    return 'position: absolute; -webkit-animation: {0} {1:g}s ease 0 {2} normal; -moz-animation: {0} {1:g}s ease 0 {2} normal; <style> @-webkit-keyframes {0} {{ {3} }} @-moz-keyframes {0} {{ {3} }} </style>'.format(uid, ms / 1000.0, lv, kf)

