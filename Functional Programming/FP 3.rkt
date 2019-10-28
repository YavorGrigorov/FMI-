#lang racket


(define (accum op nv a b term next)
  (if (> a b) nv
      (op (term a) (accum op nv (next a) b term next))
      )
  )

(define (sumdiv n)
  (accum + 0 1 (- n 1) (lambda (i) (if (= (remainder n i) 0) i 0)) (lambda (x) (+ x 1)))
  )

(let* ((+ *) (a 4) (b (+ a 3)) ( a (- b 2)))
  (+ a b 3))

(define (sorted? l)
  (if (or (null? l) (null? (cdr l))) #f
      (and (<= (car l) (car (cdr l)))
           (sorted? (cdr l)))
      )
  )

(define (insert-sorted l x)
  (cond ((null? l) (cons x '()))
        ((< x (car l)) (cons x l))
        (else (cons (car l) (insert-sorted (cdr l) x)))
        )
  )

(define false (lambda (x y) x))