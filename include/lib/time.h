#pragma once
#include <lib/types.h>

typedef struct
{
    uint8_t  padding;           // unused
    uint8_t  second;            // max = 60
    uint8_t  minute;            // max = 60
    uint8_t  hour;              // max = 24
    uint8_t  day;               // max = 31
    uint8_t  month;             // max = 12
    uint16_t year;              // max = 65536
} PACKED time_t;

typedef struct
{
    uint16_t millisecond;       // max = 1000
    uint8_t  second;            // max = 60
    uint8_t  minute;            // max = 60
    uint8_t  hour;              // max = 24
    uint8_t  day;               // max = 31
    uint8_t  week_day;          // max = 6
    uint8_t  month;             // max = 12
    uint16_t year;              // max = 65536
} PACKED time_ext_t;

time_t      time_create(uint8_t sec, uint8_t min, uint8_t hr, uint8_t day, uint8_t month, uint16_t yr);
time_ext_t  time_create_ext(uint16_t ms, uint8_t sec, uint8_t min, uint8_t hr, uint8_t day, uint8_t week_day, uint8_t month, uint16_t yr);
uint64_t    time_pack(time_t time);
time_t      time_unpack(uint64_t time);
char*       time_string(time_t time, bool_t military, bool_t secs);
char*       time_string_date(time_t time, bool_t month_short);
char*       time_string_timedate(time_t time, bool_t military, bool_t secs, bool_t month_short);
const char* time_string_month(uint8_t month);
const char* time_string_month_short(uint8_t month);