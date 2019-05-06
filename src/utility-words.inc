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
