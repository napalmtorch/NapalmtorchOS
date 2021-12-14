#include <kernel/system/sysinfo.h>
#include <kernel/core/kernel.h>

const char* OS_NAME         = "Napalm OS";
const char* OS_VERSION      = "Version 0.3";
const char* OS_DEV_STATUS   = "Alpha";

const char* sysinfo_get_name() { return OS_NAME; }
const char* sysinfo_get_version() { return OS_VERSION; }
const char* sysinfo_get_dev_status() { return OS_DEV_STATUS; }

void sysinfo_print_info()
{
    vga_set_cursor(0, 0);
    vga_writeln_fg("Napalm OS", COL4_MAGENTA);
    vga_writeln_fg("Version 0.2", COL4_DARKGRAY);

    vga_write_fg("Bootloader       :       ", COL4_YELLOW);
    vga_writeln(mboot_get().bootloader_name);

    vga_write_fg("Kernel Memory    :       ", COL4_YELLOW);
    vga_printf("0x%8x - 0x%8x\n", kernel_address_start(), kernel_address_end());

    vga_write_fg("Kernel Size      :       ", COL4_YELLOW);
    vga_printf("%d MB ", kernel_size / 1024 / 1024);
    vga_set_cursor_x(34);
    vga_set_fg(COL4_GRAY); vga_printf("(%d bytes)         \n", kernel_size); vga_set_fg(0x0F);

    vga_write_fg("RAM Installed    :       ", COL4_YELLOW);;
    vga_printf("%d MB ", mm_amount_installed() / 1024 / 1024);
    vga_set_cursor_x(34);
    vga_set_fg(COL4_GRAY); vga_printf("(%d bytes)         \n", mm_amount_installed()); vga_set_fg(0x0F);

    vga_write_fg("RAM Reserved     :       ", COL4_YELLOW);
    vga_printf("%d MB ", mm_amount_reserved() / 1024 / 1024); 
    vga_set_cursor_x(34);
    vga_set_fg(COL4_GRAY); vga_printf("(%d bytes)        \n", mm_amount_installed()); vga_set_fg(0x0F);

    vga_write_fg("RAM Used         :       ", COL4_YELLOW);
    vga_printf("%d MB ", mm_amount_used() / 1024 / 1024); 
    vga_set_cursor_x(34);
    vga_set_fg(COL4_GRAY); vga_printf("(%d bytes)        \n", mm_amount_used()); vga_set_fg(0x0F);

    vga_write_fg("RAM Free         :       ", COL4_YELLOW);
    vga_printf("%d MB ", mm_amount_free() / 1024 / 1024); 
    vga_set_cursor_x(34);
    vga_set_fg(COL4_GRAY); vga_printf("(%d bytes)         \n", mm_amount_free()); vga_set_fg(0x0F);
}