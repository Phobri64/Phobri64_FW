#ifndef ZENITH_CFG_H
#define ZENITH_CFG_H

#define ZTH_WEBUSB_URL "zenithcontrollabs.github.io/Phobri64_GUI"
#define ZTH_MANUFACTURER "Zenith Control Labs"
#define ZTH_PRODUCT "Phobri64"

// TODO: change this to something that you pass to the zenith_init() function
// because not all interfaces will be in use
#define HOJA_SERIAL_PIN 11

#define ZTH_N64_REMAP_DEFAULT                                                  \
    {                                                                          \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0,  \
            0, 0, 0, 0, 0, 0, 0, 0, 0                                          \
    }

#endif // ZENITH_CFG_H