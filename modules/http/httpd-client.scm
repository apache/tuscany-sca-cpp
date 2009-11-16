; JSON-RPC test case

(define (echo x ref) (ref "echo" x))

; ATOMPub test case

(define (getall ref)
  (ref "getall")
)

(define (get id ref)
  (ref "get" id)
)

(define (post entry ref)
  (ref "post" entry)
)

(define (put id entry ref)
  (ref "put" id entry)
)

(define (delete id ref)
  (ref "delete" id)
)
