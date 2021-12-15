#include <kernel/system/cli.h>
#include <kernel/core/kernel.h>

void CMD_METHOD_SERVICES(char* input, char** argv, int argc)
{
    service_printlist();
}