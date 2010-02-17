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
  (if (nul id)
    '("Sample Feed" "123456789"
      ("Item" "111" ((javaClass "services.Item") (name "Apple") (currencyCode "USD") (currencySymbol "$") (price 2.99)))
      ("Item" "222" ((javaClass "services.Item") (name "Orange") (currencyCode "USD") (currencySymbol "$") (price 3.55)))
      ("Item" "333" ((javaClass "services.Item") (name "Pear") (currencyCode "USD") (currencySymbol "$") (price 1.55))))
      
    '("Item" "111" ((javaClass "services.Item") (name "Apple") (currencyCode "USD") (currencySymbol "$") (price 2.99))))
)

(define (post coll entry)
  '("123456789")
)

(define (put id entry)
  true
)

(define (delete id)
  true
)
