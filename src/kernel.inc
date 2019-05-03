; This file stores the core defined words.

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

; ---------------------
; arithmetic operations
; ---------------------
native "+", plus
    pop rax
    add [rsp], rax
    jmp next

native "-", minus
    pop rax
    sub [rsp], rax
    jmp next

native "*", mul
    pop rax
    pop rdx
    imul rdx
    push rax
    jmp next

native "/", div
    pop rcx
    pop rax
    cqo
    idiv rcx
    push rax
    jmp next

native "%", mod
    pop rcx
    pop rax
    cqo
    idiv rcx
    push rdx
    jmp next

; ----------------
; logic operations
; ----------------
native "not", not
    pop rax
    test rax, rax
    setz al
    movzx rax, al
    push rax
    jmp next

native "=", equals
    pop rax
    pop rdx
    cmp rax, rdx
    sete al
    movzx rax, al
    push rax
    jmp next

native "<", lt
    pop rdx
    pop rax
    cmp rax, rdx
    setl al
    movzx rax, al
    push rax
    jmp next

native "land", land
    pop rax
    pop rdx
    test rax, rax
    jz .no
    push rdx
    jmp next
.no:
    push rax
    jmp next

native "lor", lor
    pop rax
    pop rdx
    test rax, rax
    jnz .yes
    push rdx
    jmp next
.yes:
    push rax
    jmp next

; --------------------
; exit from colon word
; --------------------
native "exit", exit
    rpop pc
    jmp next
