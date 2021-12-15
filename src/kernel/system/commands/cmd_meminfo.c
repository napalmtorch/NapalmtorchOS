#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_MEMINFO(char* input, char** argv, int argc)
{
    term_write_fg("Kernel Memory    :       ", COL32_YELLOW);
    term_printf("0x%8x - 0x%8x\n", kernel_address_start(), kernel_address_end());

    term_write_fg("Kernel Size      :       ", COL32_YELLOW);
    term_printf("%d MB ", kernel_size / 1024 / 1024);
    term_set_cursor_x(34);
    term_set_fg(COL32_GRAY); term_printf("(%d bytes)         \n", kernel_size); term_set_fg(COL32_WHITE);

    term_write_fg("RAM Installed    :       ", COL32_YELLOW);;
    term_printf("%d MB ", mm_amount_installed() / 1024 / 1024);
    term_set_cursor_x(34);
    term_set_fg(COL32_GRAY); term_printf("(%d bytes)         \n", mm_amount_installed()); term_set_fg(COL32_WHITE);

    term_write_fg("RAM Reserved     :       ", COL32_YELLOW);
    term_printf("%d MB ", mm_amount_reserved() / 1024 / 1024); 
    term_set_cursor_x(34);
    term_set_fg(COL32_GRAY); term_printf("(%d bytes)        \n", mm_amount_installed()); term_set_fg(COL32_WHITE);

    term_write_fg("RAM Used         :       ", COL32_YELLOW);
    term_printf("%d MB ", mm_amount_used() / 1024 / 1024); 
    term_set_cursor_x(34);
    term_set_fg(COL32_GRAY); term_printf("(%d bytes)        \n", mm_amount_used()); term_set_fg(COL32_WHITE);

    term_write_fg("RAM Free         :       ", COL32_YELLOW);
    term_printf("%d MB ", mm_amount_free() / 1024 / 1024); 
    term_set_cursor_x(34);
    term_set_fg(COL32_GRAY); term_printf("(%d bytes)         \n", mm_amount_free()); term_set_fg(COL32_WHITE);
}