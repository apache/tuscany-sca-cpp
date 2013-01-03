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

# Mockup component references for testing

class reference:
    def __init__(self, name, l):
        self.name = name
        self.l = l

    def __call__(self, *args):
        return self.l(*args)

    def __getattr__(self, name):
        if name == 'get' or name == 'put' or name == 'patch' or name == 'post' or name == 'delete':
            return self
        raise AttributeError()

    def __repr__(self):
        return repr((self.name, self.l))

def mkref(name, l):
    return reference(name, l)

