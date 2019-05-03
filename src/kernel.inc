; This file stores the core predefined words.

native "exit", exit
    rpop pc
    jmp next

native "drop", drop
    add rsp, 8
    jmp next

native "dup", dup
    push qword [rsp]
    jmp next
