;
; VOS kernel
; Copyright 2018 Michal Harasimowicz
;
[bits 32]
SECTION .text

%define ID_FLAG 0x00200000

global cpu_has_cpuid_supported
global cpu_has_brand_string_supported
global cpu_get_basic_info
global cpu_get_branding_info

cpu_has_cpuid_supported:
    push ebp
    mov ebp, esp

    pushfd                                  ;Save EFLAGS
    pushfd                                  ;Store EFLAGS
    xor dword [esp], ID_FLAG                ;Invert the ID bit in stored EFLAGS
    popfd                                   ;Load stored EFLAGS (with ID bit inver$
    pushfd                                  ;Store EFLAGS again (ID bit may or may$
    pop eax                                 ;eax = modified EFLAGS (ID bit may or $
    xor eax,[esp]                           ;eax = whichever bits were changed
    popfd                                   ;Restore original EFLAGS
    and eax, ID_FLAG                        ;if non zero then cpuid supported

    cmp eax, 0

    je cpu_does_not_support_cpuid
    jmp cpu_supports_cpuid

cpu_supports_cpuid:
    pop ebp
    mov eax, 1
    ret

cpu_does_not_support_cpuid:
    pop ebp
    mov eax, 0
    ret



cpu_has_brand_string_supported:
    push ebp
    mov ebp, esp
    pushad

    mov eax, 1; calling cpuid for branding identification
    cpuid

    cmp eax, 080000004H
    je cpu_does_not_support_branding
    jmp cpu_supports_branding

cpu_supports_branding:
    popad
    pop ebp
    mov eax, 1
    ret

cpu_does_not_support_branding:
    popad
    pop ebp
    mov eax, 0
    ret




; cpuid documentation based on
; https://c9x.me/x86/html/file_module_x86_id_45.html
cpu_get_basic_info:
    push ebp
    mov ebp, esp
    pushad
    mov eax, [ebp + 8] ; text param <- start of result string
    push eax

    mov eax, 0 ;calling cpuid for basic info
    cpuid

    pop eax
    mov dword [eax + 0], ebx
    mov dword [eax + 4], edx
    mov dword [eax + 8], ecx
    mov byte  [eax + 12], 0x00

    popad
    pop ebp
    mov eax, 0
    ret


cpu_get_branding_info:
    push ebp
    mov ebp, esp
    pushad

    mov eax, [ebp + 8] ; text param <- start of result string
    
    push eax
    mov eax, 80000002H ;calling cpuid for branding info 1
    cpuid

    push eax
    pop ebx
    pop eax 

    mov dword [eax + 0], ebx

    push eax
    mov eax, 80000002H ;calling cpuid for branding info 1 continued
    cpuid

    pop eax

    mov dword [eax + 4], ebx
    mov dword [eax + 8], ecx
    mov dword [eax + 12], edx

    push eax
    mov eax, 80000003H ;calling cpuid for branding info 2
    cpuid

    push eax
    pop ebx
    pop eax 

    mov dword [eax + 16], ebx

    push eax
    mov eax, 80000003H ;calling cpuid for branding info 2 continued
    cpuid

    pop eax

    mov dword [eax + 20], ebx
    mov dword [eax + 24], ecx
    mov dword [eax + 28], edx

    push eax
    mov eax, 80000004H ;calling cpuid for branding info 3
    cpuid

    push eax
    pop ebx
    pop eax 

    mov dword [eax + 32], ebx

    push eax
    mov eax, 80000004H ;calling cpuid for branding info 3 continued
    cpuid

    pop eax

    mov dword [eax + 36], ebx
    mov dword [eax + 40], ecx
    mov dword [eax + 44], edx
    mov byte  [eax + 48], 0x00

    ;mov byte  [eax + 32], 0x00
    ;mov byte  [eax + 16], 0x00

    popad
    pop ebp
    mov eax, 0
    ret