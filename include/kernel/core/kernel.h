#pragma once
#include <lib/types.h>
#include <lib/memory.h>
#include <lib/string.h>
#include <lib/common.h>
#include <kernel/graphics/color.h>
#include <kernel/core/multiboot.h>
#include <kernel/hardware/interrupts/descriptors.h>
#include <kernel/hardware/interrupts/interrupts.h>
#include <kernel/hardware/interrupts/pic.h>
#include <kernel/hardware/common/ports.h>
#include <kernel/hardware/common/serial.h>
#include <kernel/hardware/common/pit.h>
#include <kernel/hardware/common/rtc.h>
#include <kernel/hardware/devices/video/vga.h>
#include <kernel/system/debug.h>
#include <kernel/system/memmgr.h>
#include <kernel/system/taskmgr.h>
#include <kernel/system/thread.h>
#include <kernel/system/sysinfo.h>

extern uint32_t kernel_size;

void kernel_entry();
void kernel_boot();
void kernel_before_run();
void kernel_run();

uint32_t kernel_address_start();
uint32_t kernel_address_end();
uint32_t kernel_mboot_address_start();