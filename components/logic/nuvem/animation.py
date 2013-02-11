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
    if r[0:1] == ('setup',):
        ms = msec.get(r)
        if ms is None or ms == 0:
            return ''
        l = loop.get(r)
        lv = -1 if l is True else 0 if l is None or l is False else int(l)
        return 'setupAnimationHandler(' + str(int(ms)) + ', ' + str(lv) + ');'

    return content.get(r)

