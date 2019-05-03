; This file stores the core defined words.

;-------------------------------------
; for manipulations with regular stack
;-------------------------------------
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

;----------------------
; arithmetic operations
;----------------------
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

;-----------------
; logic operations
;-----------------
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

;-------------------
; bitwise operations
;-------------------
native "and", and
    pop rax
    and [rsp], rax
    jmp next

native "or", or
    pop rax
    or [rsp], rax
    jmp next

;------------------------------------
; for manipulations with return stack
;------------------------------------

; moves element from regular to return stack
native ">r", to_r
    pop rax
    rpush rax
    jmp next

; moves element from return to regular stack
native "r>", from_r
    rpop rax
    push rax
    jmp next

; 'r>' logic but without removing from return stack
native "r@", r_fetch
    push qword [rstack]
    jmp next

;--------------------------------------------
; when just putting values on stack we want 
; Forthress to avoid executing them as words
;
; 'lit' does this job
;--------------------------------------------
native "lit", lit
    push qword [pc]
    add pc, 8
    jmp next

native "branch", branch
    mov pc, [pc]
    jmp next

native "0branch", branch0
    pop rax
    test rax, rax
    jnz .skip
    mov pc, [pc]
    jmp next
.skip:
    add pc, 8
    jmp next

;--------------------
; working with memory
;--------------------

; fetch value from memory
native "@", fetch
    pop rax
    push qword[rax]
    jmp next

; store value by address
native "!", write
    pop rax
    pop rdx
    mov [rax], rdx
    jmp next

; store one byte by address
native "c!", write_char
    pop rax
    pop rdx
    mov [rax], dl
    jmp next

; read one byte starting at addr
native "c@", fetch_char
    pop rax
    movzx rax, byte [rax]
    push rax
    jmp next

; execute word with it's execution token
native "execute", execute
    pop rax
    mov w, rax
    jmp [rax]

; push dp-adress to stack
; it points to the first empty cell in user's data section
const "forth-dp", dp

;-------------------------------------------
; implementation of any colon-word
;
; it's role to save old [pc] value
; to return stack and put a new value in it
;-------------------------------------------
native "docol", docol
    rpush pc
    add w, 8
    mov pc, w
    jmp next

;---------------------------------------------------
; exit from colon word (any colon-word ends with it)
;
; it returns [pc] to the state before executed word
;---------------------------------------------------
native "exit", exit
    rpop pc
    jmp next
