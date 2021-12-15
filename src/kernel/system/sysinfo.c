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
    term_set_cursor(0, 0);
    term_writeln_fg("Napalm OS", COL32_MAGENTA);
    term_writeln_fg("Version 0.2", COL32_DARKGRAY);

    term_write_fg("Bootloader       :       ", COL32_YELLOW);
    term_writeln(mboot_get().bootloader_name);

    term_write_fg("Kernel Memory    :       ", COL32_YELLOW);
    term_printf("0x%8x - 0x%8x\n", kernel_address_start(), kernel_address_end());

    term_write_fg("Kernel Size      :       ", COL32_YELLOW);
    term_printf("%d MB ", kernel_size / 1024 / 1024);
    term_set_cursor_x(34);
    term_set_fg(COL32_GRAY); term_printf("(%d bytes)         \n", kernel_size); term_set_fg(0x0F);

    term_write_fg("RAM Installed    :       ", COL32_YELLOW);;
    term_printf("%d MB ", mm_amount_installed() / 1024 / 1024);
    term_set_cursor_x(34);
    term_set_fg(COL32_GRAY); term_printf("(%d bytes)         \n", mm_amount_installed()); term_set_fg(0x0F);

    term_write_fg("RAM Reserved     :       ", COL32_YELLOW);
    term_printf("%d MB ", mm_amount_reserved() / 1024 / 1024); 
    term_set_cursor_x(34);
    term_set_fg(COL32_GRAY); term_printf("(%d bytes)        \n", mm_amount_installed()); term_set_fg(0x0F);

    term_write_fg("RAM Used         :       ", COL32_YELLOW);
    term_printf("%d MB ", mm_amount_used() / 1024 / 1024); 
    term_set_cursor_x(34);
    term_set_fg(COL32_GRAY); term_printf("(%d bytes)        \n", mm_amount_used()); term_set_fg(0x0F);

    term_write_fg("RAM Free         :       ", COL32_YELLOW);
    term_printf("%d MB ", mm_amount_free() / 1024 / 1024); 
    term_set_cursor_x(34);
    term_set_fg(COL32_GRAY); term_printf("(%d bytes)         \n", mm_amount_free()); term_set_fg(0x0F);

    term_write_fg("CPU Usage        :       ", COL32_YELLOW);
    term_printf("%f", taskmgr_get_cpu_usage());
    term_writeln("%");
}