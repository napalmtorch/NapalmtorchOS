#pragma once
#include <lib/types.h>
#include <lib/time.h>

void rtc_init();
void rtc_update();
void rtc_read();
time_t rtc_get_time();