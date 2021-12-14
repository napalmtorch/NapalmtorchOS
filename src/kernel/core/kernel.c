#include <kernel/core/kernel.h>

extern uint32_t kernel_start;
extern uint32_t kernel_end;

uint32_t kernel_size;
uint32_t mboot_start;
thread_t* thread_kernel;
thread_t* thread_idle;
uint32_t  kernel_ticks, kernel_time, kernel_timelast, kernel_tps;
uint32_t  idle_tps;

int idle_main(void* arg);

// entry point from bootstrap assembly
void kernel_entry(uint32_t* mboot_ptr)
{
    // grab multiboot pointer
    mboot_start = mboot_ptr;
    
    // boot sequence
    kernel_boot();
    kernel_before_run();
    sysinfo_print_info();

    // main loop
    while (TRUE)
    {
        kernel_run();
    }
}

void kernel_boot()
{
    // setup debugging
    serial_setport(SERIALPORT_COM1);
    debug_setmode(DEBUGMODE_SERIAL);
    debug_ok("Initialized debugging interface");

    // initialize descriptors
    gdt_init();
    idt_init();

    // initialize interrupt services routines
    isr_init();

    // read multiboot and initialize memory manager
    mboot_read();
    mm_init();

    // initialize vga interface
    vga_init();

    // initialize rtc and pit interrupts
    rtc_init();
    pit_init(5000);
}

void kernel_before_run()
{
    // load kernel thread
    thread_kernel   = thread_initial();

    // reset timing information
    kernel_ticks    = 0;
    kernel_time     = UINT32_MAX;
    kernel_timelast = 0;

    // initialize task manager
    taskmgr_init(thread_kernel);

    // load idle thread
    thread_idle = thread_create(&idle_main, 8192, NULL);

    // start task manager and enable interrupts
    taskmgr_start();
    asm volatile("sti");
}

void kernel_run()
{
    kernel_ticks++;
    kernel_time = pit_get_seconds_total();
    if (kernel_time != kernel_timelast)
    {
        tlock();

        // update timing information
        kernel_timelast = kernel_time;
        kernel_tps = kernel_ticks;
        kernel_ticks = 0;

        sysinfo_print_info();

        // create and clear strings on stack
        char str_ktps[64];
        char str_itps[64];
        char temp[64];
        memset(str_ktps, 0, 64);
        memset(str_itps, 0, 64);
        memset(temp, 0, 64);

        // build strings
        strcat(str_ktps, "KERNEL TPS: ");
        strcat(str_ktps, ltoa(kernel_tps, temp, 10));
        strcat(str_itps, "IDLE TPS:   ");
        strcat(str_itps, ltoa(idle_tps, temp, 10));

        // print string values
        vga_putstr(0, vga_get_height() - 1, str_ktps, COL4_WHITE, COL4_DARKBLUE);
        vga_putstr(0, vga_get_height() - 2, str_itps, COL4_WHITE, COL4_DARKBLUE);

        tunlock();
    }
}

// idle thread method
int idle_main(void* args)
{
    idle_tps = 0;

    uint32_t time = 0, last_time = 0, idle_ticks = 0;
    while (TRUE)
    {
        idle_ticks++;
        time = pit_get_seconds_total();
        if (time != last_time)
        {
            last_time  = time;
            idle_tps   = idle_ticks;
            idle_ticks = 0;
        }
    }
    return 0;
}

uint32_t kernel_address_start() { return (uint32_t)&kernel_start; }
uint32_t kernel_address_end() { return (uint32_t)&kernel_end; }
uint32_t kernel_mboot_address_start() { return mboot_start; }