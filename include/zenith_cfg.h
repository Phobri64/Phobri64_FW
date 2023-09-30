#ifndef ZENITH_CFG_H
#define ZENITH_CFG_H

#define ZTH_WEBUSB_URL "zenithcontrollabs.github.io/Phobri64_GUI"
#define ZTH_MANUFACTURER "Zenith Control Labs"
#define ZTH_PRODUCT "Phobri64"

#define ZTH_PID 0x0001

#define ZENITH_SERIAL_PIN 11

#define ZTH_N64_REMAP_DEFAULT                                                  \
    {                                                                          \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0,  \
            0, 0, 0, 0, 0, 0, 0, 0, 0                                          \
    }

#ifdef HW_PHOBRI_PROTO
#define ZTH_SEPARATE_CAL_READ 1
#endif

#endif // ZENITH_CFG_H