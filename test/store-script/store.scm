
(; "Currency implementation")

(define (currency_convert from to amount) 
  (if (equal? to "EUR") (* amount 0.70) amount)
)

(define (currency_symbol currency) 
  (if (equal? currency "EUR") "E" "$")
)

(define (currency_impl op args)
  (cond
    ((equal? op "convert") (apply currency_convert args))
    ((equal? op "symbol") (apply currency_symbol args))
  )
)

(; "Currency composite")

(define (currency_service op . args) (currency_impl op args))

(; "Catalog implementation")

(define (catalog_get converter)
  (define (convert price) (converter "convert" "USD" "USD" price))

  (define code "USD")
  (define symbol (converter "symbol" code))

  (list (list (list 'name "apple") (list 'currency code) (list 'symbol symbol) (list 'price (convert 2.99)))
    (list (list 'name "orange") (list 'currency code) (list 'symbol symbol) (list 'price (convert 3.55)))
    (list (list 'name "pear") (list 'currency code) (list 'symbol symbol) (list 'price (convert 1.55)))
   )
)

(define (catalog_impl converter op args)
  (cond
    ((equal? op "get") (apply catalog_get (cons converter args)))
  )
)

(; "Catalog composite")

(define (catalog_service op . args) (catalog_impl currency_service op args))

(; "Cart implementation")

(define (cart_post content item)
  (cons item content)
)

(define (cart_getall content)
  content
)

(define (cart_impl op args)
  (cond
    ((equal? op "post") (apply cart_post args))
    ((equal? op "getall") (apply cart_getall args))
  )
)

(; "Store UI implementation")

(define (storeui_post cart content item)
  (cart "post" content item)
)

(define (storeui_getcart cart content)
  (cart "getall" content)
)

(define (storeui_getcatalog catalog)
  (catalog "get")
)

(define (storeui_impl cart catalog op args)
  (cond
    ((equal? op "post") (apply storeui_post (cons cart args)))
    ((equal? op "getall") (apply storeui_getcart (cons cart args)))
    ((equal? op "getcatalog") (apply storeui_getcatalog (cons catalog args)))
  )
)

(; "Store UI composite")

(define (cart_service op . args) (cart_impl op args))

(define (storeui_service op . args) (storeui_impl cart_service catalog_service op args))

(; "Store UI test case")

(define catalog (storeui_service "getcatalog"))
(define empty (list))
(define apple (car catalog))
(define full (storeui_service "post" empty apple))
(display (storeui_service "getall" full))

(; "Store UI JSON-RPC interop test case")

(define (system.listMethods) (list "Service.get"))
(define (Service.get) (storeui_service "getcatalog"))

