# AT24C interface for Raspberry Pi Pico

Here's a simple way to use AT24C01 and AT24C02 with a Pico. This code allows you to use multiple AT24C on the same I²C line.

The default connection with the chip follows the array below. You can change I²C pins in _AT24C.c_.

| Pico Pins           | AT24C Pins |
|------------------|-------------|
| Pin 36 (3V3 Out) | VCC  |
| Pin 38 (GND)     | GND  |
| Pin 7 (GPIO 5)   | SCL  |
| Pin 6 (GPIO 4)   | SDA  |

## Chip struct

`AT24C` is the struct were informations about chip are stored. **Each chip has its own struct**.

```c
// You can find this stuct in AT24C.h

typedef struct {
    uint8_t address;
    uint8_t page_size;
    uint16_t storage_capacity;
} AT24C;
```

To configure your AT24C, you have to use the `at24c_config_chip` function. The code manages that struct itself.

## Functions

### Initialize I²C

`at24c_i2c_init()` is the fuction which initialize the I²C protocol. If you already have your own function, you don't have to use this one.

### Config chip

It is the function you have to use to initialize a chip.

```c
void at24c_config_chip(AT24C* chip, Model chip_model, bool A0, bool A1, bool A2)
```

`chip` is the pointer to your AT24C struct.  
`chip_model` is the model of your chip. You can only use `AT24C01` or `AT24C02`.  
`A0, A1, A2` are physical address inputs of your chip. Set `true` if it's on **VCC** and `false` if on **GND**.

### Check the communication

Check if your chip works correctly. Returns true if the chip works.

```c
bool at24c_is_connected(AT24C* chip)
```

`chip` is the pointer to your AT24C struct.

### Write data

It is the function that write data on AT24C. Returns `true` if success.

```c
bool at24c_write(AT24C* chip, uint8_t reg, uint8_t* value, uint8_t length)
```

`chip` is the pointer to your AT24C struct.  
`reg` is the register where you want to write the first byte of your data. Up to `0x7F` for AT24C01 and `0xFF` for AT24C02.  
`value` is the pointer to bytes you want to write.  
`length` is the length of your data.

### Read data

It's the function which read data from the chip. Returns `true` if success.

```c
bool at24c_read(AT24C* chip, uint8_t reg, uint8_t* dest, uint8_t length)
```

`chip` is the pointer to your AT24C struct.  
`reg` is the register where you want to read the first byte of your data. From 0x00 to 0x7F for AT24C01 and 0x00-0xFF for AT24C02.  
`dest` is the pointer to your buffer which store data.  
`length` is the length of data you want to read.

## Code sample

This code initializes an AT24C02, write `Hello world !` at register 0x00 and read it.

```c
// This code comes from example.c

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
```