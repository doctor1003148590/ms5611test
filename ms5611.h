#ifndef MS5611_H
#define MS5611_H

#include <stdint.h>

//MS5611 I2C address
#define MS5611_ADDR            0x77
//Reset command
#define CMD_RESET              0x1E
//Convert D1 pressure commands
#define CMD_CONVERT_D1_OSR256  0x40  //Convert D1 pressure oversampling ratio 256
#define CMD_CONVERT_D1_OSR512  0x42  //Convert D1 pressure oversampling ratio 512
#define CMD_CONVERT_D1_OSR1024 0x44  //Convert D1 pressure oversampling ratio 1024
#define CMD_CONVERT_D1_OSR2048 0x46  //Convert D1 pressure oversampling ratio 2048
#define CMD_CONVERT_D1_OSR4096 0x48  //Convert D1 pressure oversampling ratio 4096
//Convert D2 temperature commands
#define CMD_CONVERT_D2_OSR256  0x50  //Convert D2 temperature oversampling ratio 256
#define CMD_CONVERT_D2_OSR512  0x52  //Convert D2 temperature oversampling ratio 512
#define CMD_CONVERT_D2_OSR1024 0x54  //Convert D2 temperature oversampling ratio 1024
#define CMD_CONVERT_D2_OSR2048 0x56  //Convert D2 temperature oversampling ratio 2048
#define CMD_CONVERT_D2_OSR4096 0x58  //Convert D2 temperature oversampling ratio 4096
//ADC Read command
#define CMD_ADC_READ           0x00
//PROM Read commands
#define CMD_PROM_READ_BASE     0xA0  // bit 4,3,2 make eight addresses from 0xA0 to 0xAE which is: 0xA0 0xA2 0xA4 0xA6 0xA8 0xAA 0xAC 0xAE

#define DT_MIN                 -16776960
#define DT_MAX                 16777216
#define OFF_MIN                -8589672450
#define OFF_MAX                12884705280
#define SENS_MIN               -4294836225
#define SENS_MAX               6442352640
#define SEA_LEVEL_PRESSURE     1013.25 // sea-level atmospheric pressure on Earth

extern float     temp;   //actual temperature
extern float     p;      //temperature compensated pressure

void ms5611_init();
void ms5611_reset();
void ms5611_update();
void ReadProm();
void GetTemperature();
void GetCompensatedPressure();
uint8_t WriteCommand(uint8_t devAddr, uint8_t data);
uint32_t ReadAdc();

#endif