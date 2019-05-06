; This file stores utility words, required by Forth itself.

%define sys_read_no 0
%define sys_write_no 1

;----------------
; secondary tools
;----------------

; double duplicate
colon "2dup", ddup
  dq xt_to_r, xt_dup, xt_r_fetch, xt_swap, xt_from_r, xt_exit

; makes number of opposite sign
colon "neg", neg
  dq xt_lit, 0, xt_swap, xt_minus, xt_exit

; single increment
colon "inc", inc
  dq xt_lit, 1, xt_plus, xt_exit

; double increment
colon "2inc", dinc
  dq xt_to_r, xt_inc, xt_from_r, xt_inc, xt_exit

; drops if zero
colon "drop0", drop0
    dq xt_dup
    branch0 .is_zero
    dq xt_exit
  .is_zero:
    dq xt_drop, xt_exit

; simple system read
colon "sys-read", sys_read
  TIMES 3 dq xt_to_r
  dq xt_lit, sys_read_no
  TIMES 3 dq xt_from_r
  TIMES 3 dq xt_lit, 0
  dq xt_syscall, xt_drop, xt_exit

; simple system write
colon "sys-write", sys_write
  TIMES 3 dq xt_to_r
  dq xt_lit, sys_write_no
  TIMES 3 dq xt_from_r
  TIMES 3 dq xt_lit, 0
  dq xt_syscall, xt_drop, xt_exit

; for correct work-finishing with Forth
colon "bye", bye
  dq xt_lit, 60
  TIMES 6 dq xt_lit, 0
  dq xt_syscall

;-----------------------------
; tools for working with chars
;-----------------------------

; checks whether char is digit or not
colon "char-is-digit", char_is_digit
  dq xt_to_r
  dq xt_r_fetch, xt_lit, '9'+1, xt_lt
  dq xt_lit, '0'-1, xt_r_fetch, xt_lt, xt_land
  dq xt_from_r, xt_drop, xt_exit

; converts char to digit
; ( char -- digit )
colon "char-to-digit", char_to_digit
  dq xt_lit, '0', xt_minus, xt_exit

; check symbol if it's new line or tab
colon "char-is-space", char_is_space
  dq xt_to_r
  ; space
  dq xt_r_fetch, xt_lit, ' ', xt_equals
  ; new line symbol
  dq xt_r_fetch, xt_lit, 10, xt_equals, xt_lor
  ; caret return symbol
  dq xt_r_fetch, xt_lit, 13, xt_equals, xt_lor
  ; tab symbol
  dq xt_from_r,  xt_lit, '\t', xt_equals, xt_lor, xt_exit

; pushes cell to stack
colon "stack-cell", stack_cell
  dq xt_lit, 0, xt_sp, xt_exit

; reads char pointed by file descriptor
; pushes 0 or 1 as indicator
; ( fd - char 1) or ( - 0 )
colon "file-read-char", file_read_char
    dq xt_to_r, xt_stack_cell, xt_from_r, xt_swap, xt_lit, 1, xt_sys_read
    branch0 .fail
    dq xt_lit, 1
  .fail :
    dq xt_exit

; writes char pointed by file descriptor
; ( fd char - )
colon "file-write-char", file_write_char
  dq xt_swap, xt_to_r, xt_to_r
  dq xt_stack_cell, xt_dup, xt_from_r, xt_swap, xt_write_char
  dq xt_from_r, xt_swap, xt_lit, 1, xt_sys_write, xt_drop, xt_drop, xt_exit

; prints caret symbol
colon "print-cr", print_cr
  dq xt_lit, 1, xt_lit, 10, xt_file_write_char, xt_exit

;-------------------------------
; tools for working with strings
;-------------------------------

; counts string length
colon "string-length", string_length
    dq xt_dup
  .loop:
    dq xt_dup, xt_fetch_char
    branch0 .exit
    dq xt_inc
    branch .loop
  .exit:
    dq xt_swap, xt_minus, xt_exit

