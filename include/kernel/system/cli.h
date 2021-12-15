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
void CMD_METHOD_SERVICES(char* input, char** argv, int argc);
void CMD_METHOD_THREADS(char* input, char** argv, int argc);
void CMD_METHOD_TESTTHREAD(char* input, char** argv, int argc);
void CMD_METHOD_KILL(char* input, char** argv, int argc);
void CMD_METHOD_CPUINFO(char* input, char** argv, int argc);
void CMD_METHOD_MEMINFO(char* input, char** argv, int argc);
void CMD_METHOD_HEAP(char* input, char** argv, int argc);
void CMD_METHOD_RUN(char* input, char** argv, int argc);

void CMD_METHOD_LS(char* input, char** argv, int argc);
void CMD_METHOD_VIEW(char* input, char** argv, int argc);

static const cli_cmd_t CMD_CLS          = { "CLS", "Clear the screen", "cls [color]", CMD_METHOD_CLS };
static const cli_cmd_t CMD_HELP         = { "HELP", "Show list of commands", "help [-u : usage, -s : shortened]", CMD_METHOD_HELP };
static const cli_cmd_t CMD_THREADS      = { "THREADS", "Show list of threads", "threads", CMD_METHOD_THREADS };
static const cli_cmd_t CMD_SERVICES     = { "SERVICES", "Show list of services", "services", CMD_METHOD_SERVICES };
static const cli_cmd_t CMD_TESTTHREAD   = { "TESTTHREAD", "Start a test thread", "testthread", CMD_METHOD_TESTTHREAD };
static const cli_cmd_t CMD_KILL         = { "KILL", "Terminate thread with specified id", "kill [id]", CMD_METHOD_KILL };
static const cli_cmd_t CMD_CPUINFO      = { "CPUINFO", "Show processor information", "cpuinfo", CMD_METHOD_CPUINFO };
static const cli_cmd_t CMD_MEMINFO      = { "MEMINFO", "Show memory information", "meminfo", CMD_METHOD_MEMINFO };
static const cli_cmd_t CMD_HEAP         = { "HEAP", "Show list of heap allocations", "heap", CMD_METHOD_HEAP };
static const cli_cmd_t CMD_RUN          = { "RUN", "Load an executable program", "run [path] [args]", CMD_METHOD_RUN };

static const cli_cmd_t CMD_LS           = { "LS", "List contents of specified directory", "ls [path]", CMD_METHOD_LS };
static const cli_cmd_t CMD_VIEW         = { "VIEW", "View contents of specified file", "view [-t : text, -b : binary] [path]", CMD_METHOD_VIEW };
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