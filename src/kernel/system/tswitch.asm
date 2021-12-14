[GLOBAL switch_thread]
[EXTERN current_thread]

; perform context switch of threads
switch_thread:
    cli

    ; load current thread into eax
    mov eax, [current_thread]
    mov eax, [eax]

    ; save general purpose registers to current thread
    mov [eax+0],  esp
    mov [eax+4],  ebp
    mov [eax+8],  eax
    mov [eax+12], ebx
    mov [eax+16], ecx
    mov [eax+20], edx
    mov [eax+24], esi
    mov [eax+28], edi
    
    ; save eflags to current thread
    pushf
    pop ecx
    mov [eax+32], ecx

    ; load next thread from argument into eax
    mov eax, [esp+4]

    ; load next thread into current thread
    mov [current_thread], eax
    mov eax, [eax]
    
    ; restore general purpose registers of next thread
    mov esp, [eax+0]
    mov ebp, [eax+4]
    mov eax, [eax+8]
    mov ebx, [eax+12]
    mov ecx, [eax+16]
    mov edx, [eax+20]
    mov esi, [eax+24]
    mov edi, [eax+28]
    
    ; restore eflags of next thread
    mov eax, [eax+32]
    push eax
    popf
    
    ; re-enable interrupts
    sti

    ; return to next thread which will be popped from next thread stack
    ret

    