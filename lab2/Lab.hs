-- Created by Ivan Martsilenko SADMT-4
-- parser for lab 2 Theory and practice of lenguage proccesors
-- variant 4 wo w1 automaton

module Lab where
import Data.List
import Data.Set as Set
import System.IO
import Data.Function


main :: String -> String -> IO()
main inputPath outputPath = do
   src <- readFile inputPath
   writeFile outputPath $ result src


   --- dev ---

------------------------------------------- parse data ------------------------------------------------------------
test:: String -> String -> IO
test inputPath outputPath = do 
    writeFile outputPath 

w0 src = (lines src)!!0
StartStatus src = (lines src)!!1
FinishStatus src = words ((lines src)!!2)
statusFrom [] = {}
transSymbol [] = {}
statusTo [] = {}
    loadAutomaton (linex src) 


loadAutomaton x = 
    if null x then
        return ()
    else do  
        elems = words $ head x
        statusFrom++elems!!0
        transSymbol++elems!!1
        statusTo++elems!!2
        loadAutomaton (tail x) 

----------------------------------------- end parsing -------------------------------------------------------------