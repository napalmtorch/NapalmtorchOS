#include <kernel/hardware/common/pit.h>

// pit ports
#define PIT_CMD  0x43
#define PIT_DATA 0x40

// current pit frequency
uint32_t pit_freq;

// pit time values
uint32_t pit_millis;
uint32_t pit_millis_total;
uint64_t pit_ticks;
uint32_t pit_secs;

// timer
uint32_t pit_timer;

// initialize programmable interval timer at specified frequency
void pit_init(uint32_t freq)
{
    // set frequency variable
    if (freq == 0) { pit_disable(); }
    if (freq > 5000) { freq = 5000; }
    pit_freq = freq;

    // calculate actual frequency
    uint32_t f = 1193180 / pit_freq;
    uint8_t high = (uint8_t)((f & 0xFF00) >> 8);
    uint8_t low  = (uint8_t)((f & 0x00FF));

    // send frequency to pit
    port_outb(PIT_CMD, 0x36);
    port_outb(PIT_DATA, low);
    port_outb(PIT_DATA, high);
    
    debug_ok("Initialized PIT at %d Hz", pit_freq);
}

// disable programmable interval timer - not yet implemented
void pit_disable() { }

// calculate time using pit interrupt
void pit_calculate()
{
    pit_ticks++;

    pit_timer++;
    if (pit_timer >= pit_freq / 1000)
    {
        pit_millis++;
        pit_millis_total++;
        pit_timer = 0;
    }

    if (pit_millis > 1000) 
    { 
        pit_secs++; 
        pit_millis = 0; 
        pit_timer = 0;
    }
}

// get current pit frequency
uint32_t pit_get_freq() { return pit_freq; }

// get total pit seconds
uint32_t pit_get_seconds_total() { return pit_secs; }

// get current pit milliseconds
uint32_t pit_get_millis() { return pit_millis; }

// get total pit milliseconds
uint32_t pit_get_millis_total() { return pit_millis_total; }

// get total pit ticks
uint64_t pit_get_ticks() { return pit_ticks; }