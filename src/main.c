#include <sys/cdefs.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <dev/iicbus/iic.h>

#include "../inc/bmp180.h"
#include "../inc/i2c.h"
#include "../inc/temperature.h"

static struct BMP180_CALIBRATION calibration;

int
main(int argc, char ** argv)
{

	int err;
	uint8_t id;
	struct device dev;
	volatile long UT;
	volatile long T;
	strcpy(dev.name, "/dev/iic0");

	init_device(&dev);
	
	if (dev.fd == 0)
		exit(-1);

	id = i2c_get_id(&dev);
	if ( id != 0x55 )
		exit(-1);
	
	printf("Device ID is %x\n",id);
	printf("Getting Calibration data\n");
	i2c_get_calibration(&dev,&calibration);
	UT = i2c_get_temperature(&dev);
	T = bmp180_get_temperature(&calibration, UT);
	printf("Temperature is: %lu\n",T);

	long test_temp = 27898;
	printf("Test temperature is: %lu\n",bmp180_get_temperature(&calibration, test_temp));
	close_device(&dev);

	exit(0);
}
