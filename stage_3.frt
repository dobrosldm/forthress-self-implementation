: here forth-here @ ;

: IMMEDIATE forth-last-word @ forth-cfa 1 - dup c@ 1 or swap c! ;

: do here ; IMMEDIATE
: while ' 0branch , , ; IMMEDIATE

: ( do 0 file-read-char drop 41 = while ; IMMEDIATE

( line of code above lets us to write comments )

( "here" is like a label - we use it when we need to jump from "here 0" places )
( it helps to implement branching )