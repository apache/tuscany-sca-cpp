# JSON-RPC test case

def echo(x):
    return x

# ATOMPub test case

def getall():
  return ("Sample Feed", "123456789",
     ("Item", "111", (("'javaClass", "services.Item"), ("'name", "Apple"), ("'currencyCode", "USD"), ("'currencySymbol", "$"), ("'price", 2.99))),
     ("Item", "222", (("'javaClass", "services.Item"), ("'name", "Orange"), ("'currencyCode", "USD"), ("'currencySymbol", "$"), ("'price", 3.55))),
     ("Item", "333", (("'javaClass", "services.Item"), ("name", "Pear"), ("'currencyCode", "USD"), ("'currencySymbol", "$"), ("'price", 1.55))))

def get(id):
  entry = (("'javaClass", "services.Item"), ("'name", "Apple"), ("'currencyCode", "USD"), ("'currencySymbol", "$"), ("'price", 2.99))
  return ("Item", id, entry)

def post(entry):
    return "123456789"

def put(id, entry):
    return true

def deleteall():
    return true

def delete(id):
    return true

