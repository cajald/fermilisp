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

(define (idenity x)
  x)

(define (caar x)
  (car (car x)))

(define (cdar x)
  (cdr (car x)))

(define (cddr x)
  (cdr (cdr x)))

(define (caddr x)
  (car (cddr x)))

(define (cdddr x)
  (cdr (cddr x)))

(define (length xs)
  (if (null? xs)
      0
      (+ 1 (length (cdr xs)))))

(define (append a b)
  (if (null? a)
      b
      (cons (car a)
	    (append (cdr a) b))))

(define (rev xs acc)
  (if (null? xs)
      acc
      (rev (cdr xs)
           (cons (car xs) acc))))

(define (reverse xs)
  (rev xs nil))

(define (map f xs)
  (if (null? xs)
      nil
      (cons (f (car xs))
	    (map f (cdr xs)))))

(define (filter pred xs)
  (if (null? xs)
      nil
      (if (pref (car xs))
	  (cons (car xs)
		(filter pred (cdr xs)))
	  (filter pred (cdr xs)))))

(define (foldl f acc xs)
  (if (null? xs)
      acc
      (foldl f
	     (f acc (car xs))
	     (cdr xs))))

(define (member x xss)
  (if (null? xs)
      #f
      (if (= x (car xs))
	  #t
	  (member x (cdr xs)))))

(define (zero? x)
  (= x 0))

(define (positive? x)
  (< 0 x))

(define (negative? x)
  (< x 0))

(define (zero? x)
  (= x 0))

(define (fact n)
  (if (= n 0)
      1
      (* n (fact (- n 1)))))

