%define _lw 0

;-----------------------------------------------
; macroses for easier dictionary creation
;
; divides definition into data section and
; code section, where pointer to asm-code stored
;-----------------------------------------------
%macro native 3
    section .data
    wh_ %+ %2 : dq _lw
    db 0
    db %1, 0
    db %3
    
    %define _lw wh_%+ %2 
    xt_ %+ %2 :  dq i_ %+ %2 
    section .text
    i_ %+ %2:
%endmacro

%macro native 2
native %1, %2, 0
%endmacro
