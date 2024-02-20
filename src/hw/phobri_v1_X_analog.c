#include "hw/phobri_v1_X.h"
#include "zenith/types.h"
#include "zenith/utilities/running_avg.h"
#include "zenith_cfg.h"
#include <hardware/gpio.h>

#define CALIB_AVG_LEN_BITS 3
#define NORMAL_AVG_LEN_BITS 0

ten_d_analysis_t td;

void hybrid_lis3mdl_setup(uint i2c_addr) {
    // Temp disabled, LP mode w/ FAST_ODR @ 1kHZ
    uint8_t ctrl_reg1_cfg[] = {0x20, 0b10000010};
    // Full scale +/- 4G
    // Not needed for now because matches default
    uint8_t ctrl_reg2_cfg[] = {0x21, 0b01100000};
    // Continuous measurement
    uint8_t ctrl_reg3_cfg[] = {0x22, 0b00000000};
    // LP mode on Z-axis as well
    // Not needed for now because matches default
    // uint8_t ctrl_reg4_cfg[] = {0x23, 0b00000000};

    i2c_write_blocking(STICK_I2C_INTF, i2c_addr, ctrl_reg1_cfg, 2, false);
    i2c_write_blocking(STICK_I2C_INTF, i2c_addr, ctrl_reg2_cfg, 2, false);
    i2c_write_blocking(STICK_I2C_INTF, i2c_addr, ctrl_reg3_cfg, 2, false);
}

void __time_critical_func(lis3mdl_xy_read)() {
    uint8_t regl;
    uint8_t regh;
    uint8_t read_buf[2];
    int16_t xval;
    int16_t yval;
    int16_t zval;
    int16_t temp;

    regl = 0x28;
    regh = 0x29;
    i2c_write_blocking(STICK_I2C_INTF, I2C_HX_ADDR, &regl, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HX_ADDR, read_buf, 1, false);
    i2c_write_blocking(STICK_I2C_INTF, I2C_HX_ADDR, &regh, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HX_ADDR, (read_buf + 1), 1, false);
    xval = (read_buf[1] << 8) + read_buf[0];

    regl = 0x2a;
    regh = 0x2b;
    i2c_write_blocking(STICK_I2C_INTF, I2C_HX_ADDR, &regl, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HX_ADDR, read_buf, 1, false);
    i2c_write_blocking(STICK_I2C_INTF, I2C_HX_ADDR, &regh, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HX_ADDR, (read_buf + 1), 1, false);
    yval = (read_buf[1] << 8) + read_buf[0];

    regl = 0x2c;
    regh = 0x2d;
    i2c_write_blocking(STICK_I2C_INTF, I2C_HX_ADDR, &regl, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HX_ADDR, read_buf, 1, false);
    i2c_write_blocking(STICK_I2C_INTF, I2C_HX_ADDR, &regh, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HX_ADDR, (read_buf + 1), 1, false);
    zval = (read_buf[1] << 8) + read_buf[0];

    regl = 0x2E;
    regh = 0x2F;
    i2c_write_blocking(STICK_I2C_INTF, I2C_HX_ADDR, &regl, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HX_ADDR, read_buf, 1, false);
    i2c_write_blocking(STICK_I2C_INTF, I2C_HX_ADDR, &regh, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HX_ADDR, (read_buf + 1), 1, false);
    temp = (read_buf[1] << 8) + read_buf[0];

    td.hx_x = xval;
    td.hx_y = yval;
    td.hx_z = zval;
    td.hx_temp = temp;

    regl = 0x28;
    regh = 0x29;
    i2c_write_blocking(STICK_I2C_INTF, I2C_HY_ADDR, &regl, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HY_ADDR, read_buf, 1, false);
    i2c_write_blocking(STICK_I2C_INTF, I2C_HY_ADDR, &regh, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HY_ADDR, (read_buf + 1), 1, false);
    xval = (read_buf[1] << 8) + read_buf[0];

    regl = 0x2a;
    regh = 0x2b;
    i2c_write_blocking(STICK_I2C_INTF, I2C_HY_ADDR, &regl, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HY_ADDR, read_buf, 1, false);
    i2c_write_blocking(STICK_I2C_INTF, I2C_HY_ADDR, &regh, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HY_ADDR, (read_buf + 1), 1, false);
    yval = (read_buf[1] << 8) + read_buf[0];

    regl = 0x2c;
    regh = 0x2d;
    i2c_write_blocking(STICK_I2C_INTF, I2C_HY_ADDR, &regl, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HY_ADDR, read_buf, 1, false);
    i2c_write_blocking(STICK_I2C_INTF, I2C_HY_ADDR, &regh, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HY_ADDR, (read_buf + 1), 1, false);
    zval = (read_buf[1] << 8) + read_buf[0];

    regl = 0x2E;
    regh = 0x2F;
    i2c_write_blocking(STICK_I2C_INTF, I2C_HY_ADDR, &regl, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HY_ADDR, read_buf, 1, false);
    i2c_write_blocking(STICK_I2C_INTF, I2C_HY_ADDR, &regh, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, I2C_HY_ADDR, (read_buf + 1), 1, false);
    temp = (read_buf[1] << 8) + read_buf[0];

    td.hy_x = xval;
    td.hy_y = yval;
    td.hy_z = zval;
    td.hy_temp = temp;
}

