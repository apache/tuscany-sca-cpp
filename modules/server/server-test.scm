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

; JSON-RPC test case

(define (echo x) x)

; ATOMPub test case

(define (get id)
  (if (null? id)
    '((feed (title "Sample Feed") (id "123456789") (entry
       (((title "Item") (id "111") (content (item (name "Apple") (currencyCode "USD") (currencySymbol "$") (price 2.99))))
        ((title "Item") (id "222") (content (item (name "Orange") (currencyCode "USD") (currencySymbol "$") (price 3.55))))
        ((title "Item") (id "333") (content (item (name "Pear") (currencyCode "USD") (currencySymbol "$") (price 1.55))))))))
    
    (list (list 'entry '(title "Item") (list 'id (car id)) '(content (item (name "Apple") (currencyCode "USD") (currencySymbol "$") (price 2.99))))))
)

(define (post collection item)
  '("123456789")
)

(define (put id item)
  true
)

(define (patch id item)
  true
)

(define (delete id)
  true
)
