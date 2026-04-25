#ifndef ATC24C_H
#define AT24C_H

typedef struct {
    uint8_t address;
    uint8_t page_size;
    uint16_t storage_capacity;
} AT24C;

typedef enum {
    AT24C01,
    AT24C02
} Model;

void at24c_i2c_init(void);
void at24c_config_chip(AT24C* chip, Model chip_model, bool A0, bool A1, bool A2);

bool at24c_is_connected(AT24C* chip);

bool at24c_write(AT24C* chip, uint8_t reg, uint8_t* value, uint8_t length);
bool at24c_read(AT24C* chip, uint8_t reg, uint8_t* dest, uint8_t length);

#endif