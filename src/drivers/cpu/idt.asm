;
; VOS kernel
; Copyright 2018 Michal Harasimowicz
;
[bits 32]
SECTION .text

global idt_flush

idt_flush:
    push ebp
    mov ebp, esp
    
    mov eax, [ebp + 8]
    lidt [eax]

    pop ebp
    
    ret
