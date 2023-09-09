#include "hw/phobri_v1_X.h"
#include "zenith/types.h"
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <stdint.h>

// #define SIGNAL_CIRC_BUF_LEN_BITS 5
// #define SIGNAL_CIRC_BUF_LEN (1 << SIGNAL_CIRC_BUF_LEN_BITS)

// TODO: expand this with a circular buffer,
// storing older data and perhaps other axes
uint16_t _hx_val = 0;
uint16_t _hy_val = 0;

void lis3mdl_setup(uint i2c_addr) {
    // Temp disabled, LP mode w/ FAST_ODR @ 1kHZ
    uint8_t ctrl_reg1_cfg[] = {0x20, 0b00000010};
    // Full scale +/- 4G
    // Not needed for now because matches default
    // uint8_t ctrl_reg2_cfg[] = {0x21, 0b00000000};
    // Continuous measurement
    uint8_t ctrl_reg3_cfg[] = {0x22, 0b00000000};
    // LP mode on Z-axis as well
    // Not needed for now because matches default
    // uint8_t ctrl_reg4_cfg[] = {0x23, 0b00000000};

    i2c_write_blocking(STICK_I2C_INTF, i2c_addr, ctrl_reg1_cfg, 2, false);
    i2c_write_blocking(STICK_I2C_INTF, i2c_addr, ctrl_reg3_cfg, 2, false);
}

void __time_critical_func(lis3mdl_read)(uint16_t *dest, uint i2c_addr) {

    uint8_t regl;
    uint8_t regh;
    uint8_t read_buf[2];

    regl = 0x28;
    regh = 0x29;
    i2c_write_blocking(STICK_I2C_INTF, i2c_addr, &regl, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, i2c_addr, read_buf, 1, false);
    i2c_write_blocking(STICK_I2C_INTF, i2c_addr, &regh, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, i2c_addr, (read_buf + 1), 1, false);
    *dest = (read_buf[1] << 8) + read_buf[0];

    // not using these for now, only reading the X-axis

    // regl = 0x2a;
    // regh = 0x2b;
    // i2c_write_blocking(STICK_I2C_INTF, i2c_addr, &regl, 1, true);
    // i2c_read_blocking(STICK_I2C_INTF, i2c_addr, read_buf, 1, false);
    // i2c_write_blocking(STICK_I2C_INTF, i2c_addr, &regh, 1, true);
    // i2c_read_blocking(STICK_I2C_INTF, i2c_addr, (read_buf + 1), 1, false);
    // a->y = (read_buf[1] << 8) + read_buf[0];

    // regl = 0x2c;
    // regh = 0x2d;
    // i2c_write_blocking(STICK_I2C_INTF, i2c_addr, &regl, 1, true);
    // i2c_read_blocking(STICK_I2C_INTF, i2c_addr, read_buf, 1, false);
    // i2c_write_blocking(STICK_I2C_INTF, i2c_addr, &regh, 1, true);
    // i2c_read_blocking(STICK_I2C_INTF, i2c_addr, (read_buf + 1), 1, false);
    // a->z = (read_buf[1] << 8) + read_buf[0];
}

void __time_critical_func(hx_drdy_isr)(uint gpio, uint32_t events) {
    (void)gpio;
    (void)events;
    lis3mdl_read(&_hx_val, I2C_HX_ADDR);
}

void __time_critical_func(hy_drdy_isr)(uint gpio, uint32_t events) {
    (void)gpio;
    (void)events;
    lis3mdl_read(&_hy_val, I2C_HY_ADDR);
}

inline void phobri_v1_x_3d_core1_init(void) {
    i2c_init(STICK_I2C_INTF, 400 * 1000);
    gpio_set_function(STICK_I2C_SCL, GPIO_FUNC_I2C);
    gpio_set_function(STICK_I2C_SDA, GPIO_FUNC_I2C);

    lis3mdl_setup(I2C_HX_ADDR);
    lis3mdl_setup(I2C_HY_ADDR);

    gpio_set_irq_enabled_with_callback(STICK_HX_DRDY, GPIO_IRQ_EDGE_RISE, true,
                                       &hx_drdy_isr);
    gpio_set_irq_enabled_with_callback(STICK_HY_DRDY, GPIO_IRQ_EDGE_RISE, true,
                                       &hy_drdy_isr);
}

inline void phobri_v1_X_3d_read_analog(analog_data_t *analog_data) {
    analog_data->ax1 = UINT_N_TO_AX(_hx_val, 16);
    analog_data->ax2 = UINT_N_TO_AX(_hy_val, 16);
}