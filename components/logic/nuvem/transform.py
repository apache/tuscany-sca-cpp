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

def get(r, x, y, d):
    vx = x.get(r)
    vy = y.get(r)
    vd = d.get(r)
    rx = round(float(0 if vx is None else vx), 0)
    ry = round(float(0 if vy is None else vy), 0)
    rd = round(float(0 if vd is None else vd), 0)
    return 'position: absolute; -webkit-transform: translate3d({0:g}px,{1:g}px,0px) rotate({2:g}deg); -moz-transform: translate({0:g}px,{1:g}px) rotate({2:g}deg); -ms-transform: translate({0:g}px,{1:g}px) rotate({2:g}deg); transform: translate({0:g}px,{1:g}px) rotate({2:g}deg)'.format(rx, ry, rd)

