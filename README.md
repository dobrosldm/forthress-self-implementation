# Description
"forthress_self-implementation" is a Forth interpreter based on course by @sayon and his [forthress](https://github.com/sayon/forthress).
Forthress itself is written in NASM using bootstrap technique. It means that the main interpreter/compiler loop is written in Forthress. The inner interpreter is written in assembly, and so are some words.
# Words
## Arithmetic operators
`+`-- (a b -- \[a+b\])

`-` -- (a b -- \[a - b\])

`*` -- (a b -- \[a\*b\])

`/` -- (a b -- \[a/b\])

`%` -- (a b -- \[a mod b\])

## Logic operators
`=` -- (a b -- c) c = 1 if a == b else c = 0

`<` -- (a b -- \[a < b\])

`>` -- (a b -- \[a > b\])

`not` -- (a -- a') a' = 1 if a == 0 else a' = 1

`land` -- (a b -- \[a && b\])

`lor` -- (a b -- \[ a || b\])

## Bitwise operators
`and` -- (a b -- \[a & b\])

`or` -- (a b -- \[a | b\])

## Regular stack commands
`drop` -- (a -- )

`swap` -- (a b -- b a)

`dup` -- (a -- a a)

`2dup` -- (a -- a a a)

`inc` -- (a -- a+1)

`2inc` -- (a -- a+2)

`neg` -- (a -- -a)

`.s` -- prints stack without discarding its elements

## Return stack commands
`>r` -- push to return stack

`r>` -- pop from return stack

`r@` -- fetch from return stack

## Memory commands
`@` -- fetch by address (addr -- val)

`!` -- store value by address (val addr -- )

`c!` -- store char by addres (char addr -- )

`c@` -- fetch char by address (addr -- char)

`execute` -- execute word with this execution token (token -- )

`forth-dp` -- put the address of the first free segment of user's memory ( -- dp)

## Execution management commands
`docol` -- implementation of any colon word

`branch` -- jump to address

`0branch` -- jump to address if top of the stack equals 0

`exit` -- exit from colon word (any colon-word ends with it)

## Service commands
`lit` -- push value to stack

`sp` -- simply pushes rsp to access it

`forth-stack-base` -- push stack base

`bye` -- for correct work-finishing with Forth
