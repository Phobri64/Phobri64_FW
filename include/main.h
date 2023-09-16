#ifndef MAIN_H_
#define MAIN_H_

#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <hardware/watchdog.h>
#include <pico/bootrom.h>
#include <pico/stdlib.h>

// HARDWARE SELECTION
// Define just one on the command line, or you can uncomment here
// Otherwise, hw_intf.h selects the default in the else branch
// #define HW_PHOBRI_PROTO
// #define HW_PHOBRI_V1_X_ANALOG
// #define HW_PHOBRI_V1_X_3D

#include "hw_intf.h"
#include "zenith.h"

#endif /* MAIN_H_ */