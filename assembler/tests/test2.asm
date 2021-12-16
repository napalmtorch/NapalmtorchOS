MBOOT_PAGE_ALIGN    equ 1 << 0
MBOOT_MEM_INFO      equ 1 << 1 
MBOOT_VID_INFO      equ 1 << 2
MBOOT_HEADER_MAGIC  equ 0x1BADB002

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
    in EAX, 0x92
    or EAX, 2
    out 0x92, EAX
    ret
