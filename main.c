#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "ms5611.h"

int main()
{
    int fd;

	printf("This is a testing program of ms5611 on Orange Pi... \n");
    fd = open("/dev/i2c-0", O_RDWR);
    if (fd < 0)
    {
        fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
        return(-1);
    }


	if(ms5611_init())
	{
		while(1)
		{
			ms5611_update();
			printf("temperature = %d\tpressure = %d\n",temp,p);
			usleep(20*1000);
		}
	}
	else
		close(fd);
		return 0;
}
