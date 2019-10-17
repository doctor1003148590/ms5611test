#include <unistd.h>
#include <stdio.h>

#include "ms5611.h"

int main() {
	printf(" This is the testing program of ms5611... \n");
	ms5611_init();
	while(1)
	{
		ms5611_update();
		printf("temperature = %2.1f\tpressure = %2.1f\n",temp,p);
		usleep(20*1000);
	}
	return 0;
}
