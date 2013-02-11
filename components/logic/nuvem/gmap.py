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

def get(r, markers, s):

    def ismarker(m):
        return m[1] is not None

    def marker(m):
        # expecting (label or icon url, (color, loc))
        label = m[0][1:]
        deco = m[1][0]
        loc = string.join(map(str, m[1][1:3]), ',')
        if deco.find(':') == -1:
            return ("'markers", 'color:{0}|label:{1}|{2}'.format(deco, label, loc))
        return ("'markers", 'icon:{0}|{1}'.format(deco, loc))
    
    mv = markers.get(r)
    return s.get((("'size", '320x320'), ("'maptype", 'roadmap'), ("'sensor", 'true')) + tuple(map(marker, filter(ismarker, () if mv is None else mv))))

