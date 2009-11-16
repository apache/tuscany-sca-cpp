; Store implementation

(define (post item catalog shoppingCart shoppingTotal)
  (shoppingCart "post" item)
)

(define (getall catalog shoppingCart shoppingTotal)
  (shoppingCart "getall")
)

(define (get id catalog shoppingCart shoppingTotal)
  (shoppingCart "get" id)
)

(define (getcatalog catalog shoppingCart shoppingTotal)
  (catalog "get")
)

(define (gettotal catalog shoppingCart shoppingTotal)
  (shoppingCart "gettotal")
)

(define (delete id catalog shoppingCart shoppingTotal)
  (shoppingCart "delete" id)
)

(define (system.listMethods) (list "Service.get" "Service.getTotal"))

(define Service.getCatalog getcatalog)

(define Service.getTotal gettotal)
