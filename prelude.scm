(define (> a b)
  (< b a))

(define (>= a b)
  (or (> a b)
      (= a b)))

(define (<= a b)
  (or (< a b)
      (= a b)))

(define (true? x) (not (not x)))
(define (false x) (not (true? x)))

(define (inc x) (+ x 1))
(define (dec x) (- x 1))

(define (cadr x) (car (cdr x)))
(define (caddr x) (car (cdr (cdr x))))

