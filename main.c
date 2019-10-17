#include <unistd.h>
#include <stdio.h>

#include "ms5611.h"

int main() {
	printf("This is the testing program of ms5611... \n");
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
		return 0;
}
