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

; Shopping cart implementation

(define cartId "1234")

; Get the shopping cart from the cache
; Return an empty cart if not found
(define (getcart id cache)
  (define cart (cache "get" (list id)))
  (if (nul cart)
    (list)
    cart)
)

; Post a new item to the cart, create a new cart if necessary
(define (post collection item cache)
  (define id (uuid))
  (define newItem (list (car item) id (caddr item)))
  (define cart (cons newItem (getcart cartId cache)))
  (cache "put" (list cartId) cart)
  (list id)
)

; Find an item in the cart
(define (find id cart)
  (if (nul cart)
    (cons "Item" (list "0" (list)))
    (if (= id (cadr (car cart)))
      (car cart)
      (find id (cdr cart))))
)

; Get items from the cart
(define (get id cache)
  (if (nul id)
    (cons "Your Cart" (cons cartId (getcart cartId cache)))
    (find (car id) (getcart cartId cache))
  )
)

; Delete items from the  cart
(define (delete id cache)
  (if (nul id)
    (cache "delete" (list cartId))
    true
  )
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
(define (total cache)
  (define cart (getcart cartId cache))
  (sum cart)
)

