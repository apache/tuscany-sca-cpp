# Currency converter implementation

def convert(fr, to, amount):
    if to == "EUR":
        return amount * 0.70
    return amount

def symbol(currency):
    if currency == "EUR":
        return "E"
    return "$"

