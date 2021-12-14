#pragma once
#include <lib/types.h>
#include <lib/stream.h>

typedef void (*cli_cmd_exec_t)(char* input, char** argv, int argc);

typedef struct
{
    const char*    name;
    const char*    help;
    const char*    usage;
    cli_cmd_exec_t m_execute;

} PACKED cli_cmd_t;

void CMD_METHOD_CLS(char* input, char** argv, int argc);
void CMD_METHOD_HELP(char* input, char** argv, int argc);
void CMD_METHOD_THREADS(char* input, char** argv, int argc);

static const cli_cmd_t CMD_CLS          = { "CLS", "Clear the screen", "cls [color]", CMD_METHOD_CLS };
static const cli_cmd_t CMD_HELP         = { "HELP", "Show list of commands", "help [-u : usage, -s : shortened]", CMD_METHOD_HELP };
static const cli_cmd_t CMD_THREADS      = { "THREADS", "Show list of threads", "threads", CMD_METHOD_THREADS };

void   cli_init();
void   cli_register_defaults();
void   cli_monitor();
bool_t cli_register(cli_cmd_t cmd);
bool_t cli_unregister(cli_cmd_t cmd);
bool_t cli_unregister_byname(const char* cmd);

void cli_print_caret();
void cli_print_help(bool_t usage);
void cli_print_help_short();
void cli_enter_pressed(bytestream_t* stream);
void cli_execute(const char* input);
bytestream_t* cli_get_instream();