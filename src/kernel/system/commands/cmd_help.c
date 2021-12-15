#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_HELP(char* input, char** argv, int argc)
{
    if (argc == 1) { cli_print_help(FALSE); return; }
    if (argc >= 2)
    {
        if (streql(argv[1], "-u")) { cli_print_help(TRUE); return; }
        if (streql(argv[1], "-s")) { cli_print_help_short(); return; }
    }

   term_writeln_fg("Invalid arguments", COL32_RED);
}