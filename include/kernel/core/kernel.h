#pragma once
#include <lib/types.h>
#include <lib/memory.h>
#include <lib/string.h>
#include <lib/common.h>
#include <lib/stream.h>
#include <lib/math.h>
#include <kernel/graphics/color.h>
#include <kernel/core/multiboot.h>
#include <kernel/hardware/interrupts/descriptors.h>
#include <kernel/hardware/interrupts/interrupts.h>
#include <kernel/hardware/interrupts/pic.h>
#include <kernel/hardware/common/ports.h>
#include <kernel/hardware/common/serial.h>
#include <kernel/hardware/common/pit.h>
#include <kernel/hardware/common/rtc.h>
#include <kernel/hardware/common/realmode.h>
#include <kernel/hardware/common/paging.h>
#include <kernel/hardware/devices/video/vga.h>
#include <kernel/hardware/devices/video/vesa.h>
#include <kernel/hardware/devices/input/keyboard_ps2.h>
#include <kernel/hardware/devices/storage/ata_pio.h>
#include <kernel/system/debug.h>
#include <kernel/system/memmgr.h>
#include <kernel/system/taskmgr.h>
#include <kernel/system/thread.h>
#include <kernel/system/sysinfo.h>
#include <kernel/system/syscall.h>
#include <kernel/system/service.h>
#include <kernel/system/terminal.h>
#include <kernel/system/cli.h>
#include <kernel/filesys/filesys.h>
#include <kernel/filesys/vfs.h>
#include <kernel/runtime/elf.h>

extern uint32_t kernel_size;

void kernel_entry();
void kernel_boot();
void kernel_before_run();
void kernel_run();

thread_t* kernel_get_thread();
thread_t* idle_get_thread();

uint32_t kernel_address_start();
uint32_t kernel_address_end();
uint32_t kernel_mboot_address_start();