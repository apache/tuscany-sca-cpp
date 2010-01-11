# Shopping cart implementation
import uuid
import sys

cartId = "1234"

# Get the shopping cart from the cache
# Return an empty cart if not found
def getcart(id, cache):
    cart = cache("get", id)
    if cart is None:
        return ()
    return cart

# Post a new item to the cart, create a new cart if necessary
def post(item, cache):
    id = str(uuid.uuid1())
    cart = ((item[0], id, item[2]),) + getcart(cartId, cache)
    cache("put", cartId, cart)
    return id

# Return the content of the cart
def getall(cache):
    return ("Your Cart", cartId) + getcart(cartId, cache)

# Find an item in the cart
def find(id, cart):
    if cart == ():
        return ("Item", "0", ())
    elif id == cart[0][1]:
        return cart[0]
    else:
        return find(id, cart[1:])

# Get an item from the cart
def get(id, cache):
    return find(id, getcart(cartId, cache))

# Delete the whole cart
def deleteall(cache):
    return cache("delete", cartId)

# Delete an item from the  cart
def delete(id, cache):
    return true

# Return the price of an item
def price(item):
    return float(filter(lambda x: x[0] == "'price", item[2])[0][1])

# Sum the prices of a list of items
def sum(items):
    if items == ():
        return 0
    return price(items[0]) + sum(items[1:])

# Return the total price of the items in the cart
def gettotal(cache):
    cart = getcart(cartId, cache)
    return sum(cart)

# TODO remove these JSON-RPC specific functions
def listMethods(cache):
    return ("Service.gettotal",)

