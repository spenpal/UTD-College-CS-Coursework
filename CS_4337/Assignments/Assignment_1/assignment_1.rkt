#|
Name: Sanjeev Penupala
NetID: SXP170022
Section Number: 501
|#

#lang scheme
;Question 5
#|
(define (sumodd lis)
  (cond
    ((null? lis) 0)
    ((not (list? (car lis)))
     (cond
       ((> (car lis) 7) (sumodd (cdr lis)))
       ((even? (car lis)) (sumodd (cdr lis)))
       (else (+ (car lis) (sumodd (cdr lis))))
    ))
    (else (+ (sumodd (car lis)) (sumodd (cdr lis))))
))

(print (sumodd '(9 2 3 4 5)))
(display "\n")
(print (sumodd '(9 2 3 (4 5))))
|#


;Question 6
#|
(define (deleteitem lis)
  (cond
    ((null? lis) '())
    ((eqv? (length lis) 2) (deleteitem (cdr lis)))
    (else (cons (car lis) (deleteitem (cdr lis))))
  )
)

(print (deleteitem '(a b c d e)))
(display "\n")
(print (deleteitem '(a b (c d) e)))

|#


;Question 7
#|
(define (newlist lis)
  (cond
    ((null? lis) '())
    ((list? (car lis)) (append (newlist (cdr lis)) (list (newlist (car lis)))))
    (else (append (newlist (cdr lis)) (list (car lis))))
  )
)

(print (newlist '((a b) (c d) a b)))
(display "\n")
(print (newlist '(a b (c) b d)))
|#


;Question 8
#|
(define (leaves lis)
  (reverse (flatten lis))
)

(print (leaves '(((1 2) (3 4)) ((1 2) (3 4)))))
|#


;Question 9
#|
(define (EXP-DEPTH lis)
  (cond
    ((null? lis) 1)
    ((not (list? (car lis))) (EXP-DEPTH (cdr lis)))
    (else (apply max (append (list (+ 1 (EXP-DEPTH (car lis)))) (list (+ 1 (EXP-DEPTH (cdr lis)))))))
  )
)

(print (EXP-DEPTH '(I J ((K) L) M)))
|#


;Question 10
#|
(define (subsets lis)
  (combinations lis)
)

(print (subsets '(a b c)))
|#