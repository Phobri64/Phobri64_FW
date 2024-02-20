#ifndef ZENITH_CFG_H
#define ZENITH_CFG_H

#define ZTH_WEBUSB_URL "zenithcontrollabs.github.io/Phobri64_GUI"
#define ZTH_MANUFACTURER "Zenith Control Labs"
#define ZTH_PRODUCT "Phobri64"

#define ZTH_PID 0x0001

#include "hw_intf.h"
#define ZENITH_SERIAL_PIN JOYBUS_PIN

#define ZTH_N64_REMAP_DEFAULT                                                  \
    {                                                                          \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0,  \
            0, 0, 0, 0, 0, 0, 0, 0, 0                                          \
    }

#ifdef HW_PHOBRI_PROTO
#define ZTH_SEPARATE_CAL_READ 1
#endif

#define ZTH_STICK_INTERVAL 2000
#endif // ZENITH_CFG_H

typedef struct {
    int16_t hx_x;
    int16_t hx_y;
    int16_t hx_z;
    int16_t hx_temp;
    int16_t hy_x;
    int16_t hy_y;
    int16_t hy_z;
    int16_t hy_temp;
    uint16_t ax;
    uint16_t ay;
} ten_d_analysis_t;

extern ten_d_analysis_t td;
