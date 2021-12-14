#include <kernel/hardware/interrupts/pic.h>
#include <kernel/core/kernel.h>

// pic ports
#define PIC_MASTER_CMD  0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD   0xA0
#define PIC_SLAVE_DATA  0xA1

// wait for pic data to be processed - required for older machines
void pic_wait() { for (uint32_t i = 0; i < 1048576; i++); }

// initialize pic controller
void pic_init()
{
    port_outb(PIC_MASTER_CMD,  0x11); pic_wait();
    port_outb(PIC_SLAVE_CMD,   0x11); pic_wait();
    port_outb(PIC_MASTER_DATA, 0x20); pic_wait();
    port_outb(PIC_SLAVE_DATA,  0x28); pic_wait();
    port_outb(PIC_MASTER_DATA, 0x04); pic_wait();
    port_outb(PIC_SLAVE_DATA,  0x02); pic_wait();
    port_outb(PIC_MASTER_DATA, 0x01); pic_wait();
    port_outb(PIC_SLAVE_DATA,  0x01); pic_wait();
    port_outb(PIC_MASTER_DATA, 0x00); pic_wait();
    port_outb(PIC_SLAVE_DATA,  0x00); pic_wait();
}
