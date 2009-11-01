(; "JSON-RPC test case")

(define (echo x) x)

(; "ATOMPub test case")

(define (getall)
  '("Sample Feed" "123456789"
     ("Item" "111" (javaClass "services.Item") (name "Apple") (currency "USD") (symbol "$") (price 2.99))
     ("Item" "222" (javaClass "services.Item") (name "Orange") (currency "USD") (symbol "$") (price 3.55))
     ("Item" "333" (javaClass "services.Item") (name "Pear") (currency "USD") (symbol "$") (price 1.55)))
)

(define (get id)
  (define entry '((javaClass "services.Item") (name "Apple") (currency "USD") (symbol "$") (price 2.99)))
  (cons "Item" (list id entry))
)

(define (post entry)
  (display entry)
  "123456789"
)

(define (put entry)
  (display entry)
  true
)

(define (delete . args)
  (display args)
  true
)

