
-- Part 1 is on laptop


module SEM_N_2 where

-- making a function infixed 
--  f a b = a + b
--  1 'f' 2
--  --> 1+2 --> 3
--  i think

data BinTree a = 
        Empty |
        Node { val :: a, left::BinTree, right::BinTree}
        deriving (Eq, Show, Read)

leaf v = Node v Empty Empty
leaves :: BinTree a -> [a]
leaves Empty = []
leaves (Node v Empty Empty) = [v]
leaves (Node _ l v) = leaves l ++ leaves r -- ++ = append

mapTree :: (a->b) -> BinTree a -> BinTree b
mapTree _ Empty = Empty
mapTree f (Node v l r) = BinTree (f v) (mapTree f l) (mapTree f r)

foldTree :: (b -> a -> b) -> b -> BinTree a -> b
foldTree op nv Empty = nv
foldTree op nv (Node v l r) = foldTree op (op (foldTree op nv l) v) r 

-- see flip

-- data <type> <args> = <ctor>
data Map k v = Map [(k,v)]

getKeys :: Map k v -> [k]
getKeys (Map []) = []
getKeys (Map ((k,_) : t) ) = k : (getKeys (Map s)) 
-- when you hear that you reconstruct what you already have ..
--  
-- v2
getVals (Map l) = map snd l

insert k v (Map l) = 
          Map (k, v) : [(x,y) | (x,y) <- l , x /= k]
-- so that's how you did it // (x,y) <- l == (member (x,y) l)

remove k (Map l) = Map [(x,y) | (x,y) <- l, x /= k]
remove k m = Map [(x,y) | (x,y) <- toList map, x /=k]

insert k v m = Map (k,v) : (toList $ remove k m)

toList (Map l) = l

mapMap f m = Map (zip (getKeys m) (map f (map snd (toList m))) -- or (getVals m)

filterMap pred m = Map [(k,v) | (k,v) <- toList m , pred k]

contains k m = elem k (getKeys m)

find k m = snd $ filterMap (== k) m
-- test and see if you need "head" before snd
















