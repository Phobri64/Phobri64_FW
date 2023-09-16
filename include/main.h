#ifndef MAIN_H_
#define MAIN_H_

#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <hardware/watchdog.h>
#include <pico/bootrom.h>
#include <pico/stdlib.h>

// HARDWARE SELECTION
// Select just one
#define HW_PHOBRI_PROTO
// #define HW_PHOBRI_V1_X_ANALOG
// #define HW_PHOBRI_V1_X_3D

#include "hw_intf.h"
#include "zenith.h"

#endif /* MAIN_H_ */