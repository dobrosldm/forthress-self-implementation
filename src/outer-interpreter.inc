; This file stores words, required by outer interpreter.

;------------------------
; data and rodata section
;------------------------

section .bss
word_buf:
  resq 512
user_dict:
  resq 65536

section .data
here:
  dq user_dict
fd:
  dq 0
state:
  dq STATE_INTERPRET

const "forth-here", here
const "forth-last-word", last_word
const "forth-is-compiling", state
const "forth-input-fd", fd
const "forth-stack-start", stack_start

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

; Forthress interpreter
; uses in_fd internally to know what to interpret
; ( str - )
colon "forth-interpret", forth_interpret
  .start:
    ; ( inbuf )
    dq xt_dup, xt_forth_find_word
    ; ( inbuf dictaddr? 0/1 )     
    branch0 .number
    ; ( xt )                       
    dq xt_swap, xt_drop, xt_forth_cfa     
    ; if not compiling
    dq xt_state, xt_fetch
    branch0 .interpreting
    dq xt_dup, xt_forth_xt_is_immediate, xt_not
    branch0 .interpreting
  .compiling:
    dq xt_comma, xt_exit
  .interpreting:
    dq xt_execute, xt_exit
  ; ( inbuf )
  .number: 
    ; ( inbuf num? 0/len )
    dq xt_dup, xt_string_parse_number      
    branch0 .no_word
    ; ( inbuf num )
    dq xt_swap, xt_drop
    ; if interpreting, that's it
    dq xt_state, xt_fetch                  
    branch0 .exit
    dq xt_forth_follows_branch, xt_not
    branch0 .is_branch
  .not_branch:
    dq xt_lit, xt_lit, xt_comma
  ; ( num prev_xt )
  .is_branch: 
    dq xt_comma, xt_exit
  ; ( inbuf )
  .no_word: 
    dq xt_lit, msg.no_such_word, xt_print_string, xt_print_string, xt_print_cr
  .exit:
    dq xt_exit

; finds executable part of forth-word in dictionary
colon "forth-find-word", forth_find_word
    dq xt_to_r, xt_last_word, xt_fetch
    dq xt_state, xt_fetch
    branch0 .loop
    dq xt_fetch
  .loop:
    dq xt_dup
    branch0 .not_found
    dq xt_dup, xt_forth_dict_word_name, xt_r_fetch, xt_string_equals, xt_not
    branch0 .found
    dq xt_fetch
    branch .loop
  .found:
    dq xt_lit, 1
  .not_found:
    dq xt_from_r, xt_drop, xt_exit

; finds forth-word by name in dictionary
colon "forth-dict-word-name", forth_dict_word_name
  dq xt_lit, 9, xt_plus, xt_exit
