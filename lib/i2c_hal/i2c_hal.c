#include "i2c_hal.h"

volatile uint8_t status = TWI_NONE;

ISR(TWI_vect) 
{
    status = (TWSR & TWI_NONE);
}

static uint8_t i2c_start()
{
    uint16_t timer = 0;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);

    while(status != TWI_START)
    {
        timer++; 
        if(timer >= TWI_TIMEOUT)
        {
            return TWI_ERROR_START;
        }
    }
    return TWI_OK;
}

static void i2c_stop()
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN) | (1 << TWIE);
}

static uint8_t i2c_restart()
{
    uint16_t timer = 0;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);

    while(status != TWI_RESTART)
    {
        timer++;
        if(timer >= TWI_TIMEOUT)
        {
            return TWI_ERROR_START;
        }
    }
    return TWI_OK;
}

static uint8_t i2c_addr_write_ack()
{
    uint16_t timer = 0;

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);

    while(status != TWI_ADDR_ACK)
    {
        timer++;
        if(timer >= TWI_TIMEOUT)
        {
            return TWI_ERROR_START;
        }
    }
    return TWI_OK;
}

static uint8_t i2c_byte_write_ack()
{
    uint16_t timer = 0;

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);

    while(status != TWI_BYTE_ACK)
    {
        timer++;
        if(timer >= TWI_TIMEOUT)
        {
            return TWI_ERROR_START;
        }
    }
    return TWI_OK;
}

static uint8_t i2c_addr_read_ack()
{
    uint16_t timer = 0;

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);

    while(status != TWIREC_ADDR_ACK)
    {
        timer++;
        if(timer >= TWI_TIMEOUT)
        {
            return TWI_ERROR_START;
        }
    }
    return TWI_OK;
}

static uint8_t i2c_byte_read_ack(uint8_t ack)
{
    uint16_t timer = 0;
    if(ack != 0)
    {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);

        while(status != TWIREC_BYTE_ACK)
        {
            timer++;
            if(timer >= TWI_TIMEOUT)
            {
                return TWI_ERROR_START;
            }
        }   
    }
    else
    {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);

        while(status != TWIREC_BYTE_NACK)
        {
            timer++;
            if(timer >= TWI_TIMEOUT)
            {
                return TWI_ERROR_START;
            }
        }   
    }
    return TWI_OK;
}

//////////////////////////////////////////////////////////////////////////////////

uint8_t i2c_begin_transmission(uint8_t addr, uint8_t rw)
{
    uint8_t err = TWI_OK;

    err = i2c_start();
    if(err != TWI_OK)
    {
        i2c_stop();
        return err;
    }

    TWDR = (addr << 1) | rw;
    if(rw == 0)
        err = i2c_addr_write_ack();
    else
        err = i2c_addr_read_ack();

    if(err != TWI_OK)
    {
        i2c_stop();
        return err;
    }

    return err;
}

uint8_t i2c_read_byte(uint8_t ack, uint8_t *byte)
{

}

uint8_t i2c_read(uint8_t addr, uint8_t *data, uint16_t len)
{
    uint16_t i = 0;
    uint8_t err = TWI_OK;

    err = i2c_start();
    if(err != TWI_OK)
    {
        i2c_stop();
        return err;
    }

    TWDR = (addr << 1) | 1;
    err = i2c_addr_read_ack();
    if(err != TWI_OK)
    {
        i2c_stop();
        return err;
    }

    for(i = 0; i < (len - 1); i++)
    {
        err = i2c_byte_read_ack(1);
        if(err != TWI_OK)
        {
            i2c_stop();
            return err;
        }
        data[i] = TWDR;
    }

    err = i2c_byte_read_ack(0);
    if(err != TWI_OK)
    {
        i2c_stop();
        return err;
    }
    data[i] = TWDR;

    i2c_stop();
    
    return err;
}

uint8_t i2c_write(uint8_t addr, uint8_t *data, uint16_t len)
{
    uint16_t i = 0;
    uint8_t err = TWI_OK;

    err = i2c_start();
    if(err != TWI_OK)
    {
        i2c_stop();
        return err;
    }

    TWDR = (addr << 1) | 0;
    err = i2c_addr_write_ack();
    if(err != TWI_OK)
    {
        i2c_stop();
        return err;
    }

    for(i = 0; i < len; i++)
    {
        TWDR = data[i];
        err = i2c_byte_write_ack();
        if(err != TWI_OK)
        {
            i2c_stop();
            return err;
        }
    }

    i2c_stop();

    return err;
}


void i2c_init(uint32_t speed)
{
    uint32_t gen_t = 0;
    gen_t = (((F_CPU / speed ) - 16) / 2) & 0xFF;
    
    TWBR = gen_t & 0xFF;
    TWCR = (1 << TWEN) | (1 << TWIE);
}