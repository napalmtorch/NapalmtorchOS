#pragma once
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

#define UNUSED(x) void(x)
#define PACKED __attribute__((packed))

typedef enum { FALSE, TRUE } bool_t;

static inline int sleepms(uint32_t ms) { return usleep(ms * 1000); }