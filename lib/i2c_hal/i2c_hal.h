#ifndef I2C_HAL_H
#define I2C_HAL_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define STATUS_MASK 0xF8

//1ms timeout
#define TWI_TIMEOUT 1600

#define TWI_START 0x08
#define TWI_RESTART 0x10

//Master transmitter mode statuses
#define TWI_ADDR_ACK 0x18
#define TWI_ADDR_NACK 0x20
#define TWI_BYTE_ACK 0x28
#define TWI_BYTE_NACK 0x30

//Master receiver mode statuses
#define TWIREC_ADDR_ACK 0x40
#define TWIREC_ADDR_NACK 0x48
#define TWIREC_BYTE_ACK 0x50
#define TWIREC_BYTE_NACK 0x58

#define TWI_ERROR 0x38
#define TWI_NONE 0xF8

enum {
    TWI_OK,
    TWI_ERROR_START,
    TWI_ERROR_RESTART,
    TWI_NACK
};

void i2c_init(uint32_t speed);

uint8_t i2c_read(uint8_t addr, uint8_t *data, uint16_t len);
uint8_t i2c_write(uint8_t addr, uint8_t *data, uint16_t len);

uint8_t i2c_random_read(uint8_t addr, uint8_t addr_byte, uint8_t *data);
uint8_t i2c_sequential_read(uint8_t addr, uint8_t byte_addr, uint8_t *data, uint16_t len);
uint8_t i2c_byte_write(uint8_t addr, uint8_t byte_addr, uint8_t byte);
uint8_t i2c_sequential_write(uint8_t addr, uint8_t byte_addr, uint8_t *data, uint16_t len);

#endif