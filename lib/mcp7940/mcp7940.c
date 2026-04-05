#include "mcp7940.h"

static uint8_t clear_register_bit(uint8_t addr_byte, uint8_t bit)
{
    uint8_t err = 0x00;
    uint8_t read_data = 0, new_data = 0;
    err = i2c_random_read(MCP7940_ADDR, addr_byte, &read_data);
    if(I2C_OK != err)
    {
        return I2C_ERROR;
    }
    new_data = read_data & ~(1 << bit);
    err = i2c_byte_write(MCP7940_ADDR, addr_byte, new_data);
    if(I2C_OK != err)
    {
        return I2C_ERROR;
    }

    return I2C_OK;
}

static uint8_t set_register_bit(uint8_t addr_byte, uint8_t bit)
{
    uint8_t err = 0x00;
    uint8_t read_data = 0, new_data = 0;
    err = i2c_random_read(MCP7940_ADDR, addr_byte, &read_data);
    if(I2C_OK != err)
    {
        return I2C_ERROR;
    }
    new_data = read_data | (1 << bit);
    err = i2c_byte_write(MCP7940_ADDR, addr_byte, new_data);
    if(I2C_OK != err)
    {
        return I2C_ERROR;
    }

    return I2C_OK;
}

static uint8_t read_register_bit(uint8_t addr_byte, uint8_t bit, uint8_t *val)
{
    uint8_t err = 0x00;
    uint8_t read_data = 0;
    err = i2c_random_read(MCP7940_ADDR, addr_byte, &read_data);
    if(I2C_OK != err)
    {
        return I2C_ERROR;
    }

    *val = (read_data >> bit) & 0x01;
    return I2C_OK;
}

static uint8_t bcd_to_int(uint8_t bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

uint8_t mcp7940_device_start()
{
    uint8_t err = i2c_byte_write(MCP7940_ADDR, RTCSEC, 0x80);
    uint8_t osc_status = 0;
    if(I2C_OK != err)
    {
        return I2C_ERROR;
    }

    for(uint8_t i = 0; i < 255; i++)
    {
        read_register_bit(RTCWKDAY, OSCRUN, &osc_status);
        if(osc_status)
            break;
    }

    return osc_status;
}

mcp7940_time mcp7940_now()
{
    mcp7940_time time;
    
    uint8_t seconds_reg = 0;
    i2c_random_read(MCP7940_ADDR, RTCSEC, &seconds_reg);

    seconds_reg = seconds_reg & 0x7F;
    time.seconds = bcd_to_int(seconds_reg);

    return time;
}