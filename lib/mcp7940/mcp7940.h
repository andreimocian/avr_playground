#ifndef MCP7940
#define MCP7940

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <i2c_hal.h>

#define MCP7940_ADDR 0x6F

#define RTCSEC 0x00
#define RTCMIN 0x01
#define RTCHOUR 0x02
#define RTCWKDAY 0x03

//RTCSEC
#define ST 7
#define SECTEN2 6
#define SECTEN1 5
#define SECTEN0 4
#define SECONE3 3
#define SECONE2 2
#define SECONE1 1
#define SECONE0 0

//RTCMIN
#define MINTEN2 6
#define MINTEN1 5
#define MINTEN0 4
#define MINONE3 3
#define MINONE2 2
#define MINONE1 1
#define MINONE0 0

//RTCHOUR
#define H12_24 6
#define AM_PM_HRTEN1 5
#define HRTEN0 4
#define HRONE3 3
#define HRONE2 2
#define HRONE1 1
#define HRONE0 0

//RTCWKDAY
#define OSCRUN 5
#define PWRFAIL 4
#define VBATEN 3
#define WKDAY2 2
#define WKDAY1 1
#define WKDAY0 0

#define I2C_OK 0

enum {
    MCP7940_OK,
    MCP7940_ERROR,
    I2C_ERROR
};

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
}mcp7940_time;

uint8_t mcp7940_device_start();
mcp7940_time mcp7940_now();

#endif