#include "pico/stdlib.h"
#include "AT24C.h"
#include <stdio.h>

void main(void) {
    stdio_init_all();
    at24c_i2c_init();

    AT24C chip;
    at24c_config_chip(&chip, AT24C02, 0,0,0);

    char message[] = "Hello world !";
    at24c_write(&chip, 0x00, message, 14);

    char buffer[14];
    at24c_read(&chip, 0x00, buffer, 14);

    while(1) {
        printf("%s\n", buffer);
        sleep_ms(500);
    }
}