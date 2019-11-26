#lang racket

; Idea for prolog interpreter with racket
;  prolog works by looking at a tree of all possible paths
;   get code -> gen tree -> find first true path (e.g. rets #t) ;
;   -> gen bin code for where we are ( '11011')
;   -> ret the true outcome and when asked for recalc go with the ecoding and
;         ret next true and chnge code


(define (member? el l)
  (if (null? l) #f
      (or (equal? el (car l))
          (member? el (cdr l))
          )
      )
  )

;Armyanov seminar ::
;
; ALOT needs to be implemented ....
;

;returns a list of extended paths
;   extened path are @path appened with the kids of the last node
;  example b.kids = [1 2 3]   path = [a c b]
;    out -> [ [a c b 1] [a c b 2] [a c b 3] ]
; IMPORTANT:: paths are looked at in reverse, e.g. it's [1 b c a] and so on
(define (extend path graph) '())
;it's obvious
(define (get-nodes graph) '())
(define (has-path? u v graph) #t)
;also implement a graph and get-kids and so on ...


;util ::
(define (extend path graph)
  (map (lambda (node) (cons node path))
       (filter (lambda (node) (or (< (length path) 2)
                                   (not (= node (cadr path)))
                                   )
                 )
               (all-next (car path) graph) ;the fuck !?
               )
       )
  )


;

(define (is-cyclic? path) ;without (a a) edges
  (and (> (length path) 2)
       (or (member? (car path) (cdr path))
           (is-cyclic? (cdr path))
           )
       )
  )

(define (is-cyclic-graph g)
  (define (dfs-cycle? front)
    (if (null? front) #f
        (or (is-cyclec? (car front))
            (dfs-cycle? (append (extend (car front) graph) (cdr front)))
            )
        )
    )
  (dfs-cycle? (map list (get-nodes g)))
  )

(define (is-cyclic-graph-bfs? g)
  (define (cycle-bfs? front)
    (if (null? front) #f
        (or (> (length (car front)) (length (get-nodes graph)))
            (cycle-bfs? (append (cdr front) (extend (car front) graph)))
            )
        )
    )
  (cycle-bfs? (map list (get-nodes graph)))
  )

(define (is-connected? graph)
;could use a let..
  (aplly and (map (lambda (x) (has-path? (car (get-nodes graph)) x))
                  (cdr (get-nodes graph)))
         )
  )

; implement yourself
;(define (is-connected? graph)
;  (let (nodes (get-nodes graph)))
;  (foldl (lamda (x y) (if (x) (has-path? (car nodes) y)) #f) #t ;the fuck??

;Weighted graph
; ( (a . ((c 1) (b 2) (d 3)) .. )
; or.. ( (a (c 1) (b 2) (d 3)) ..)
;

;A* ..
; the queue :: ( (path . weight) ..)
;    heuristic : sum (heu(x) | all x in path) + weight

; See how to solve game-15 and rubics cube with A*..
;    also time-frame systems
;

(define (best-path-A* a b graph)
  (define (A* front)
    (if (null? front) '()
        (if (= (caar front) b) (car front)
            (A* (sort (append (all-next-paths (car front)) (cdr front))))
            );TODO :: sort and all-next-path
        )
    )
  (A* (cons (list a) 0))
  )









