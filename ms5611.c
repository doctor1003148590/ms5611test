//ms5611.c
//Initialize and get data from ms5611.
//Check MS5611 datasheet for more information.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ms5611.h"
#include "i2c_ms5611.h"

//variables
uint16_t  c[7]={0}; //values read from PROM
long  d1=0;     //Digital pressure value
long  d2=0;     //Digital temperature value
int64_t   dt=0;     //difference between actual and reference temperature
int32_t   temp=0;   //actual temperature
int64_t   off=0;    //offset at actual temperature
int64_t   sens=0;   //sensitivity at actual temperature
int32_t   p=0;      //temperature compensated pressure

//=============================================================================
// Function Name  : ms5611_init
// Description    : reset ms5611 and read PROM values
//=============================================================================
char ms5611_init()
{
	printf("Initializing ... \n");
	if(!ms5611_reset())
	{
		printf("Ms5611 reset failed.\n");  //read ms5611 PROM values
		return 0;
	}
	if(!ReadProm())
	{
		printf("Read proms failed.\n");
		return 0;
	}
	else
		printf("Ms5611 initialized .\n");
	return 1;
}

char ms5611_reset()
{
	usleep(10000);
	printf("Reseting ms5611... \n");
	if(!writeBytes(MS5611_ADDR,CMD_RESET))
	{
		printf("Ms5611 Reseted. \n");  //reset ms5611
		return 1;
	}
	else
		return 0;
}

//=============================================================================
// Function Name  : ReadProms
// Description    : read the PROM from address 0~7
//=============================================================================
char ReadProm()
{
	printf("Reading PROM... \n");
	uint8_t i=0;
	uint8_t buf[2]={0}; //buffer
	for (i=0;i<=6;i++)
	{
		usleep(10000);
		if (writeBytes(MS5611_ADDR,CMD_PROM_READ_BASE+2*i)==-1)
		{
			printf("Failed to send PROM read request.\n");
			return 0;
		}
		if(readBytes(MS5611_ADDR,2,&buf[0])==0)
		{
			printf("Failed to read PROM data %d.\n",i);
			return 0;
		}
		else
			c[i]=(buf[0]<<8) +  buf[1];
		printf("PROM data %d value is  %d.\n",i,c[i]);
	}
	return 1;
}

//=============================================================================
// Function Name  : ms5611_update
// Description    : update d1 and d2 data
//=============================================================================
void ms5611_update()
{

	if (writeBytes(MS5611_ADDR,CMD_CONVERT_D1_OSR256)==-1)//convert D1 pressure
	{
		printf("Failed to send convert D1 pressure command.\n");
	}
	usleep(10000);
	d1=ReadAdc();//read converted D1 pressure adc to d1
	usleep(10000);

	if (writeBytes(MS5611_ADDR,CMD_CONVERT_D2_OSR256)==-1)//convert D2 temperature
	{
		printf("Failed to send convert D2 temperature command.\n");
	}
	usleep(10000);
	d2=ReadAdc();//read converted D2 temperature adc to d2
	usleep(10000);

	GetTemperature();
	GetCompensatedPressure();
}


//=============================================================================
// Function Name  : ReadAdc
// Description    : read the converted ADC data
// Input          : uint8_t address
// Output         : uint32_t 3 bytes converted ADC data
//=============================================================================
long ReadAdc()
{
	long  adc=0;
	uint8_t   buf[3]={0}; //buffer
	if (writeBytes(MS5611_ADDR,CMD_ADC_READ)==-1)
	{
		printf("Failed to send read ADC result command.\n");
	}

	if(readBytes(MS5611_ADDR,3,&buf[0])==0)
	{
		printf("Failed to read ADC result.\n");
	}
	else
	{
		adc=(buf[0]<<16) + (buf[1]<<8) + buf[2];
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
