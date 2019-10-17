//ms5611.c
//Initialize and get data from ms5611.
//Check MS5611 datasheet for more information.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#include "ms5611.h"
#include "i2c_ms5611.h"

//variables
uint16_t  c[8]={0}; //values read from PROM
uint32_t  d1=0;     //Digital pressure value
uint32_t  d2=0;     //Digital temperature value
float     dt=0;     //difference between actual and reference temperature
float     temp=0;   //actual temperature
double    off=0;    //offset at actual temperature
double    sens=0;   //sensitivity at actual temperature
float     p=0;      //temperature compensated pressure

//=============================================================================
// Function Name  : ms5611_init
// Description    : reset ms5611 and read PROM values
//=============================================================================
void ms5611_init()
{
	printf(" Initializing ms5611... \n");
	ms5611_reset();
	ReadProm();  //read ms5611 PROM values
	printf(" Ms5611 initialized!\n");
}

void ms5611_reset()
{
	printf(" Reseting ms5611... \n");
	WriteCommand(MS5611_ADDR,CMD_RESET);  //reset ms5611
	usleep(1000);

}

//=============================================================================
// Function Name  : ms5611_update
// Description    : update d1 and d2 data
//=============================================================================
void ms5611_update()
{
	if (!WriteCommand(MS5611_ADDR,CMD_CONVERT_D1_OSR256))//convert D1 pressure
	{
		printf("Failed to send convert D1 pressure command.\n");
	}
	usleep(1000);
	d1=ReadAdc();//read converted D1 pressure adc to d1
	usleep(1000);

	if (!WriteCommand(MS5611_ADDR,CMD_CONVERT_D2_OSR256))//convert D2 temperature
	{
		printf("Failed to send convert D2 temperature command.\n");
	}
	usleep(1000);
	d2=ReadAdc();//read converted D2 temperature adc to d2
	usleep(1000);

	GetTemperature();
	GetCompensatedPressure();
}

//=============================================================================
// Function Name  : ReadProms
// Description    : read the PROM from address 0~7
//=============================================================================
void ReadProm()
{
	printf(" Reading PROM... \n");
	uint8_t i=0;
	uint8_t buf[2]={0}; //buffer
	for (i=0;i<=7;i++)
	{
		if (!WriteCommand(MS5611_ADDR,CMD_PROM_READ_BASE+2*i))
		{
			printf("Failed to send PROM read request.\n");
		}
		if(readBytes(MS5611_ADDR,2,&buf[2])==0)
		{
			c[i]=(buf[0]<<8) +  buf[1];
			printf("Failed to read PROM data %d.\n",i);
		}
		usleep(1000);
	}
}

//=============================================================================
// Function Name  : ReadAdc
// Description    : read the converted ADC data
// Input          : uint8_t address
// Output         : uint32_t 3 bytes converted ADC data
//=============================================================================
uint32_t ReadAdc()
{
	uint32_t  adc=0;
	uint8_t   buf[3]={0}; //buffer
	if (!WriteCommand(MS5611_ADDR,CMD_ADC_READ))
	{
		printf("Failed to send read ADC result command.\n");
	}

	if(readBytes(MS5611_ADDR,3,&buf[3])==0)
	{
		adc=(buf[0]<<16) + (buf[1]<<8) + buf[2];
	}
	else
	{
		printf("Failed to read ADC result.\n");
	}
	return adc;
}

//=============================================================================
// Function Name  : GetTemperature
// Description    : calculate temperature
// Output         : temperature
//=============================================================================
void GetTemperature()
{
	dt=d2 - (((uint32_t)c[5])<<8);
	temp=2000+dt*((uint32_t)c[6])/8388608;
}

//=============================================================================
// Function Name  : MS5611_GetPressure
// Description    : calculate temperaure compensated pressure
// Output         : temperaure compensated pressure
//=============================================================================
void GetCompensatedPressure()
{
	float     t2=0;
	float     off2=0,sens2=0; //
	
	off=(uint32_t)(c[2]<<16)+((uint32_t)c[4]*dt)/128.0;
	sens=(uint32_t)(c[1]<<15)+((uint32_t)c[3]*dt)/256.0;
	if(temp < 2000)  //low temperature compensation when under 20 degrees C
		{
		t2 = (dt*dt)/0x80000000;
  		off2 = 5*((temp-2000)*(temp-2000))/2;
  		sens2 = 5*((temp-2000)*(temp-2000))/4;
  		if(temp < -1500)  //very low temperature compensation when under -15 degrees C
  			{
  			off2 = off2 + 7*((temp+1500)*(temp+1500));
  			sens2 = sens + 11*((temp+1500)*(temp+1500))/2;
  			}
		}
	else  //high temperature above 20 degrees C
		{
  		t2 = 0;
  		off2 = 0;
  		sens2 = 0;
		}
	temp = temp - t2;
	off = off - off2;
	sens = sens - sens2;
	p=(d1*sens/2097152.0-off)/32768.0;
}

uint8_t WriteCommand(uint8_t devAddr,uint8_t data)
{
	return writeBytes(devAddr,1,&data);
}
