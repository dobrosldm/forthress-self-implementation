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
