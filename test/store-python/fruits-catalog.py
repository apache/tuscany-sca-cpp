# Catalog implementation

def get(converter):
  def convert(price):
      return converter("convert", "USD", "USD", price)
  code = "USD"
  symbol = converter("symbol", code)
  return (
    (("'javaClass", "services.Item"), ("'name", "Apple"), ("'currencyCode", code), ("'currencySymbol", symbol), ("'price", convert(2.99))),
    (("'javaClass", "services.Item"), ("'name", "Orange"), ("'currencyCode", code), ("'currencySymbol", symbol), ("'price", convert(3.55))),
    (("'javaClass", "services.Item"), ("'name", "Pear"), ("'currencyCode", code), ("'currencySymbol", symbol), ("'price", convert(1.55)))
  )

# TODO remove these JSON-RPC specific functions
def listMethods(converter):
    return ("Service.get",)

