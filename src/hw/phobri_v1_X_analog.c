#include "hw/phobri_v1_X.h"
#include "zenith/types.h"
#include "zenith/utilities/running_avg.h"
#include <hardware/gpio.h>

#define CALIB_AVG_LEN_BITS 3
#define NORMAL_AVG_LEN_BITS 0

running_avg_t x_avg;
running_avg_t y_avg;
ax_t hybrid_lis3mdl_val = 0;

void hybrid_lis3mdl_setup(uint i2c_addr) {
    // Temp disabled, LP mode w/ FAST_ODR @ 1kHZ
    uint8_t ctrl_reg1_cfg[] = {0x20, 0b00000010};
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

void __time_critical_func(hybrid_lis3mdl_read)(float *dest, uint i2c_addr) {

    uint8_t regl;
    uint8_t regh;
    uint8_t read_buf[2];
    int16_t xval;
    int16_t yval;
    int16_t zval;

    regl = 0x28;
    regh = 0x29;
    i2c_write_blocking(STICK_I2C_INTF, i2c_addr, &regl, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, i2c_addr, read_buf, 1, false);
    i2c_write_blocking(STICK_I2C_INTF, i2c_addr, &regh, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, i2c_addr, (read_buf + 1), 1, false);
    xval = (read_buf[1] << 8) + read_buf[0];

    // not using these for now, only reading the X-axis

    regl = 0x2a;
    regh = 0x2b;
    i2c_write_blocking(STICK_I2C_INTF, i2c_addr, &regl, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, i2c_addr, read_buf, 1, false);
    i2c_write_blocking(STICK_I2C_INTF, i2c_addr, &regh, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, i2c_addr, (read_buf + 1), 1, false);
    yval = (read_buf[1] << 8) + read_buf[0];

    regl = 0x2c;
    regh = 0x2d;
    i2c_write_blocking(STICK_I2C_INTF, i2c_addr, &regl, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, i2c_addr, read_buf, 1, false);
    i2c_write_blocking(STICK_I2C_INTF, i2c_addr, &regh, 1, true);
    i2c_read_blocking(STICK_I2C_INTF, i2c_addr, (read_buf + 1), 1, false);
    zval = (read_buf[1] << 8) + read_buf[0];

    *dest = INT_N_TO_AX(zval, 16); // atan2f((float)zval, (float)xval);
    // sqrtf((float)((xval * xval) + (yval * yval) + (zval * zval)));
}
void __time_critical_func(hybrid_lis3mdl_isr)(uint gpio, uint32_t events) {
    (void)gpio;
    (void)events;
    hybrid_lis3mdl_read(&hybrid_lis3mdl_val, I2C_HX_ADDR);
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
    // uint16_t _adc_x_val = ((data_buf[3] << 8) + data_buf[2]);
    uint16_t _adc_y_val = ((data_buf[1] << 8) + data_buf[0]);
    // update_running_avg(&x_avg, _adc_x_val);
    update_running_avg(&y_avg, _adc_y_val);
    // printf("%d %d\n", _adc_x_val, _adc_y_val);
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

    // init_running_avg(&x_avg, CALIB_AVG_LEN_BITS, NORMAL_AVG_LEN_BITS);
    init_running_avg(&y_avg, CALIB_AVG_LEN_BITS, NORMAL_AVG_LEN_BITS);

    // gpio_set_irq_enabled_with_callback(STICK_ADC_DRDY_N, GPIO_IRQ_EDGE_FALL,
    //                                   true, &ads7142_isr);
}

void phobri_v1_x_analog_read_analog(analog_data_t *analog_data) {
    ads7142_isr(0, 0);
    hybrid_lis3mdl_isr(0, 0);
    analog_data->ax1 =
        hybrid_lis3mdl_val; // UINT_N_TO_AX(
                            //(uint16_t)(x_avg.running_sum_small >>
                            // NORMAL_AVG_LEN_BITS), 16);
    analog_data->ax2 = UINT_N_TO_AX(
        (uint16_t)(y_avg.running_sum_small >> NORMAL_AVG_LEN_BITS), 16);
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