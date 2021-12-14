#pragma once
#include <lib/types.h>
#include <lib/stream.h>
#include <kernel/system/service.h>
#include <kernel/hardware/interrupts/interrupts.h>
#include <kernel/hardware/devices/input/keys.h>

typedef struct
{
    const char* name;
    char        upper_case[60];
    char        lower_case[60];
} PACKED kb_layout_t;

static const kb_layout_t KB_LAYOUT_US = 
{
    "US",
    "??!@#$%^&*()_+??QWERTYUIOP{}??ASDFGHJKL:\"~?|ZXCVBNM<>???? \0",
    "??1234567890-=??qwertyuiop[]??asdfghjkl;'`?\\zxcvbnm,./??? \0",
};

void kbps2_init();
void kbps2_callback(registers_t* regs);
void kbps2_handle_scancode(uint8_t code);
void kbps2_handle_char(char c);

bool_t kbps2_key_down(KEY key);
bool_t kbps2_key_up(KEY key);

void kbps2_set_enter_event(void (*event)(bytestream_t*));
void kbps2_toggle_term_output(bool_t state);
void kbps2_set_stream(bytestream_t stream);
bytestream_t kbps2_get_stream();