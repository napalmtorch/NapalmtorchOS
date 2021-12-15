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

int idle_main(void* arg);

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

    // initialize vesa driver
    vesa_identify();

    term_init();

    // initialize rtc and pit interrupts
    rtc_init();
    pit_init(2000);
}

void kernel_before_run()
{
    cli();
    // load kernel thread
    thread_kernel   = thread_initial();
    strcpy(thread_kernel->name, "kernel");

    // reset timing information
    kernel_time     = UINT32_MAX;
    kernel_timelast = 0;

    // initialize task manager
    taskmgr_init(thread_kernel);

    // load idle thread
    thread_idle = thread_create("idle", idle_main, 8192, 0xFFFFFFFF);
    taskmgr_ready_thread(thread_idle);

    // initialize keyboard
    kbps2_init();
    kbps2_toggle_term_output(TRUE);
    
    cli_init();
    cli_print_caret();

    // start task manager and enable interrupts
    taskmgr_start();
    sti();
}

void kernel_run()
{
    spinlock_lock(&kernel_lock);
    
    cli_monitor();
    thread_monitor(thread_kernel);

    kernel_time = pit_get_seconds_total();
    if (kernel_time != kernel_timelast)
    {
        kernel_timelast = kernel_time;
        taskmgr_calculate_cpu_usage();
    }
    spinlock_unlock(&kernel_lock);
}

// idle thread method
int idle_main(void* arg)
{
    while (TRUE)
    {
        spinlock_lock(&idle_lock);
        thread_monitor(thread_idle);
        spinlock_unlock(&idle_lock);
    }
    return 0;
}

thread_t* kernel_get_thread() { return thread_kernel; }
thread_t* idle_get_thread() { return thread_idle; }

uint32_t kernel_address_start() { return (uint32_t)&kernel_start; }
uint32_t kernel_address_end() { return (uint32_t)&kernel_end; }
uint32_t kernel_mboot_address_start() { return mboot_start; }