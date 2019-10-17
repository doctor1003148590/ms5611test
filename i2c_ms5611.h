
#ifndef _I2C_MS5611_H_
#define _I2C_MS5611_H_

#include <stdint.h>

#define I2C_OK 0
#define I2C_ERR -1

int8_t readBytes(uint8_t devAddr, uint8_t length, uint8_t *data);
int writeBytes(uint8_t devAddr, uint8_t length, uint8_t *data);

#endif
