# JSON-RPC test case

def echo(x, ref):
    return ref("echo", x)

# ATOMPub test case

def getall(ref):
    return ref("getall")

def get(id, ref):
    return ref("get", id)

def post(entry, ref):
    return ref("post", entry)

def put(id, entry, ref):
    return ref("put", id, entry)

def delete(id, ref):
    return ref("delete", id)

