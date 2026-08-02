
#ifndef PLATFORM_TIME_H
#define PLATFORM_TIME_H

#include <stdint.h>

#if defined(PICO_BOARD) || defined(RASPBERRYPI_PICO)
// --- Raspberry Pi Pico Target ---
#include "pico/stdlib.h"

static inline void platform_sleep_ms(uint32_t ms) { sleep_ms(ms); }
#else
// --- Host PC Target (Linux Fedora / POSIX) ---
#if !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 199309L
#endif

#include <time.h>

static inline void platform_sleep_ms(uint32_t ms) {
  struct timespec ts = {.tv_sec = ms / 1000, .tv_nsec = (ms % 1000) * 1000000L};
  nanosleep(&ts, NULL);
}
#endif

#endif
