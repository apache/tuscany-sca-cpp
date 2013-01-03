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

# Mockup cache for testing

class cache:
    def __init__(self, name, values):
        self.name = name
        self.values = values

    def get(self, id):
        if id in self.values:
            return self.values[id]
        return None

    def put(self, id, value):
        self.values[id] = value
        return True

    def patch(self, id, value):
        self.values[id] = value
        return True

    def post(self, id):
        return self.put(id)

    def delete(self, id):
        if id in self.values:
            del self.values[id]
            return True
        return False

    def __repr__(self):
        return repr((self.name, self.values))

def mkcache(name, values = {}):
    return cache(name, values)