inline void ads7142_write_reg(uint8_t regaddr, uint8_t regval) {
    uint8_t data[] = {0x8, regaddr, regval};

    i2c_write_blocking(STICK_I2C_INTF, I2C_ADC_ADDR, data, 3, false);
}

void ads7142_setup(void) {
    ads7142_write_reg(0x1F, 0x01);
    ads7142_write_reg(0x15, 0x01);
    ads7142_write_reg(0x24, 0x03);
    ads7142_write_reg(0x1C, 0x07);
    ads7142_write_reg(0x20, 0x03);
    ads7142_write_reg(0x18, 0x00);
    ads7142_write_reg(0x19, 0x15);
    ads7142_write_reg(0x30, 0x0F);
    ads7142_write_reg(0x1E, 0x01);
}

void __time_critical_func(ads7142_isr)(uint gpio, uint32_t events) {
    uint8_t data_buf[4] = {
        0x30,
        0x08,
        0,
        0,
    };

    i2c_write_blocking(STICK_I2C_INTF, I2C_ADC_ADDR, data_buf, 2, false);
    i2c_read_blocking(STICK_I2C_INTF, I2C_ADC_ADDR, data_buf, 4, false);
    td.ax = ((data_buf[3] << 8) + data_buf[2]);
    td.ay = ((data_buf[1] << 8) + data_buf[0]);
    ads7142_write_reg(0x1E, 0x01);
}

void phobri_v1_x_analog_core1_init(void) {
    gpio_init(STICK_HX_DRDY);
    gpio_init(STICK_HY_DRDY);
    gpio_init(STICK_ADC_DRDY_N);
    gpio_set_dir(STICK_HX_DRDY, GPIO_IN);
    gpio_set_dir(STICK_HY_DRDY, GPIO_IN);
    gpio_set_dir(STICK_ADC_DRDY_N, GPIO_IN);

    i2c_init(STICK_I2C_INTF, 400 * 1000);
    gpio_set_function(STICK_I2C_SCL, GPIO_FUNC_I2C);
    gpio_set_function(STICK_I2C_SDA, GPIO_FUNC_I2C);

    ads7142_setup();
    hybrid_lis3mdl_setup(I2C_HX_ADDR);
    hybrid_lis3mdl_setup(I2C_HY_ADDR);

    // gpio_set_irq_enabled_with_callback(STICK_ADC_DRDY_N, GPIO_IRQ_EDGE_FALL,
    //                                   true, &ads7142_isr);
}

void phobri_v1_x_analog_read_analog(analog_data_t *analog_data) {
    ads7142_isr(0, 0);
    lis3mdl_xy_read();

    analog_data->ax1 = UINT_N_TO_AX(td.ax, 16);
    analog_data->ax2 = UINT_N_TO_AX(td.ay, 16);
}

/*
void phobri_v1_x_analog_read_cal(analog_data_t *analog) {
    analog->ax1 = hybrid_lis3mdl_val; // UINT_N_TO_AX(
                              //(uint16_t)(x_avg.running_sum_large >>
                              // CALIB_AVG_LEN_BITS), 16);
    analog->ax2 = UINT_N_TO_AX(
        (uint16_t)(y_avg.running_sum_large >> CALIB_AVG_LEN_BITS), 16);
}
*/