#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

service_t* cli_service;
cli_cmd_t* cli_list;
uint32_t   cli_count;
uint32_t   cli_count_max;
char**     cli_args;
int        cli_args_count;
bool_t     cli_executing;
bytestream_t cli_instream;

void cli_start(service_t* service)
{
    cli_count_max  = 256;
    cli_count      = 0;
    cli_args       = NULL;
    cli_args_count = 0;
    cli_list       = tcalloc(sizeof(cli_cmd_t) * cli_count_max, MEMSTATE_ARRAY);
    cli_executing  = FALSE;
    cli_instream   = bytestream_create(1024, NULL);

    kbps2_set_stream(cli_instream);

    cli_register_defaults();
}

void cli_stop(service_t* service)
{
    if (cli_list != NULL) { free(cli_list); }
    if (cli_args != NULL) { freearray(cli_args, cli_args_count); }
    cli_count = 0;
}


void cli_init()
{
    cli_service = service_create("CommandLine", SERVICETYPE_INTERFACE, cli_start, cli_stop);
    if (!service_register(cli_service)) { return; }

    cli_service->initialized = TRUE;
    if (!service_start(cli_service)) { return; }
}

int cli_freeindex()
{
    if (cli_list == NULL) { return -1; }
    for (uint32_t i = 0; i < cli_count_max; i++)
    {
        if (cli_list[i].m_execute == NULL) { return i; }
    }
    return -1;
}

void cli_register_defaults()
{
    cli_register(CMD_CLS);
    cli_register(CMD_HELP);
    cli_register(CMD_THREADS);
    cli_register(CMD_KILL);
    cli_register(CMD_CPUINFO);
}

bool_t cli_register(cli_cmd_t cmd)
{
    int i = cli_freeindex();
    if (i < 0 || i >= cli_count_max) { panicf(EXCEPTION_INDEX_OUTOFRANGE, NULL, "cli_register"); return FALSE; }
    cli_list[i].name      = cmd.name;
    cli_list[i].help      = cmd.help;
    cli_list[i].usage     = cmd.usage;
    cli_list[i].m_execute = cmd.m_execute;
    cli_count++;
    debug_info("Registered cli command: %s", cli_list[i].name);
    return TRUE;
}

bool_t cli_unregister(cli_cmd_t cmd)
{

}

bool_t cli_unregister_byname(const char* cmd)
{

}

void cli_print_caret()
{
   vga_write_fg("root", COL4_GREEN);
   vga_write("@");
   vga_write_fg("/", COL4_YELLOW);
   vga_write("> ");
}

void cli_print_help(bool_t usage)
{
    for (uint32_t i = 0; i < cli_count_max; i++)
    {
        if (cli_list[i].m_execute == NULL) { continue; }
       vga_write("- ");
       vga_write_fg(cli_list[i].name, COL4_YELLOW);
       vga_set_cursor_x(24);
        if (usage) {vga_writeln(cli_list[i].usage); } else {vga_writeln(cli_list[i].help); }
    }
}

void cli_print_help_short()
{
    for (uint32_t i = 0; i < cli_count_max; i++)
    {
        if (cli_list[i].m_execute == NULL) { continue; }
        if (vga_get_cursor_x() + strlen(cli_list[i].name) >=vga_get_width()) {vga_newline(); }
       vga_write(cli_list[i].name);
       vga_writechar(0x20);
    }
   vga_newline();
}

void cli_monitor()
{
    if (kbps2_key_down(KEY_ENTER) && !cli_executing)
    {
        cli_executing = TRUE;
        vga_newline();
        cli_execute((char*)cli_instream.data);
        bytestream_clear(&cli_instream);
        cli_print_caret();
    }

    if (kbps2_key_up(KEY_ENTER) && cli_executing) { cli_executing = FALSE; }
}

void cli_enter_pressed(bytestream_t* stream)
{

}

void cli_execute(const char* input)
{
    if (input == NULL) { return; }
    if (strlen(input) == 0) { return; }

    if (cli_args != NULL) { freearray(cli_args, cli_args_count); }
    cli_args = strsplit(input, 0x20, &cli_args_count);

    char* cmd = tcalloc(strlen(input), MEMSTATE_STRING);
    strcpy(cmd, cli_args[0]);
    strupr(cmd);
    
    for (uint32_t i = 0; i < cli_count_max; i++)
    {
        if (cli_list[i].m_execute == NULL) { continue; }
        if (streql(cli_list[i].name, cmd))
        {
            cli_list[i].m_execute(input, cli_args, cli_args_count);
            free(cmd);
            return;
        }
    }

   vga_writeln_fg("Invalid command or file", COL4_RED);
    free(cmd);
}

bytestream_t* cli_get_instream() { return &cli_instream; }
