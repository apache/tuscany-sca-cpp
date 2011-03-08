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

# JSON-RPC test case

def echo(x):
    return x

# ATOMPub test case

def get(id):
    if id == ("index.html",):
        return ("text/plain", ("It works!",))

    if id == ():
        return (("'feed", ("'title", "Sample Feed"), ("'id", "123456789"), ("'entry", 
            ((("'title", "Item"), ("'id", "111"), ("'content", ("'item", ("'name", "Apple"), ("'currencyCode", "USD"), ("'currencySymbol", "$"), ("'price", 2.99)))),
             (("'title", "Item"), ("'id", "222"), ("'content", ("'item", ("'name", "Orange"), ("'currencyCode", "USD"), ("'currencySymbol", "$"), ("'price", 3.55)))),
             (("'title", "Item"), ("'id", "333"), ("'content", ("'item", ("'name", "Pear"), ("'currencyCode", "USD"), ("'currencySymbol", "$"), ("'price", 1.55))))))),)

    content = ("'content", ("'item", ("'name", "Apple"), ("'currencyCode", "USD"), ("'currencySymbol", "$"), ("'price", 2.99)))
    return (("'entry", ("'title", "Item"), ("'id", id[0]), content),)

def post(collection, item):
    return ("123456789",)

def put(id, item):
    return True

def delete(id):
    return True
