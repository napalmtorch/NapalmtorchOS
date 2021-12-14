#pragma once
#include <lib/types.h>

const char* sysinfo_get_name();
const char* sysinfo_get_version();
const char* sysinfo_get_dev_status();

void sysinfo_print_info();