-- Created by Ivan Martsilenko SADMT-4
-- parser for lab 1 Theory and practice of lenguage proccesors
-- variant 12

module Parser where
import Data.List
import Data.Set as Set
import System.IO
import Data.Function

main :: String -> String -> IO()
main inputPath outputPath = do
   src <- readFile inputPath
   writeFile outputPath $ result src

cut30 :: String -> String
cut30 str = Prelude.take 30 str

--cleanup from delimetres and split
cleanup :: String -> [String]
cleanup str = words $ Prelude.map (\x -> if (x `elem` delimetres) then ' ' else x) str
  where delimetres = "<,/.>;\\|%$#№\"\'" 

makeUniq x = Set.toList (Set.fromList $ Prelude.map cut30 $ cleanup x )

countConsonants::[Char]->(Int)
countConsonants txt = countConsonantsAux txt 0 

countConsonantsAux::[Char]->Int->(Int)
countConsonantsAux [] con = (con)
countConsonantsAux (c:r) con
    |c=='a' || c=='e' || c=='i' || c=='o' || c=='u' || c== 'y' 
    || c=='A' || c == 'E' || c == 'I' || c== 'O' || c== 'U' || c == 'Y'  = countConsonantsAux r con 
    |otherwise = countConsonantsAux r (con + 1)

-- Sorting combined lists
mySort src  =sortBy (compare `on` snd)  $ zip ( makeUniq src) (Prelude.map countConsonants (makeUniq  src))

separateAfterSort ( word, _) = word

result :: String -> String
result src = unlines $ Prelude.map separateAfterSort $ mySort src 