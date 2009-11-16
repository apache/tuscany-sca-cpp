; Currency converter implementation

(define (convert from to amount)
  (if (equal? to "EUR") (* amount 0.70) amount)
)

(define (symbol currency) 
  (if (equal? currency "EUR") "E" "$")
)
