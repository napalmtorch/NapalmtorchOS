#include <kernel/hardware/devices/video/vesa.h>
#include <kernel/core/kernel.h>

#define LNG_PTR(seg, off) ((seg << 4) | off)
#define REAL_PTR(arr) LNG_PTR(arr[1], arr[0])
#define SEG(addr) (((uint32_t)addr >> 4) & 0xF000)
#define OFF(addr) ((uint32_t)addr & 0xFFFF)

service_t*       vesa_service;
vesa_mode_info_t vesa_mode_info;
vesa_info_t      vesa_info;
uint32_t         vesa_width, vesa_height;
uint32_t*        vesa_buffer;
uint32_t         vesa_buffer_size;

void vesa_start(service_t* service)
{
    vesa_probe();
}

void vesa_stop(service_t* service)
{

}

void vesa_probe()
{
    // copy data from memory into structure
    vesa_mode_info_t* ptr = (vesa_mode_info_t*)mboot_get().vbe_mode_info;
    memcpy(&vesa_mode_info, ptr, sizeof(vesa_mode_info_t));

    vesa_info_t* info_ptr = (vesa_info_t*)0x80000;
    memcpy(&vesa_info, info_ptr, sizeof(vesa_info_t));

    paging_map_virtual_to_phys(32 * 1024 * 1024, vesa_mode_info.physical_base);

    //registers16_t regs;
    //regs.AX = 0x4F00;
    //regs.ES = 0x8000;
    //regs.DI = 0x0000;
    //_int_16(0x10, &regs);

    // set properties
    vesa_width       = vesa_mode_info.width;
    vesa_height      = vesa_mode_info.height;
    vesa_buffer      = 32 * 1024 * 1024; //(uint32_t*)vesa_mode_info.physical_base;
    vesa_buffer_size = vesa_width * vesa_height * 4;
}

void vesa_identify()
{
    vesa_service = service_create("VESA", SERVICETYPE_DRIVER, vesa_start, vesa_stop);
    if (!service_register(vesa_service)) { return; }

    vesa_service->initialized = TRUE;
    if (!service_start(vesa_service)) { return; }
}

bool_t vesa_setmode(int w, int h)
{
    if (!vesa_service->started) { return FALSE; }
                
    // populate info block
    vesa_probe();
    
    // data declarations
    registers16_t regs;
    vesa_mode_info_t* info = (vesa_mode_info_t*)mboot_get().vbe_mode_info;
    uint16_t* modes = (uint16_t*)REAL_PTR(vesa_info.video_mode);

    uint16_t mode;
    for (int i = 0; modes[i] != 0xFFFF; i++)
    {
        // set up interrupt to get mode info
        regs.AX = 0x4F01;
        regs.CX = modes[i];
        regs.ES = SEG(info);
        regs.DI = OFF(info);

        // call interrupt
        _int_16(0x10, &regs);

        // check if mode is valid
        if (info->width == w && info->height == h && info->depth == 32)
        {
            mode = modes[i];
            
            size_t size = (info->height * info->pitch);
    

            vesa_width  = info->width;
            vesa_height = info->height;
            vesa_buffer = info->physical_base;
            vesa_buffer_size = size;

            regs.AX = 0x4F02;
            regs.BX = mode | 0x4000;
            _int_16(0x10, &regs);

            memcpy(&vesa_mode_info, info, sizeof(vesa_mode_info_t));
            debug_info("Set VESA mode to %dx%dx%d", vesa_width, vesa_height, 32);
            return TRUE;
        }
    }
    return FALSE;
}

bool_t vesa_ismode_valid(int w, int h)
{
    return FALSE;
}

void vesa_clear(uint32_t color)
{
    uint32_t dest = (uint32_t)vesa_buffer;
    uint32_t num_dwords = vesa_buffer_size / 4;
    uint32_t num_bytes  = vesa_buffer_size % 4;
    uint32_t *dest32 = (uint32_t*)dest;
    uint8_t  *dest8 = ((uint8_t*)dest)+num_dwords*4;
    uint8_t  val8 = (uint8_t)color;
    uint32_t i;
    for (i=0;i<num_dwords;i++) { dest32[i] = color; }
    for (i=0;i<num_bytes;i++)  { dest8[i] = val8; }
}

void vesa_setpixel(int x, int y, uint32_t color)
{
    vesa_buffer[y * vesa_width + x] = color;
}

uint32_t vesa_getpixel(int x, int y)
{
    return vesa_buffer[y * vesa_width + x];
}

uint32_t  vesa_get_width() { return vesa_width; }

uint32_t  vesa_get_height() { return vesa_height; }

uint32_t vesa_get_buffer_size() { return vesa_buffer_size; }

uint32_t* vesa_get_buffer() { return vesa_buffer; }