# Store implementation

def post(item, catalog, shoppingCart, shoppingTotal):
    return shoppingCart("post", item)

def getall(catalog, shoppingCart, shoppingTotal):
    return shoppingCart("getall")

def get(id, catalog, shoppingCart, shoppingTotal):
    return shoppingCart("get", id)

def getcatalog(catalog, shoppingCart, shoppingTotal):
    return catalog("get")

def gettotal(catalog, shoppingCart, shoppingTotal):
    return shoppingCart("gettotal")

def deleteall(catalog, shoppingCart, shoppingTotal):
    return shoppingCart("deleteall")

def delete(id, catalog, shoppingCart, shoppingTotal):
    return shoppingCart("delete", id)

# TODO remove these JSON-RPC specific functions
def system.listMethods(catalog, shoppingCart, shoppingTotal):
    return ("Service.get", "Service.getTotal")

Service.getCatalog = getcatalog
Service.getTotal = gettotal

