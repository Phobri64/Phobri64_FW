#include "hw/phobri_v1_X.h"
#include "zenith/types.h"
#include <hardware/gpio.h>

uint16_t _adc_x_val = 0;
uint16_t _adc_y_val = 0;

void ads7142_setup(void) {
    // Manual mode with Auto-Sequence
    uint8_t opmode_sel_data[] = {
        0x8,
        0x1C,
        0b00000101,
    };

    // Start the sequence
    uint8_t start_sequence_data[] = {
        0x8,
        0x1E,
        0b00000001,
    };

    i2c_write_blocking(STICK_I2C_INTF, I2C_ADC_ADDR, opmode_sel_data, 3, false);
    i2c_write_blocking(STICK_I2C_INTF, I2C_ADC_ADDR, start_sequence_data, 3,
                       false);
}

void __time_critical_func(ads7142_isr)(uint gpio, uint32_t events) {
    uint8_t data_buf[4];

    i2c_read_blocking(STICK_I2C_INTF, I2C_ADC_ADDR, data_buf, 4, false);
    _adc_x_val = data_buf[0] << 8 + data_buf[1];
    _adc_y_val = data_buf[2] << 8 + data_buf[3];
}

void phobri_v1_x_analog_core1_init(void) {
    i2c_init(STICK_I2C_INTF, 400 * 1000);
    gpio_set_function(STICK_I2C_SCL, GPIO_FUNC_I2C);
    gpio_set_function(STICK_I2C_SDA, GPIO_FUNC_I2C);

    ads7142_setup();

    gpio_set_irq_enabled_with_callback(STICK_ADC_DRDY_N, GPIO_IRQ_EDGE_FALL,
                                       true, &ads7142_isr);
}

void phobri_v1_x_analog_read_analog(analog_data_t *analog_data) {
    analog_data->ax1 = UINT_N_TO_AX(_adc_x_val, 16);
    analog_data->ax2 = UINT_N_TO_AX(_adc_y_val, 16);
}