#pragma once
#include <lib/types.h>

#define ATA_SECTOR_SIZE 512

void atapio_init();
bool_t atapio_identify();
bool_t atapio_read(uint64_t sector, uint32_t count, uint8_t* buffer);
bool_t atapio_write(uint64_t sector, uint32_t count, uint8_t* buffer);
bool_t atapio_fill(uint64_t sector, uint32_t count, uint8_t* buffer);