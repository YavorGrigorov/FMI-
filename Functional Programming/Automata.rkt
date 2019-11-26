
#lang racket

(define (member? el l)
  (if (null? l) #f (or (= el (car l)) (member? el (cdr l)))))


; Determined automata A = (Alpha, Q, s, delta, F) where
;       Alpha - finite set of symbols (literals)
;       Q - finite set of states
;       s from Q - starting state
;       F subset of Q - accepting states
;       delta : QxAlpha -> Q    partial funtion
;   L(A) = {alpha from Alpha* where delta*(s, alpha) is from F}
;
; 

; delta = ( (q1 a1 p1) (q2 a2 p2) ... (qk ak pk)) where k is natural num
(define (automata Alpha Q s F delta) 
    (define (del q a) 
        (define (memb q a dl)
            (cond ((null? dl) #f)
                ((and (= (caar dl) q) (= (cadar dl) a)) (caddar dl))
                (else (memb q a (cdr dl)))
            )
        )
      (memb q a delta)
    )
    (list Alpha Q s F del) ;; in all honesty, Alpha and Q are obselite
)
(define (word? alpha A)
    (let* (
        (s (cddar A))
        (F (cdar (cddr A)))
        (delta (car (cddr (cddr A))))
    )
    (define (delta* q alpha)
        (if (null? alpha) (member? q F)
            (delta* (delta q (car alpha)) (cdr alpha))
        )
    )
      
    (delta* s alpha)
))


(define (mif cond? do eldo)
    (or (and (cond?) (do)) 
        (eldo))
)

(define (f l)
    (mif (lambda () (null? l)) (lambda () l) 
        (lambda () (append (f (cdr l)) (list (car l))))
    )
)

(define (while cond? action)
  (if (cond?) (or (action) (while cond? action)) #f))

;shouldn't work
(define (g l)
  (while (lambda () (not (null? l)))
           (lambda () (append (cdr l) (car l)))
           )
  )

(define (for init cond? corr action)
  (define (for-help iter)
    (if (cond? iter) (and (action iter) (for-help (corr iter) cond? corr action)) #f)
  )
  (for-help (init))
)




