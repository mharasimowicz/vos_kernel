;
; VOS kernel
; Copyright 2018 Michal Harasimowicz
;
[bits 32]
SECTION .text

global tss_flush

tss_flush:
    push ebp
    mov ebp, esp
    
    ;mov eax, [ebp + 8]
    mov eax, 0
    mov ax, 0x2B
    ltr ax
    
    pop ebp

    ret
