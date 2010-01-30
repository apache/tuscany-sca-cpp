# Catalog implementation

def get(converter, currencyCode):
  code = currencyCode()
  def convert(price):
      return converter("convert", "USD", code, price)
  symbol = converter("symbol", code)
  return (
    (("'javaClass", "services.Item"), ("'name", "Apple"), ("'currencyCode", code), ("'currencySymbol", symbol), ("'price", convert(2.99))),
    (("'javaClass", "services.Item"), ("'name", "Orange"), ("'currencyCode", code), ("'currencySymbol", symbol), ("'price", convert(3.55))),
    (("'javaClass", "services.Item"), ("'name", "Pear"), ("'currencyCode", code), ("'currencySymbol", symbol), ("'price", convert(1.55)))
  )

# TODO remove these JSON-RPC specific functions
def listMethods(converter, currencyCode):
    return ("Service.get",)

