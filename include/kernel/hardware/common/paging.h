#pragma once
#include <lib/types.h>

void paging_init();
void paging_map(uint32_t v_addr, uint32_t p_addr);
void paging_unmap(uint32_t v_addr);