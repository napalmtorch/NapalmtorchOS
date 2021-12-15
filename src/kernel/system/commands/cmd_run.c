#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

static const uint8_t prog_data[] = 
{
    0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x00,
    0xFE,
};

void CMD_METHOD_RUN(char* input, char** argv, int argc)
{
    executable_t program;
    program.data = prog_data;
    program.total_size = sizeof(prog_data);
    program.code_address = 0;
    program.data_address = 0;
    program.code_size = sizeof(prog_data);
    program.data_size = 0;
    program.filename = NULL;

    runtime_t* runtime = runtime_create("vm_test", program);
    runtime_start(runtime);
}