; This is the main file which defines initial Forthress state.

global _start

%include "macro.inc"

; command counter
%define pc r15
; address for current instruction
%define w r14
; return stack position
%define rstack r13

;-------------------------------------------------------------
section .text

; new core defined words are stored here
%include "kernel.inc"

;-------------------------------------------------------------
section .bss

; global data for user
user_mem:
	resq 65536
; returns stack end
resq 1023              
; returns stack start
rstack_start:
	resq 1 

;-------------------------------------------------------------
section .data

; a pointer to the last word in the dictionary
last_word:
	dq _lw      
; pointer to current global data
dp:
	dq user_mem
; address of data stack
stack_start:
	dq 0 

;-------------------------------------------------------------
section .text

; virtual machine starting point
_start: 
    mov rstack, rstack_start
    mov [stack_start], rsp
    ;  setting up initial stack state
    mov pc, forth_init

; Forthress inner-interpreter
next:                  
    mov w, pc
    add pc, 8
    mov w, [w]
    jmp [w]
