[GLOBAL switch_thread]
[EXTERN taskmgr_current]
[EXTERN taskmgr_next]

;uint32_t esp, ebp, eax, ebx, ecx, edx, esi, edi, eflags;
;uint32_t ds, es, fs, gs;

; perform context switch of threads
switch_thread:
    cli

    ; load current thread into eax
    mov eax, [taskmgr_current]
    ;mov eax, [eax]

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

    ;mov CX, DS
    ;mov [eax+36], ECX

    ;mov CX, ES
    ;mov [eax+40], ECX

    ;mov CX, FS
    ;mov [eax+44], ECX

    ;mov CX, GS
    ;mov [eax+48], ECX

    ; load next thread into current thread
    mov eax, [taskmgr_next]
    
    ;mov ecx, [eax+36]
    ;mov DS, CX

    ;mov ecx, [eax+40]
    ;mov ES, CX

    ;mov ecx, [eax+44]
    ;mov FS, CX

    ;mov ecx, [eax+48]
    ;mov GS, CX

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

    