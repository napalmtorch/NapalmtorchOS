#include <kernel/hardware/devices/input/keyboard_ps2.h>
#include <kernel/core/kernel.h>

service_t*      kbps2_service;
bytestream_t    kbps2_stream;
uint8_t         kbps2_scancode;
bool_t          kbps2_vga_output;
void            (*kbps2_enter_event)(bytestream_t* stream);
uint8_t*        kbps2_keymap;
spinlock_t      kbps2_lock;

bool_t kbps2_lshift, kbps2_rshift, kbps2_caps;
bool_t kbps2_lctrl;
bool_t kbps2_lalt;
bool_t kbps2_backspace, kbps2_enter;

void kbps2_keymap_clear();
void kbps2_keymap_update();

void kbps2_start(service_t* service)
{
    kbps2_scancode    = 0;
    kbps2_keymap      = tcalloc(256, MEMSTATE_ARRAY);
    kbps2_stream.size = 0;
    kbps2_stream.data = NULL;
    kbps2_keymap_clear();
    irq_register(IRQ1, kbps2_callback);
}

void kbps2_stop(service_t* service)
{
    if (kbps2_keymap != NULL) { free(kbps2_keymap); }
}

void kbps2_init()
{
    kbps2_service = service_create("PS2Keyboard", SERVICETYPE_DRIVER, kbps2_start, kbps2_stop);
    if (!service_register(kbps2_service)) { return; }

    kbps2_service->initialized = TRUE;
    if (!service_start(kbps2_service)) { return; }
}

void kbps2_callback(registers_t* regs)
{
    tlock();

    // get status
    uint8_t status = port_inb(0x64);

    // validate status
    if (status & 0b00000001 == 0) { return; }
    if (status & 0b00000010 == 1) { return; }
    if (status & 0b00001000 == 1) { return; }
    
    // read and handle data
    kbps2_scancode = port_inb(0x60);
    kbps2_handle_scancode(kbps2_scancode);

    tunlock();
}

void kbps2_handle_scancode(uint8_t code)
{
    char c = 0;

    kbps2_keymap_update();

    switch (code)
    {
        case KEYDN_BACKSPACE: { kbps2_backspace = TRUE;  break; }
        case KEYUP_BACKSPACE: { kbps2_backspace = FALSE; break; }
        case KEYDN_CAPS_LOCK: { kbps2_caps      = TRUE;  break; }
        case KEYUP_CAPS_LOCK: { kbps2_caps      = FALSE; break; }
        case KEYDN_ENTER:     
        { 
            if (kbps2_enter_event != NULL) { kbps2_enter_event(&kbps2_stream); }
            kbps2_enter = TRUE;  
            break; 
        }
        case KEYUP_ENTER:     { kbps2_enter     = FALSE; break; }
        case KEYDN_LSHIFT:    { kbps2_lshift    = TRUE;  break; }
        case KEYUP_LSHIFT:    { kbps2_lshift    = FALSE; break; }
        case KEYDN_RSHIFT:    { kbps2_rshift    = TRUE;  break; }
        case KEYUP_RSHIFT:    { kbps2_rshift    = FALSE; break; }
        case KEYDN_LCTRL:     { kbps2_lctrl     = TRUE;  break; }
        case KEYUP_LCTRL:     { kbps2_lctrl     = FALSE; break; }
        case KEYDN_LALT:      { kbps2_lalt      = TRUE;  break; }
        case KEYUP_LALT:      { kbps2_lalt      = FALSE; break; }
        default: 
        { 
            bool_t caps = (((kbps2_lshift || kbps2_rshift) && !kbps2_caps) || ((!kbps2_lshift && !kbps2_rshift) && kbps2_caps));
            if (caps) { c = KB_LAYOUT_US.upper_case[code]; }
            else      { c = KB_LAYOUT_US.lower_case[code]; }
            if ((code != KEYDN_SLASH && c == '?') || code >= 58) { c = 0; }
            break; 
        }
    }

    kbps2_handle_char(c);
}

void kbps2_handle_char(char c)
{
    if (kbps2_stream.data == NULL) { panicf(EXCEPTION_ARG_NULL, NULL, "kbps2_handle_char"); return; }

    if (kbps2_backspace)
    {
        if (strlen(kbps2_stream.data) > 0)
        {
            kbps2_stream.data[strlen(kbps2_stream.data) - 1] = 0;
            if (kbps2_vga_output) { vga_delete(); }
        }
        kbps2_backspace = FALSE;
    }
    else if (c >= 32 && c <= 126)
    {
        uint32_t len = strlen(kbps2_stream.data);
        kbps2_stream.data[len] = c;
        kbps2_stream.data[len + 1] = 0;
        if (kbps2_vga_output) { vga_writechar(c); }
    }
}

bool_t kbps2_key_down(KEY key)
{
    return kbps2_keymap[(int)key];
}

bool_t kbps2_key_up(KEY key)
{
    return kbps2_keymap[(int)key] == FALSE ? TRUE : FALSE;
}

void kbps2_keymap_clear()
{
    memset(kbps2_keymap, 0, 256);
}

void kbps2_keymap_update()
{
    if (kbps2_scancode <= 0x58) { kbps2_keymap[(int)kbps2_scancode] = TRUE; }
    else if (kbps2_scancode >= 0x81 && kbps2_scancode < 0xD8) { kbps2_keymap[(int)kbps2_scancode - 128] = FALSE; }
}

void kbps2_set_enter_event(void (*event)(bytestream_t*)) { kbps2_enter_event = event; }

void kbps2_toggle_vga_output(bool_t state) { kbps2_vga_output = state; }

void kbps2_set_stream(bytestream_t stream) 
{ 
    kbps2_stream = stream; 
}

bytestream_t kbps2_get_stream() { return kbps2_stream; }