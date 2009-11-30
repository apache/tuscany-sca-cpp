; Shopping cart implementation

(define cartId "1234")

; Get the shopping cart from the cache
; Return an empty cart if not found
(define (getcart id cache)
  (define cart (cache "get" id))
  (if (nul cart)
    (list)
    cart)
)

; Post a new item to the cart, create a new cart if necessary
(define (post item cache)
  (define id (uuid))
  (define cart (cons item (getcart cartId cache)))
  (cache "put" cartId cart)
  id
)

; Return the content of the cart
(define (getall cache)
  (define cart (getcart cartId cache))
  (cons "Your Cart" (cons cartId cart))
)

; Get an item from the cart
(define (get id cache)
  (define entry (list (list 'name "Apple") (list 'currencyCode "USD") (list 'currencySymbol "$") (list 'price 2.99)))
  (cons "Item" (list id entry))
)

; Delete the cart
(define (delete id cache)
  (cache "delete" cartId)
)

; Return the price of an item
(define (price item)
  (car (cdr (car (cdr (cdr (cdr (cdr (car (cdr (cdr item))))))))))
)

; Sum the prices of a list of items
(define (sum items)
  (if (nul items) 0 (+ (price (car items)) (sum (cdr items))))
)

; Return the total price of the items in the cart
(define (gettotal cache)
  (define cart (getcart cartId cache))
  (sum cart)
)

; TODO remove these JSON-RPC specific functions
(define (system.listMethods cache) (list "Service.getTotal"))
(define Service.getTotal gettotal)

