;
; VOS kernel
; Copyright 2018 Michal Harasimowicz
;
[bits 32]
SECTION .text

global gdt_flush

gdt_flush:
    push ebp
    mov ebp, esp
    
    mov eax, [ebp + 8]
    lgdt [eax]
    jmp 0x08:complete_flush

complete_flush:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    pop ebp
    
    ret
