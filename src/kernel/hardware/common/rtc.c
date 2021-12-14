#include <kernel/hardware/common/rtc.h>
#include <kernel/core/kernel.h>

// rtc ports
#define RTC_PORT_CMD  0x70
#define RTC_PORT_DATA 0x71

// rtc information
uint32_t rtc_tick;
uint32_t rtc_freq = 1024;
time_t   rtc_time;

// local methods
void    rtc_set_reg(uint16_t reg, uint8_t data);
uint8_t rtc_get_reg(uint16_t reg);
bool_t  rtc_is_updating();

// rtc interrupt callback
void rtc_callback(registers_t* regs)
{
    rtc_update();
    port_outb(0x70, 0x0C);	// select register C
    port_inb(0x71);		// just throw away contents
}

// initialize real-time clock interface
void rtc_init()
{
    // reset variables
    rtc_tick = 0;
    rtc_freq = 1024;
    memset(&rtc_time, 0, sizeof(time_t));

    // disable interrupts
    cli();

    // send initialization data to ports
    port_outb(0x70, 0x8B);		    // select register B, and disable NMI
    char prev = port_inb(0x71);	    // read the current value of register B
    port_outb(0x70, 0x8B);		    // set the index again (a read will reset the index to register D)
    port_outb(0x71, prev | 0x40);	// write the previous value ORed with 0x40. This turns on bit 6 of register B

    port_outb(0x70, 0x0C);      	// select register C
    (void)port_inb(0x71);           // dispose contents

    // register interrupts and re-enable
    irq_register(IRQ8, rtc_callback);
    sti();

    // finished
    rtc_read();
    debug_ok("Initialized RTC controller");
}

// read rtc information twice per tick rate
void rtc_update()
{
    rtc_tick++;
    if (rtc_tick >= rtc_freq / 2)
    {
        rtc_read();
        rtc_tick = 0;
    }
}

// read rtc information and store locally
void rtc_read()
{
    // read data from registers
    rtc_time.second = rtc_get_reg(0x00);
    rtc_time.minute = rtc_get_reg(0x02);
    rtc_time.hour   = rtc_get_reg(0x04);
    rtc_time.day    = rtc_get_reg(0x07);
    rtc_time.month  = rtc_get_reg(0x08);
    rtc_time.year   = rtc_get_reg(0x09);

    // check if data is bcd
    uint8_t bcd = rtc_get_reg(0x0B);

    // decode bcd formatted data
    if (!(bcd & 0x04))
    {
        rtc_time.second  = (rtc_time.second & 0x0F) + (rtc_time.second / 16) * 10;
        rtc_time.minute  = (rtc_time.minute & 0x0F) + (rtc_time.minute / 16) * 10;
        rtc_time.hour    = ((rtc_time.hour & 0x0F)  + (rtc_time.hour / 16) * 10) | (rtc_time.hour & 0x80);
        rtc_time.day     = (rtc_time.day & 0x0F)    + (rtc_time.day / 16) * 10;
        rtc_time.month   = (rtc_time.month & 0x0F)  + (rtc_time.month / 16) * 10;
        rtc_time.year    = (rtc_time.year & 0x0F)   + (rtc_time.year / 16) * 10;
        rtc_time.year    += 2000;
    }
}

// return structure containing time information
time_t rtc_get_time()
{
    time_t output;
    memcpy(&output, &rtc_time, sizeof(time_t));
    return output;
}

// set specified rtc register
void rtc_set_reg(uint16_t reg, uint8_t data)
{
    if (!rtc_is_updating())
    {
        cli();
        port_outb(RTC_PORT_CMD, reg);
        port_outb(RTC_PORT_DATA, data);
        sti();
    }
}

// get data from specified rtc register
uint8_t rtc_get_reg(uint16_t reg)
{
    cli();
    port_outb(RTC_PORT_CMD, reg);
    asm volatile("sti");
    return port_inb(RTC_PORT_DATA);
}

// check if rtc is able to be modified
bool_t rtc_is_updating()
{
    port_outb(RTC_PORT_CMD, 0x0A);
    uint8_t status = port_inb(RTC_PORT_DATA);
    return status & 0x80;
}