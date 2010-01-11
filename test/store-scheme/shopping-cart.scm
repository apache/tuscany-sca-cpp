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
  (define newItem (list (car item) id (caddr item)))
  (define cart (cons newItem (getcart cartId cache)))
  (cache "put" cartId cart)
  id
)

; Return the content of the cart
(define (getall cache)
  (cons "Your Cart" (cons cartId (getcart cartId cache)))
)

; Find an item in the cart
(define (find id cart)
  (if (nul cart)
    (cons "Item" (list "0" (list)))
    (if (= id (cadr (car cart)))
      (car cart)
      (find id (cdr cart))))
)

; Get an item from the cart
(define (get id cache)
  (find id (getcart cartId cache))
)

; Delete the whole cart
(define (deleteall cache)
  (cache "delete" cartId)
)

; Delete an item from the  cart
(define (delete id cache)
  true
)

; Return the price of an item
(define (price item)
  (cadr (assoc 'price (caddr item)))
)

; Sum the prices of a list of items
(define (sum items)
  (if (nul items)
    0
    (+ (price (car items)) (sum (cdr items))))
)

; Return the total price of the items in the cart
(define (gettotal cache)
  (define cart (getcart cartId cache))
  (sum cart)
)

; TODO remove these JSON-RPC specific functions
(define (listMethods cache) (list "Service.gettotal"))

