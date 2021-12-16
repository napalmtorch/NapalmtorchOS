%include "stdlib.asm"

; virtual machine system calls
SYSCALL_PRINT equ 0x01
SYSCALL_EXIT  equ 0xFF


; a test message
message dd "Hello world", 0x0A, 0x00

; stack top
stack_top equ 0xF0000000

start:
    mov ESP, stack_top
    mov EBP, ESP
    call print_string
    
print_string:
    mov ESI, message
    syscall SYSCALL_PRINT
    ret
    