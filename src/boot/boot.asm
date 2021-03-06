[BITS 32]
[GLOBAL mboot]
[GLOBAL start]
[GLOBAL stack_top]
[GLOBAL stack_bottom]
[EXTERN kernel_entry]

MBOOT_PAGE_ALIGN    equ 1 << 0
MBOOT_MEM_INFO      equ 1 << 1 
MBOOT_VID_INFO      equ 1 << 2
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_VID_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

section .text
mboot:
    dd MBOOT_HEADER_MAGIC
    dd MBOOT_HEADER_FLAGS
    dd MBOOT_CHECKSUM   
    dd mboot
    dd 0
    dd 0
    dd 0
    dd start
    dd 0
    dd 800, 600, 32

section .bss
stack_bottom:
    resb 512 * 1024
stack_top:

section .text
start:
    cli
    mov EBP, stack_top          ; set stack base
    mov ESP, EBP                ; set stack pointer
    call enable_a20             ; enable a20 line
    push EBX                    ; push multiboot address to stack
    call kernel_entry           ; jump to kernel entry
    cli                         ; disable interrupts
    hlt                         ; halt the system

enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

[GLOBAL set_gdtr]
set_gdtr:
    push EBP
    mov EBP, ESP

    lgdt [0x800]

    mov ESP, EBP
    pop EBP
    ret

[GLOBAL reload_segments]
reload_segments:
    push EBP
    mov EBP, ESP

    push EAX
    mov AX, 0x10
    mov DS, AX
    mov ES, AX
    mov FS, AX
    mov GS, AX
    mov SS, AX
    pop EAX

    jmp 0x8:reload_segments_end
reload_segments_end:
    mov ESP, EBP
    pop EBP
    ret
    