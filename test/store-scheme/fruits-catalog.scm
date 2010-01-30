; Catalog implementation

(define (get converter currencyCode)
  (define code (currencyCode))
  (define (convert price) (converter "convert" "USD" code price))
  (define symbol (converter "symbol" code))
  (list
    (list (list 'javaClass "services.Item") (list 'name "Apple") (list 'currencyCode code) (list 'currencySymbol symbol) (list 'price (convert 2.99)))
    (list (list 'javaClass "services.Item") (list 'name "Orange") (list 'currencyCode code) (list 'currencySymbol symbol) (list 'price (convert 3.55)))
    (list (list 'javaClass "services.Item") (list 'name "Pear") (list 'currencyCode code) (list 'currencySymbol symbol) (list 'price (convert 1.55)))
  )
)

; TODO remove these JSON-RPC specific functions
(define (listMethods converter currencyCode) (list "Service.get"))

