#include <hidapi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define VENDOR_ID 0x1337
#define PRODUCT_ID 0x4004

int main(int argc, char **argv) {
    hid_device *handle;
    char device_resp[5];
    int result;

    result = hid_init();

    handle = hid_open(VENDOR_ID, PRODUCT_ID, NULL);

    if (handle == NULL) {
        printf("Can't open Phobri device.\n");
        result = hid_exit();
        exit(-1);
    }

    FILE* csv_file = fopen("ctlr.csv", "w");
    if (csv_file == NULL) {
        printf("Can't open ctrl.csv for writing.\n");
        exit(-1);
    }

    int bytes_read = 0;
    int cnt = 0;

    fprintf(csv_file, "x,y\n");
    while (cnt < 2000) {
        bytes_read = hid_read(handle, device_resp, 5);

        if (bytes_read == 5) {
            uint16_t xval = (device_resp[2] << 8) + device_resp[1];
            uint16_t yval = (device_resp[4] << 8) + device_resp[3];
            fprintf(csv_file, "%d,%d\n", xval, yval);
        } else {
            printf("Didn't read the proper amount.. %x\n", bytes_read);
            printf("%x %x %x %x %x %x\n", device_resp[0], device_resp[1],device_resp[2],device_resp[3], device_resp[4], device_resp[5]);
        }
        cnt++;
    }
    fclose(csv_file);

}