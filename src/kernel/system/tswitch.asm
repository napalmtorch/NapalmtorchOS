[GLOBAL switch_thread]
[EXTERN current_thread]

;uint32_t esp, ebp, eax, ebx, ecx, edx, esi, edi, eflags;

switch_thread:
    cli
    mov eax, [current_thread]
    mov eax, [eax]

    mov [eax+0],  esp
    mov [eax+4],  ebp
    mov [eax+8],  eax
    mov [eax+12], ebx
    mov [eax+16], ecx
    mov [eax+20], edx
    mov [eax+24], esi
    mov [eax+28], edi
    
    pushf
    pop ecx
    mov [eax+32], ecx

    mov eax, [esp+4]

    mov [current_thread], eax
    mov eax, [eax]
    
    mov esp, [eax+0]
    mov ebp, [eax+4]
    mov eax, [eax+8]
    mov ebx, [eax+12]
    mov ecx, [eax+16]
    mov edx, [eax+20]
    mov esi, [eax+24]
    mov edi, [eax+28]
    
    mov eax, [eax+32]
    push eax
    popf
    
    sti
    ret