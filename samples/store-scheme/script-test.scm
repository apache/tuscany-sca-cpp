;  Licensed to the Apache Software Foundation (ASF) under one
;  or more contributor license agreements.  See the NOTICE file
;  distributed with this work for additional information
;  regarding copyright ownership.  The ASF licenses this file
;  to you under the Apache License, Version 2.0 (the
;  "License"); you may not use this file except in compliance
;  with the License.  You may obtain a copy of the License at
;  
;    http://www.apache.org/licenses/LICENSE-2.0
;    
;  Unless required by applicable law or agreed to in writing,
;  software distributed under the License is distributed on an
;  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
;  KIND, either express or implied.  See the License for the
;  specific language governing permissions and limitations
;  under the License.

; Currency implementation

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

; Currency composite

(define (currency_service op . args) (currency_impl op args))

; Catalog implementation

(define (catalog_get converter)
  (define (convert price) (converter "convert" "USD" "USD" price))

  (define code "USD")
  (define symbol (converter "symbol" code))

  (list
    (list (list 'name "Apple") (list 'currencyCode code) (list 'currencySymbol symbol) (list 'price 2.99))
    (list (list 'name "Orange") (list 'currencyCode code) (list 'currencySymbol symbol) (list 'price 3.55))
    (list (list 'name "Pear") (list 'currencyCode code) (list 'currencySymbol symbol) (list 'price 1.55))
  )
)

(define (catalog_impl converter op args)
  (cond
    ((equal? op "items") (apply catalog_get (cons converter args)))
  )
)

; Catalog composite

(define (catalog_service op . args) (catalog_impl currency_service op args))

; Cart implementation

(define (cart_post content item)
  (cons (cons "Item" (list (uuid) item)) content)
)

(define (cart_getall content)
  (cons "Sample Feed" (cons (uuid) content))
)

(define (cart_getentry id)
  (define entry (list (list 'name "Apple") (list 'currencyCode "USD") (list 'currencySymbol "$") (list 'price 2.99)))
  (cons "Item" (list id entry))
)

(define (cart_total)
  10.0
)

(define (cart_impl op args)
  (cond
    ((equal? op "post") (apply cart_post args))
    ((equal? op "getall") (apply cart_getall args))
    ((equal? op "getentry") (apply cart_getentry args))
    ((equal? op "total") (apply cart_total args))
  )
)

; Store UI implementation

(define (storeui_post cart content item)
  (cart "post" content item)
)

(define (storeui_getcart cart content)
  (cart "getall" content)
)

(define (storeui_getentry cart id)
  (cart "getentry" id)
)

(define (storeui_items catalog)
  (catalog "items")
)

(define (storeui_total cart)
  (cart "total")
)

(define (storeui_impl cart catalog op args)
  (cond
    ((equal? op "post") (apply storeui_post (cons cart args)))
    ((equal? op "getall") (apply storeui_getcart (cons cart args)))
    ((equal? op "getentry") (apply storeui_getentry (cons cart args)))
    ((equal? op "items") (apply storeui_items (cons catalog args)))
    ((equal? op "total") (apply storeui_total (cons cart args)))
  )
)

; Store UI composite

(define (cart_service op . args) (cart_impl op args))

(define (storeui_service op . args) (storeui_impl cart_service catalog_service op args))

; Store UI test case

(define catalog (storeui_service "items"))
(define empty (list))
(define apple (car catalog))
(define orange (car (cdr catalog)))
(define added1 (storeui_service "post" empty apple))
(define added2 (storeui_service "post" added1 orange))
(display (storeui_service "getall" added2))
(display (storeui_service "total"))

