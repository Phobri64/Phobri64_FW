#include "main.h"
#include <stdio.h>

void setup_gpio_button(uint8_t gpio) {
    gpio_init(gpio);
    gpio_pull_up(gpio);
    gpio_set_dir(gpio, GPIO_IN);
}

void cb_zenith_init_hardware(void) {
    setup_gpio_button(BTN_A_PIN);
    setup_gpio_button(BTN_B_PIN);
    setup_gpio_button(BTN_START_PIN);
    setup_gpio_button(BTN_ZR_PIN);
    setup_gpio_button(BTN_ZL_PIN);
    setup_gpio_button(BTN_R_PIN);
    setup_gpio_button(BTN_L_PIN);
    setup_gpio_button(BTN_CU_PIN);
    setup_gpio_button(BTN_CD_PIN);
    setup_gpio_button(BTN_CL_PIN);
    setup_gpio_button(BTN_CR_PIN);
    setup_gpio_button(BTN_DU_PIN);
    setup_gpio_button(BTN_DD_PIN);
    setup_gpio_button(BTN_DL_PIN);
    setup_gpio_button(BTN_DR_PIN);

    spi_init(STICK_SPI_INTF, 3000 * 1000);
    gpio_set_function(STICK_SPI_CLK, GPIO_FUNC_SPI);
    gpio_set_function(STICK_SPI_TX, GPIO_FUNC_SPI);
    gpio_set_function(STICK_SPI_RX, GPIO_FUNC_SPI);

    gpio_init(STICK_SPI_CS);
    gpio_set_dir(STICK_SPI_CS, GPIO_OUT);
    gpio_put(STICK_SPI_CS, true); // active low
}
void cb_zenith_read_buttons(btn_data_t *buttons) {
    buttons->s.b1 = !gpio_get(BTN_A_PIN);
    buttons->s.b2 = !gpio_get(BTN_B_PIN);
    buttons->s.b3 = !gpio_get(BTN_START_PIN);
    buttons->s.b4 = !gpio_get(BTN_ZR_PIN);
    buttons->s.b5 = !gpio_get(BTN_ZL_PIN);
    buttons->s.b6 = !gpio_get(BTN_R_PIN);
    buttons->s.b7 = !gpio_get(BTN_L_PIN);
    buttons->s.b8 = !gpio_get(BTN_CU_PIN);
    buttons->s.b9 = !gpio_get(BTN_CD_PIN);
    buttons->s.b10 = !gpio_get(BTN_CL_PIN);
    buttons->s.b11 = !gpio_get(BTN_CR_PIN);
    buttons->s.b12 = !gpio_get(BTN_DU_PIN);
    buttons->s.b13 = !gpio_get(BTN_DD_PIN);
    buttons->s.b14 = !gpio_get(BTN_DL_PIN);
    buttons->s.b15 = !gpio_get(BTN_DR_PIN);
}

uint16_t __time_critical_func(read_ext_adc)(bool is_x_axis) {
    const uint8_t config_val = (!is_x_axis) ? 0xD0 : 0xF0;
    uint8_t data_buf[3];
    gpio_put(STICK_SPI_CS, 0);

    spi_read_blocking(STICK_SPI_INTF, config_val, data_buf, 3);
    uint16_t tempValue =
        ((data_buf[0] & 0x07) << 9) | data_buf[1] << 1 | data_buf[2] >> 7;

    gpio_put(STICK_SPI_CS, 1);
    return tempValue;
}

void cb_zenith_read_analog(analog_data_t *analog_data) {
    uint16_t raw_x = read_ext_adc(true);
    uint16_t raw_y = read_ext_adc(false);
    analog_data->ax1 = UINT_N_TO_AX(raw_x, 12);
    analog_data->ax2 = UINT_N_TO_AX(raw_y, 12);
}

void cb_zenith_core1_init(void) {}
void cb_zenith_core0_inject(void) {
    if (!gpio_get(BTN_START_PIN) && !gpio_get(BTN_ZL_PIN) &&
        !gpio_get(BTN_ZR_PIN)) {
        reset_usb_boot(0, 0);
    }

    if (!gpio_get(BTN_START_PIN) && !gpio_get(BTN_L_PIN) &&
        !gpio_get(BTN_R_PIN)) {
        watchdog_reboot(0, 0, 0);
    }
}
void cb_zenith_core1_inject(void) {}

int main() {
    set_sys_clock_khz(130000, true);

    stdio_init_all();
    printf("Phobri64 Started.\n");

    setup_gpio_button(BTN_START_PIN);
    // Handle bootloader stuff
    if (!gpio_get(BTN_START_PIN)) {
        reset_usb_boot(0, 0);
    }

    zenith_start();
}
