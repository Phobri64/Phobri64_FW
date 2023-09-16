#ifndef PHOBRI_V1_X_H
#define PHOBRI_V1_X_H

#include "zenith/types.h"
#include <hardware/i2c.h>

#define JOYBUS_PIN 0
#define BTN_A_PIN 11
#define BTN_B_PIN 16
#define BTN_START_PIN 25
#define BTN_ZR_PIN 9
#define BTN_ZL_PIN 24
#define BTN_R_PIN 8
#define BTN_L_PIN 23

#define BTN_CU_PIN 2
#define BTN_CD_PIN 10
#define BTN_CL_PIN 1
#define BTN_CR_PIN 3

#define BTN_DU_PIN 20
#define BTN_DD_PIN 21
#define BTN_DL_PIN 22
#define BTN_DR_PIN 19

// TODO: change
#define DEBUG_TX_PIN 26
#define DEBUG_UART uart0

// TODO: change
#define STICK_I2C_INTF i2c1
#define STICK_I2C_SCL 18
#define STICK_I2C_SDA 17
#define STICK_HX_DRDY 12
#define STICK_HY_DRDY 13
#define STICK_ADC_DRDY_N 15

#define I2C_HX_ADDR 0x1C
#define I2C_HY_ADDR 0x1E
#define I2C_ADC_ADDR 0x18

void phobri_v1_x_3d_core1_init(void);

void phobri_v1_x_3d_read_analog(analog_data_t *analog);

void phobri_v1_x_analog_core1_init(void);

void phobri_v1_x_analog_read_analog(analog_data_t *analog);

#endif /* PHOBRI_V1_X_H */