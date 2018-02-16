;************************************************************************
; Map the 8259A PIC to use interrupts 32-47 within our interrupt table
;************************************************************************
 
%define ICW_1 0x11				; 00010001 binary. Enables initialization mode and we are sending ICW 4
 
%define PIC_1_CTRL 0x20				; Primary PIC control register
%define PIC_2_CTRL 0xA0				; Secondary PIC control register
 
%define PIC_1_DATA 0x21				; Primary PIC data register
%define PIC_2_DATA 0xA1				; Secondary PIC data register
 
%define IRQ_0	0x20				; IRQs 0-7 mapped to use interrupts 0x20-0x27
%define IRQ_8	0x28				; IRQs 8-15 mapped to use interrupts 0x28-0x36

;
; VOS kernel
; Copyright 2018 Michal Harasimowicz
;
[bits 32]
SECTION .text

global idt_flush

; idt_flush:
;     push ebp
;     mov ebp, esp
    
;     mov eax, [ebp + 8]
;     lidt [eax]

;     pop ebp
;     ret

int_handler_49:
    mov ax, 0x10
    mov gs, ax
    mov dword [gs:0xB8000],') : '
int_handler_49_done:
    hlt

; int_handler_1:
;     mov ax, 0x10
;     mov gs, ax
;     mov dword [gs:0xB8000],': D '
; int_handler_1_done:
;     hlt
 
 idt:
    resd 50*2 ; 400 bytes
 
 idtr:
    dw (50*8)-1
    dd idt

idt_flush:
    lidt [idtr]
    

    mov eax,int_handler_49
    mov [idt+49*8],ax
    mov word [idt+49*8+2],0x08
    mov word [idt+49*8+4],0x8E00
    shr eax,16
    mov [idt+49*8+6],ax

    ; mov eax,int_handler_1
    ; mov [idt+0x21*8],ax
    ; mov word [idt+0x21*8+2],0x08
    ; mov word [idt+0x21*8+4],0x8E00
    ; shr eax,16
    ; mov [idt+0x21*8+6],ax

    cli ; flushing current interrupts
    sti

    ; mov al, 0xfd
    ; out 0x21, al
    ; mov al, 0xff
    ; out 0xa1, al  
    ; ; int 48
    int 49

    ;sti