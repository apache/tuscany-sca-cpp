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

# Shopping cart implementation
import uuid
import sys

cartId = "1234"

# Get the shopping cart from the cache
# Return an empty cart if not found
def getcart(id, cache):
    cart = cache.get((id,))
    if cart is None:
        return ()
    return cart

# Post a new item to the cart, create a new cart if necessary
def post(collection, item, cache):
    id = str(uuid.uuid1())
    cart = (("'entry", item[0][1], ("'id", id), item[0][3]),) + getcart(cartId, cache)
    cache.put((cartId,), cart)
    return (id,)


# Find an item in the cart
def find(id, cart):
    if cart == ():
        return (("'entry", ("'title", "Item"), ("'id", 0)),)
    elif id == cart[0][2][1]:
        return (cart[0],)
    else:
        return find(id, cart[1:])

# Get items from the cart
def get(id, cache):
    if id == ():
        return ((("'feed", ("'title", "Your Cart"), ("'id", cartId)) + getcart(cartId, cache)),)
    return find(id[0], getcart(cartId, cache))

# Delete items from the  cart
def delete(id, cache):
    if id == ():
        return cache.delete((cartId,))
    return True

# Return the price of an item
def price(item):
    return float(filter(lambda x: x[0] == "'price", item[3][1][1:])[0][1])

# Sum the prices of a list of items
def sum(items):
    if items == ():
        return 0
    return price(items[0]) + sum(items[1:])

# Return the total price of the items in the cart
def total(cache):
    cart = getcart(cartId, cache)
    return sum(cart)

