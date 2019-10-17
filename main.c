//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "ms5611.h"

    int fd=-1;

int main()
{

	printf("This is a testing program for ms5611 on Orange Pi... \n");
    fd = open("/dev/i2c-0", O_RDWR);
    if (fd < 0)
    {
        fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
        return(-1);
    }

	if(ms5611_init())
	{
		ms5611_update();
		while(1)
		{
			ms5611_update();
			printf("temperature = %d\tpressure = %d\n",temp,p);
			usleep(20*1000);
		}
	}
	else
		if(!close(fd))
		{
			printf("/dev/i2c-0 closed. \n");
		}
		else
		{
			printf("Failed to close /dev/i2c-0\n");
		}
		return 0;
}
