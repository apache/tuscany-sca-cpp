; JSON-RPC test case

(define (echo x) x)

; ATOMPub test case

(define (getall)
  '("Sample Feed" "123456789"
     ("Item" "111" ((javaClass "services.Item") (name "Apple") (currencyCode "USD") (currencySymbol "$") (price 2.99)))
     ("Item" "222" ((javaClass "services.Item") (name "Orange") (currencyCode "USD") (currencySymbol "$") (price 3.55)))
     ("Item" "333" ((javaClass "services.Item") (name "Pear") (currencyCode "USD") (currencySymbol "$") (price 1.55))))
)

(define (get id)
  (define entry '((javaClass "services.Item") (name "Apple") (currencyCode "USD") (currencySymbol "$") (price 2.99)))
  (cons "Item" (list id entry))
)

(define (post entry)
  "123456789"
)

(define (put id entry)
  true
)

(define (deleteall)
  true
)

(define (delete id)
  true
)
