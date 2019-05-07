; This file stores words, required by outer interpreter.

;------------------------
; data and rodata section
;------------------------

section .bss
word_buf:
  resq 512
user_dict:
  resq 65536

;------------------
; functions section
;------------------

; initializes Forth from main file
forth_init:
  dq xt_lit, 0, xt_forth_interpret_fd, xt_bye

; interprets everything read from file descriptor fd
; (  fd -- )
colon "forth-interpret-fd", forth_interpret_fd
    dq xt_lit, xt_fd, xt_fetch, xt_to_r
    dq xt_fd, xt_write
  .loop:
    dq xt_fd, xt_fetch, xt_lit, word_buf, xt_file_read_word
    branch0 .eof
    dq xt_lit, word_buf, xt_forth_interpret
    branch .loop
  .eof:
    dq xt_from_r, xt_fd, xt_write, xt_exit
