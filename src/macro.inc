%define _lw 0

;-----------------------------------------------
; macroses for easier dictionary creation
;
; divides definition into data section and
; code section, where pointer to asm-code stored
;-----------------------------------------------

; for words supplied with flags
%macro native 3
    section .data
    wh_ %+ %2 : dq _lw
    db 0
    db %1, 0
    db %3
    
    %define _lw wh_%+ %2 
    xt_ %+ %2 :  
        dq i_ %+ %2 
        
    section .text
    i_ %+ %2:
%endmacro

; overloaded version with [0] as flags
%macro native 2
    native %1, %2, 0
%endmacro

;----------------------------------------
; macroses for return stack manipulations
;----------------------------------------

; push into return stack
%macro rpush 1
    sub rstack, 8
    mov qword [rstack], %1
%endmacro

; pop from return stack
%macro rpop 1
    mov %1, qword [rstack]
    add rstack, 8
%endmacro

;---------------------------------------------
; macroses for transition (if-else) operations
;---------------------------------------------
%macro branch 1 
    dq xt_branch
    dq %1
%endmacro

%macro branch0 1 
    dq xt_branch0
    dq %1 
%endmacro

;--------------------------------------------
; macros to return free user's memory pointer
;--------------------------------------------
%macro const 2
    colon %1, %2
        dq xt_lit,  %2, xt_exit
%endmacro
