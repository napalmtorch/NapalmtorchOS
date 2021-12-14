#include <lib/time.h>
#include <kernel/core/kernel.h>

time_t time_create(uint8_t sec, uint8_t min, uint8_t hr, uint8_t day, uint8_t month, uint16_t yr)
{
    time_t time;
    time.second = sec;
    time.minute = min;
    time.hour   = hr;
    time.day    = day;
    time.month  = month;
    time.year   = yr;
    return time;
}

time_ext_t time_create_ext(uint16_t ms, uint8_t sec, uint8_t min, uint8_t hr, uint8_t day, uint8_t week_day, uint8_t month, uint16_t yr)
{
    time_ext_t time;
    time.millisecond = ms;
    time.second      = sec;
    time.minute      = min;
    time.hour        = hr;
    time.week_day    = week_day;
    time.month       = month;
    time.year        = yr;
    return time;
}

uint64_t time_pack(time_t time)
{
    // get high and low of year value
    uint8_t year_upper = (uint8_t)(((time.year) & 0xFF00) >> 8);
    uint8_t year_lower = (uint8_t)(((time.year) & 0x00FF));

    // convert struct values into high and low packed values
    uint64_t packed_upper = (uint64_t)((time.padding << 24) | (time.second << 16) | (time.minute << 8) | time.hour);
    uint64_t packed_lower = (uint64_t)((time.day << 24) | (time.month << 16) | (year_upper << 8) | year_lower);

    // return packed values as 64-bit integer
    return (packed_upper << 32) | packed_lower;
}

time_t time_unpack(uint64_t time)
{
    // create struct
    time_t output;

    // unpack time values from argument
    output.padding = 0x00;
    output.second  = (uint8_t)((time & 0x00FF000000000000) >> 48);
    output.minute  = (uint8_t)((time & 0x0000FF0000000000) >> 40);
    output.hour    = (uint8_t)((time & 0x000000FF00000000) >> 32);
    output.day     = (uint8_t)((time & 0x00000000FF000000) >> 24);
    output.month   = (uint8_t)((time & 0x0000000000FF0000) >> 16);
    output.year    = ((uint8_t)((time & 0x000000000000FF00) >> 8) << 8) | (uint8_t)((time & 0x00000000000000FF));

    // return struct
    return output;
}

char* time_string(time_t time, bool_t military, bool_t secs)
{
// output string
    char* str = tcalloc(128, MEMSTATE_STRING);
    
    // temp vars
    char temp[64];
    memset(temp, 0, 64);

    // 24-hour
    if (military)
    {
        if (time.hour < 10) { strcat(str, "0"); } 
        strcat(str, ltoa(time.hour, temp, 10));
    }
    // 12-hour
    else
    {
        int hour = time.hour;
        if (hour > 12) { hour -= 12; }
        if (time.hour < 10) { strcat(str, "0"); }
        strcat(str, ltoa(hour, temp, 10));
    }
    
    // clear temp and add colon
    memset(temp, 0, 64);
    stradd(str, ':');

    // minute
    if (time.minute < 10) { strcat(str, "0"); }
    strcat(str, ltoa(time.minute, temp, 10));

    // clear temp
    memset(temp, 0, 64);

    // seconds
    if (secs)
    {
        stradd(str, ':');
        if (time.second < 0) { strcat(str, "0"); }
        strcat(str, ltoa(time.second, temp, 10));
    }

    // am/pm
    if (!military)
    { if (time.hour >= 12) { strcat(str, " PM"); } else { strcat(str, " AM"); } }

    // return output string
    return str;
}

char* time_string_date(time_t time, bool_t month_short)
{
    // output string
    char* str = tcalloc(256, MEMSTATE_STRING);

    // temp vars
    char temp[64];
    memset(temp, 0, 64);

    // month
    if (month_short) { strcat(str, time_string_month_short(time.month)); }
    else             { strcat(str, time_string_month(time.month)); }

    // day
    stradd(str, ' ');
    strcat(str, ltoa(time.day, temp, 10));

    // year
    strcat(str, ", ");
    strcat(str, ltoa(time.year, temp, 10));

    // return output string
    return str;
}

char* time_string_timedate(time_t time, bool_t military, bool_t secs, bool_t month_short)
{
    char* time_str = time_string(time, military, secs);
    char* date_str = time_string_date(time, month_short);
    char* output = tcalloc(512, MEMSTATE_STRING);
    strcat(output, time_str);
    strcat(output, " ");
    strcat(output, date_str);
    free(time_str);
    free(date_str);
    return output;
}

const char* time_string_month(uint8_t month)
{
    switch (month)
    {
        case 1:  { return "January"; }
        case 2:  { return "February"; }
        case 3:  { return "March"; }
        case 4:  { return "April"; };
        case 5:  { return "May"; }
        case 6:  { return "June"; }
        case 7:  { return "July"; }
        case 8:  { return "August"; }
        case 9:  { return "September"; }
        case 10: { return "October"; }
        case 11: { return "November"; }
        case 12: { return "December"; }
        default: { return "Error"; }
    }
}

const char* time_string_month_short(uint8_t month)
{
    switch (month)
    {
        case 1:  { return "Jan"; }
        case 2:  { return "Feb"; }
        case 3:  { return "Mar"; }
        case 4:  { return "Apr"; };
        case 5:  { return "May"; }
        case 6:  { return "Jun"; }
        case 7:  { return "Jul"; }
        case 8:  { return "Aug"; }
        case 9:  { return "Sep"; }
        case 10: { return "Oct"; }
        case 11: { return "Nov"; }
        case 12: { return "Dec"; }
        default: { return "Err"; }
    }
}