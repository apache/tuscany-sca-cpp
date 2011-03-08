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

# Palettes collection implementation
import uuid
import sys
from util import *

# Convert an id to a palette id
def paletteid(id):
    return ("'" + car(id), "'palette.composite")

# Get a palette from the palettes db
def get(id, cache):
    if isNil(id):
        return (("'feed", ("'title", "Palettes"), ("'id", "palettes")),)
    return (("'entry", ("'title", car(id)), ("'id", car(id)), ("'content", car(cache.get(paletteid(id))))),)

