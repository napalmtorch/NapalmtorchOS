#pragma once
#include <lib/types.h>

  typedef struct
{
    char         signature[4];
    uint16_t     version;
    uint16_t     oem_string[2];
    uint8_t      capabilities[4];
    uint16_t     video_mode[2];
    uint16_t     total_memory;
} PACKED vesa_info_t;

// mode info block
typedef struct
{
    uint16_t     attributes;
    uint8_t      window_a, Window_b;
    uint16_t     granularity;
    uint16_t     window_size;
    uint16_t     segment_a, Segment_b;
    uint16_t     window_func[2];
    uint16_t     pitch, width, height;
    uint8_t      char_width, char_height, planes, depth, banks;
    uint8_t      memory_model, bank_size, image_pages;
    uint8_t      reserved0;
    uint8_t      red_mask, red_position;
    uint8_t      green_mask, green_position;
    uint8_t      blue_mask, blue_position;
    uint8_t      rsv_mask, rsv_position;
    uint8_t      direct_color;
    uint32_t     physical_base;
    uint32_t     reserved1;
    uint16_t     reserved2;
} PACKED vesa_mode_info_t;

void   vesa_identify();
bool_t vesa_setmode(int w, int h);
bool_t vesa_ismode_valid(int w, int h);

void     vesa_clear(uint32_t color);
void     vesa_setpixel(int x, int y, uint32_t color);
uint32_t vesa_getpixel(int x, int y);

uint32_t  vesa_get_width();
uint32_t  vesa_get_height();
uint32_t  vesa_get_buffer_size();
uint32_t* vesa_get_buffer();