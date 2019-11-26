#lang racket
(require racket/trace)

;  HOW TO DEBBUG::
;   type (require racket/trace)
;   then (trace <function>)
;   result is the call stack
;  Aknowlegments:: a cool dude in 100

;Armyanov HW+

;Problem 1
; 

(define (max-digit n)
  (define (mdh pow10 d num-d)
    ;      curr = (int)((n % pow10) / (pow10 / 10) )
    (let* ((left+c (remainder n pow10))
          (prev-pow (/ pow10 10))
          (next-pow (* pow10 10))
          (curr (quotient left+c prev-pow) )
          ;num-curr = (n - left+c) * (pow10 / 10)  +  left+c % (pow10 / 10)
          ; (n - left+c) / 10 is everything to the left of curr
          ;       with the right offset ( pow10 / 10 )
          ;       example : n = 123456 ; pow10 = 10^3 = 1000
          ;            left+c = 456 ; n - left+c = 123000
          ;            => 12300
          ; 
          (num-curr (+ (/ (- n left+c) 10) (remainder left+c prev-pow)))
          )
        (if (= (quotient n prev-pow) 0) (cons d num-d) 
            (if (> d curr) (mdh next-pow d num-d)
                (mdh next-pow curr num-curr)
                )
            )
        )
    )
     (mdh 10 0 n)
  )

;(trace max-digit)

(define (reduce n)
  (if (< n 10) n
      (reduce (* (car (max-digit n)) (cdr (max-digit n))))
      )
  )

;fuck yeah
;thank god for debugging and call stacks..

;Problem 2







