#pragma once
#include <lib/types.h>

void pit_init(uint32_t freq);
void pit_disable();

void pit_calculate();

uint32_t pit_get_freq();
uint32_t pit_get_seconds_total();
uint32_t pit_get_millis();
uint32_t pit_get_millis_total();
uint64_t pit_get_ticks();