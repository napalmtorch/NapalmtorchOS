#include <kernel/core/multiboot.h>
#include <kernel/core/kernel.h>

// copy of multiboot header
multiboot_t mboot_header;

// copy multiboot header from memory to local structure
void mboot_read()
{
    memcpy(&mboot_header, kernel_mboot_address_start(), sizeof(multiboot_t));
    debug_ok("Bootloader: %s", mboot_get().bootloader_name);
}

// return copy of multiboot header
multiboot_t mboot_get() { return mboot_header; }