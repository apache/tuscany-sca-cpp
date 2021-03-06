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

; Catalog implementation

(define (items converter currencyCode)
  (define code (currencyCode))
  (define (convert price) (converter "convert" "USD" code price))
  (define symbol (converter "symbol" code))
  (list
    (list (list 'name "Apple") (list 'currencyCode code) (list 'currencySymbol symbol) (list 'price (convert 2.99)))
    (list (list 'name "Orange") (list 'currencyCode code) (list 'currencySymbol symbol) (list 'price (convert 3.55)))
    (list (list 'name "Pear") (list 'currencyCode code) (list 'currencySymbol symbol) (list 'price (convert 1.55)))
  )
)

