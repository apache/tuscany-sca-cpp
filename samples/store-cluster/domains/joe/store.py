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

# Store implementation

def post(item, catalog, shoppingCart, shoppingTotal):
    return shoppingCart.post(item)

def getall(catalog, shoppingCart, shoppingTotal):
    return shoppingCart.getall()

def get(id, catalog, shoppingCart, shoppingTotal):
    return shoppingCart.get(id)

def items(catalog, shoppingCart, shoppingTotal):
    return catalog.items()

def total(catalog, shoppingCart, shoppingTotal):
    return shoppingCart.total()

def deleteall(catalog, shoppingCart, shoppingTotal):
    return shoppingCart.deletall()

def delete(id, catalog, shoppingCart, shoppingTotal):
    return shoppingCart.delete(id)

