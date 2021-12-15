#include <kernel/core/kernel.h>

extern uint32_t kernel_start;
extern uint32_t kernel_end;

uint32_t kernel_size;
uint32_t mboot_start;
thread_t* thread_kernel;
thread_t* thread_idle;
uint32_t  kernel_time, kernel_timelast;

spinlock_t kernel_lock;
spinlock_t idle_lock;

int idle_main(thread_t* thread);

// entry point from bootstrap assembly
void kernel_entry(uint32_t* mboot_ptr)
{
    // grab multiboot pointer
    mboot_start = mboot_ptr;
    
    // boot sequence
    kernel_boot();
    kernel_before_run();

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

    // initialize service manager
    service_initmgr();

    // initialize vga interface
    vga_init();

    // initialize rtc and pit interrupts
    rtc_init();
    pit_init(5000);
}

void kernel_before_run()
{
    cli();
    // load kernel thread
    thread_kernel   = thread_initial();

    // reset timing information
    kernel_time     = UINT32_MAX;
    kernel_timelast = 0;

    // initialize task manager
    taskmgr_init(thread_kernel);

    // load idle thread
    thread_idle = thread_create(&idle_main, 8192);

    // initialize keyboard
    kbps2_init();
    kbps2_toggle_vga_output(TRUE);
    
    cli_init();
    cli_print_caret();

    // start task manager and enable interrupts
    taskmgr_start();
    sti();
}

void kernel_run()
{
    cli_monitor();
    thread_monitor(thread_kernel);

    kernel_time = pit_get_seconds_total();
    if (kernel_time != kernel_timelast)
    {
        tlock();
        kernel_timelast = kernel_time;
        taskmgr_calculate_cpu_usage();
        debug_info("KUPDATE");
        //sysinfo_print_info();

        // create and clear strings on stack
        char str_ktps[64];
        char str_itps[64];
        char temp[64];
        memset(str_ktps, 0, 64);
        memset(str_itps, 0, 64);
        memset(temp, 0, 64);

        // build strings
        strcat(str_ktps, "KERNEL TPS: ");
        strcat(str_ktps, ltoa(thread_kernel->time.ticks_per_second, temp, 10));
        strcat(str_itps, "IDLE TPS:   ");
        strcat(str_itps, ltoa(thread_idle->time.ticks_per_second, temp, 10));

        // print string values
        vga_putstr(0, vga_get_height() - 1, "                                    ", COL4_WHITE, COL4_DARKBLUE);
        vga_putstr(0, vga_get_height() - 2, "                                    ", COL4_WHITE, COL4_DARKBLUE);
        vga_putstr(0, vga_get_height() - 1, str_ktps, COL4_WHITE, COL4_DARKBLUE);
        vga_putstr(0, vga_get_height() - 2, str_itps, COL4_WHITE, COL4_DARKBLUE);

        tunlock();
    }
}

// idle thread method
int idle_main(thread_t* thread)
{
    while (TRUE)
    {
        tlock();
        thread_monitor(thread);
        tunlock();
    }
    return 0;
}

thread_t* kernel_get_thread() { return thread_kernel; }
thread_t* idle_get_thread() { return thread_idle; }

uint32_t kernel_address_start() { return (uint32_t)&kernel_start; }
uint32_t kernel_address_end() { return (uint32_t)&kernel_end; }
uint32_t kernel_mboot_address_start() { return mboot_start; }