; prints a certain amount of characters from string
; ( str count - )
colon "print-string-counted", print_string_counted
  dq xt_to_r, xt_to_r, xt_lit, 1, xt_from_r, xt_from_r, xt_sys_write, xt_drop, xt_exit

; prints a string to stdout
; ( str -- )
colon "print-string", print_string
  dq xt_dup, xt_string_length, xt_print_string_counted, xt_exit

; simply terminates string by specific address
; ( addr - )
colon "string-terminate", string_terminate
  dq xt_lit, 0, xt_swap, xt_write_char, xt_exit

; read word into buffer from file descriptor
; ( fd buffer -  )
colon "file-read-word", file_read_word
    dq xt_dup, xt_to_r, xt_to_r
  .read_space:
    dq xt_dup, xt_file_read_char
    branch0 .eof
    dq xt_dup, xt_char_is_space
    branch0 .commit
    dq xt_drop
    branch .read_space
  .read:
    dq xt_dup, xt_file_read_char
    branch0 .eof
    dq xt_dup, xt_char_is_space, xt_not
    branch0 .end
  ; commits word that was read
  .commit:
    dq xt_r_fetch, xt_write_char
    dq xt_from_r, xt_inc, xt_to_r
    branch .read
  .end:
    dq xt_drop
  ; check for file end
  .eof:
    dq xt_drop, xt_r_fetch, xt_string_terminate
    dq xt_from_r, xt_from_r, xt_minus, xt_exit

; mainly for interpreter to find word in dictionary
; checks whether string equals to given or not
; (str1 str2 -- [0 or 1] )
colon "string-equals", string_equals
  .start:
    dq xt_ddup, xt_fetch_char, xt_swap, xt_fetch_char, xt_equals
    branch0 .no
    dq xt_dup, xt_fetch_char
    branch0 .yes
    dq xt_dinc
    branch .start
  .yes:
    dq xt_drop, xt_drop, xt_lit, 1, xt_exit
  .no:
    dq xt_drop, xt_drop, xt_lit, 0, xt_exit

; copies string from source to destination address
; ( dest source - )
colon "string-copy", string_copy
  .loop:
    dq xt_ddup, xt_fetch_char, xt_to_r, xt_r_fetch, xt_swap, xt_write_char
    dq xt_dinc, xt_from_r, xt_not
    branch0 .loop
    dq xt_drop, xt_drop, xt_exit

; tries to parse given string to unsigned number
; returns parsed number and its length
; ( buf -- num len )
colon "string-parse-unsigned-number", string_parse_unsigned_number
    dq xt_dup, xt_to_r
    dq xt_lit, 0, xt_to_r
  .loop:
    dq xt_dup, xt_fetch_char, xt_dup, xt_char_is_digit
    branch0 .end
    dq xt_char_to_digit
    dq xt_lit, 10, xt_from_r, xt_mul, xt_plus, xt_to_r
    dq xt_inc
    branch .loop
  .end:
    dq xt_drop, xt_from_r, xt_swap, xt_from_r, xt_minus, xt_drop0, xt_exit

; tries to parse given string to signed number
; returns parsed number and its length
; ( buf -- num len )
colon "string-parse-number", string_parse_number
    dq xt_dup, xt_fetch_char, xt_dup, xt_char_is_digit, xt_not
    branch0 .unsigned
    dq xt_dup, xt_lit, '-', xt_equals, xt_not
    branch0 .negative
    dq xt_dup, xt_lit, '+', xt_equals, xt_not
    branch0 .positive
  .negative:
    dq xt_drop
    dq xt_inc, xt_string_parse_unsigned_number
    dq xt_dup
    branch0 .error
    dq xt_inc
    dq xt_swap, xt_neg, xt_swap, xt_exit
  .positive:
    dq xt_drop
    dq xt_inc, xt_string_parse_unsigned_number
    dq xt_dup
    branch0 .error
    dq xt_inc, xt_exit
  .unsigned:
    dq xt_drop, xt_string_parse_unsigned_number, xt_exit 
  .error:
    dq xt_drop, xt_lit, 0, xt_exit
