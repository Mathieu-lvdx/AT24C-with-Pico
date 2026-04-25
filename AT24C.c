#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdlib.h>
#include "AT24C.h"

#define PIN_SDA 4
#define PIN_SCL 5

i2c_inst_t* I2C_PORT = i2c0;

void at24c_i2c_init(void) {
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PIN_SDA);
    gpio_pull_up(PIN_SCL);
}

void at24c_config_chip(AT24C* chip, Model chip_model, bool A0, bool A1, bool A2) {
    chip->address = 0x50 | (A2 << 2) | (A1 << 1) | A0;
    chip->page_size = 8;
    chip->storage_capacity = (chip_model == AT24C01) ? 128 : 256;
}

bool at24c_is_connected(AT24C* chip) {
    uint8_t temp;
    int8_t succes = i2c_read_blocking(I2C_PORT, chip->address, &temp, 1, false);
    return succes == 1;
}

bool at24c_write(AT24C* chip, uint8_t reg, uint8_t* value, uint8_t length) {
    if(reg + length > chip->storage_capacity) return false;
    uint8_t index = 0;
    while(index < length) {
        uint8_t empty_space = chip->page_size - (reg + index) % chip->page_size;
        uint8_t* tx_buffer;
        uint8_t size;
        if(length - index > empty_space) {
            tx_buffer = (uint8_t*)malloc(1 + empty_space);
            size = empty_space;
        }
        else {
            tx_buffer = (uint8_t*)malloc(1 + length - index);
            size = length - index;
        }
        tx_buffer[0] = reg + index;
        for(uint8_t i=0; i<size; i++) {
            tx_buffer[i+1] = value[index];
            index ++;
        }
        i2c_write_blocking(I2C_PORT, chip->address, tx_buffer, size+1, false);
        free(tx_buffer);
        sleep_ms(5);
    }
    return true;
}

bool at24c_read(AT24C* chip, uint8_t reg, uint8_t* dest, uint8_t length) {
    if(reg + length > chip->storage_capacity) return false;
    i2c_write_blocking(I2C_PORT, chip->address, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, chip->address, dest, length, false);
    return true;
}