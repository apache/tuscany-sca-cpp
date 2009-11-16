; Shopping cart implementation

(define (post item)
  (uuid)
)

(define (getall)
  (cons "Sample Feed" (cons (uuid) '()))
)

(define (get id)
  (define entry (list (list 'name "Apple") (list 'currencyCode "USD") (list 'currencySymbol "$") (list 'price 2.99)))
  (cons "Item" (list id entry))
)

(define (delete id)
  true
)

(define (gettotal)
  11.0
)

; TODO remove these JSON-RPC specific functions
(define (system.listMethods) (list "Service.getTotal"))
(define Service.getTotal gettotal)
