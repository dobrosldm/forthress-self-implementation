: here forth-here @ ;

: IMMEDIATE forth-last-word @ forth-cfa 1 - dup c@ 1 or swap c! ;

: do here ; IMMEDIATE
: while ' 0branch , , ; IMMEDIATE

: ( do 0 file-read-char drop 41 = while ; IMMEDIATE

( line of code above lets us to write comments )

( "here" is like a label - we use it when we need to jump from "here 0" places )
( it helps to implement branching )

: if ' 0branch , here 0 , ; IMMEDIATE
: then here swap ! ; IMMEDIATE
: else ' branch , here 0 , swap here swap ! ; IMMEDIATE

: for 
      ' swap ,
      ' >r , 
      ' >r , 
    here  ' r> , 
      ' r> , 
      ' 2dup , 
      ' >r , 
      ' >r , 
      ' < ,  
      ' 0branch ,  
    here    0 , 
       swap 
; IMMEDIATE

: endfor 
    ' r> , 
    ' lit , 1 ,   
    ' + , 
    ' >r , 
    ' branch , 
  here swap ! 
    ' r> , 
    ' drop , 
    ' r> , 
    ' drop ,  
; IMMEDIATE
