# x86 Calling convention

```asm
cpu_get_basic_info:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]   ; that's param1 (uint32_t)
    mov ebx, [ebp + 12]  ; that's param2 (uint32_t)
    add eax, ebx

    pop ebp
    ret
```