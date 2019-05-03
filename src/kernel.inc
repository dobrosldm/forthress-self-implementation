; This file stores the core predefined words.

; --------------------
; exit from colon word
; --------------------
native "exit", exit
    rpop pc
    jmp next

; -------------------
; stack-manipulations
; -------------------
native "drop", drop
    add rsp, 8
    jmp next

native "dup", dup
    push qword [rsp]
    jmp next

native "swap", swap
    pop rax
    pop rdx
    push rax
    push rdx
    jmp next
