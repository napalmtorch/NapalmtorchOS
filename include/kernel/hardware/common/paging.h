#pragma once
#include <lib/types.h>

void paging_init();
void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys);