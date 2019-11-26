#lang racket

; Literals
(define g '( (a b c)
             (b c d a)
             (c x y z 1)
             (d 1 2 a c)
             (x)
             (y 3)
             (z y)
             (1 2)
             (2)
             (3)
             )
  )

(define notg '( () g))

; Util::
(define (member? el l)
    (if (null? l) #f
        (or (eqv? el (car l)) 
            (member? el (cdr l)))
    )
)



; Graph G = (V, E) where
;   V = {v1, v2, .., vn | where vi is vertex}
;   E = { (u, v) from VxV } is subset of VxV
;
;   I v.
;   :: (v1 v2 .. vn) ( (vi1 . vi2) (vi3 . vi4) ...)
;

; G = ( (v1 k1 .. kk) (v2 k1 .. km) ..)
(define (graph? g)
  (or (null? g)
      (and (not (null? (car g)))
           (graph? (cdr g))
           )
      )
  )



(define (kids v g)
  (let (curr (caar g)))
  (cond ((null? g) #f)
        ((equal? v curr) (cdr curr))
        (else ((kids (cdr g))))
        )
  )

(define (edge? u v) (member? v (cdr u)))

(define (for i n step action . params)
  (if (< i n)
      (for (step i) n step action (action params))
      params
      )
  )




;(define (add-edge u v g) 
;    (let ())
;)

























