#include "main.h"
#include <stdio.h>

typedef enum {
    HW_MODE_PROTO,
    HW_MODE_v1_X_ANALOG,
    HW_MODE_v1_X_3D,
} phobri_hw_mode_t;

// 32 byte limit
typedef struct {
    phobri_hw_mode_t hw_mode
} phobri_user_settings_t;

phobri_user_settings_t *_phobri_user_settings;

void setup_gpio_button(uint8_t gpio) {
    gpio_init(gpio);
    gpio_pull_up(gpio);
    gpio_set_dir(gpio, GPIO_IN);
}

struct {
    int A;
    int B;
    int START;
    int ZR;
    int ZL;
    int R;
    int L;
    int CU;
    int CD;
    int CL;
    int CR;
    int DU;
    int DD;
    int DL;
    int DR;
} _btn_pins;

void cb_zenith_init_hardware(void) {
    _phobri_user_settings = zenith_get_user_settings_ptr();

    if (_phobri_user_settings->hw_mode == HW_MODE_PROTO) {
        _btn_pins.A = PROTO_BTN_A_PIN;
        _btn_pins.B = PROTO_BTN_B_PIN;
        _btn_pins.START = PROTO_BTN_START_PIN;
        _btn_pins.ZR = PROTO_BTN_ZR_PIN;
        _btn_pins.ZL = PROTO_BTN_ZL_PIN;
        _btn_pins.R = PROTO_BTN_R_PIN;
        _btn_pins.L = PROTO_BTN_L_PIN;
        _btn_pins.CU = PROTO_BTN_CU_PIN;
        _btn_pins.CD = PROTO_BTN_CD_PIN;
        _btn_pins.CL = PROTO_BTN_CL_PIN;
        _btn_pins.CR = PROTO_BTN_CR_PIN;
        _btn_pins.DU = PROTO_BTN_DU_PIN;
        _btn_pins.DD = PROTO_BTN_DD_PIN;
        _btn_pins.DL = PROTO_BTN_DL_PIN;
        _btn_pins.DR = PROTO_BTN_DR_PIN;

    } else {
        _btn_pins.A = V1_X_BTN_A_PIN;
        _btn_pins.B = V1_X_BTN_B_PIN;
        _btn_pins.START = V1_X_BTN_START_PIN;
        _btn_pins.ZR = V1_X_BTN_ZR_PIN;
        _btn_pins.ZL = V1_X_BTN_ZL_PIN;
        _btn_pins.R = V1_X_BTN_R_PIN;
        _btn_pins.L = V1_X_BTN_L_PIN;
        _btn_pins.CU = V1_X_BTN_CU_PIN;
        _btn_pins.CD = V1_X_BTN_CD_PIN;
        _btn_pins.CL = V1_X_BTN_CL_PIN;
        _btn_pins.CR = V1_X_BTN_CR_PIN;
        _btn_pins.DU = V1_X_BTN_DU_PIN;
        _btn_pins.DD = V1_X_BTN_DD_PIN;
        _btn_pins.DL = V1_X_BTN_DL_PIN;
        _btn_pins.DR = V1_X_BTN_DR_PIN;
    }
    setup_gpio_button(_btn_pins.A);
    setup_gpio_button(_btn_pins.B);
    setup_gpio_button(_btn_pins.START);
    setup_gpio_button(_btn_pins.ZR);
    setup_gpio_button(_btn_pins.ZL);
    setup_gpio_button(_btn_pins.R);
    setup_gpio_button(_btn_pins.L);
    setup_gpio_button(_btn_pins.CU);
    setup_gpio_button(_btn_pins.CD);
    setup_gpio_button(_btn_pins.CL);
    setup_gpio_button(_btn_pins.CR);
    setup_gpio_button(_btn_pins.DU);
    setup_gpio_button(_btn_pins.DD);
    setup_gpio_button(_btn_pins.DL);
    setup_gpio_button(_btn_pins.DR);
}
void cb_zenith_read_buttons(btn_data_t *buttons) {
    buttons->s.b1 = !gpio_get(_btn_pins.A);
    buttons->s.b2 = !gpio_get(_btn_pins.B);
    buttons->s.b3 = !gpio_get(_btn_pins.CU);
    buttons->s.b4 = !gpio_get(_btn_pins.CD);
    buttons->s.b5 = !gpio_get(_btn_pins.CL);
    buttons->s.b6 = !gpio_get(_btn_pins.CR);
    buttons->s.b7 = !gpio_get(_btn_pins.START);
    buttons->s.b8 = !gpio_get(_btn_pins.L);
    buttons->s.b9 = !gpio_get(_btn_pins.R);
    buttons->s.b10 = !gpio_get(_btn_pins.ZL);
    buttons->s.b11 = !gpio_get(_btn_pins.DD);
    buttons->s.b12 = !gpio_get(_btn_pins.DL);
    buttons->s.b13 = !gpio_get(_btn_pins.DR);
    buttons->s.b14 = !gpio_get(_btn_pins.DU);
    buttons->s.b15 = !gpio_get(_btn_pins.ZR);
}

void cb_zenith_read_analog(analog_data_t *analog_data) {
    switch (_phobri_user_settings->hw_mode) {
    case HW_MODE_PROTO: {
        phobri_proto_core1_read_analog();
    } break;
    case HW_MODE_v1_X_3D: {
        phobri_v1_x_3d_core1_read_analog();
    } break;
    case HW_MODE_v1_X_ANALOG: {
        phobri_v1_x_analog_core1_read_analog();
    } break;
    default: {
        __unreachable();
    } break;
    }
}

void cb_zenith_core1_init(void) {
    switch (_phobri_user_settings->hw_mode) {
    case HW_MODE_PROTO: {
        phobri_proto_core1_init();
    } break;
    case HW_MODE_v1_X_3D: {
        phobri_v1_x_3d_core1_init();
    } break;
    case HW_MODE_v1_X_ANALOG: {
        phobri_v1_x_analog_core1_init();
    } break;
    default: {
        __unreachable();
    } break;
    }
}
void cb_zenith_core0_inject(void) {
#ifdef DEBUG
    if (!gpio_get(_btn_pins.START) && !gpio_get(_btn_pins.ZL) &&
        !gpio_get(_btn_pins.ZR)) {
        reset_usb_boot(0, 0);
    }

    if (!gpio_get(_btn_pins.START) && !gpio_get(_btn_pins.L) &&
        !gpio_get(_btn_pins.R)) {
        watchdog_reboot(0, 0, 0);
    }
#endif
}

// TODO: could use this later to switch between hardware modes at runtime
// but might not be potentially desirable
bool cb_zenith_webusb_settings_cmd(uint8_t *in, uint8_t *out) { return false; }

void cb_zenith_user_settings_reset_default(uint8_t *data) {
    phobri_user_settings_t default_settings = {.hw_mode = HW_MODE_v1_X_3D};
    memcpy(data, &default_settings, sizeof(phobri_user_settings_t));
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
