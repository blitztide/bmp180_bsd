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
#include "../inc/pressure.h"
#include "../inc/altitude.h"

static struct BMP180_CALIBRATION calibration;
static struct BMP180_CONF configuration;

int
main(int argc, char ** argv)
{

	int err;
	uint8_t id;
	struct device dev;
	volatile int16_t UT;
	volatile int16_t T;
	volatile uint32_t UP;
	volatile uint32_t P;
	volatile uint32_t RP;
	volatile double A;

	strcpy(dev.name, "/dev/iic0");
	configuration.calib = &calibration;
	configuration.oss = 3;

	init_device(&dev);
	
	if (dev.fd <= 0)
	{
		printf("Unable to open device: %s\n",dev.name);
		exit(-1);
	}

	id = i2c_get_id(&dev);
	if ( id != 0x55 )
		exit(-1);
	
	printf("Device ID is %x\n",id);

	// Calibration

	printf("Getting Calibration data\n");
	i2c_get_calibration(&dev,&calibration);

	// Temperature

	UT = i2c_get_temperature(&dev);
	T = bmp180_get_temperature(&calibration, UT);
	T = T/10;
	printf("Temperature is: %u°C\n",T);

	// Start Pressure Check

	UP = i2c_get_pressure(&dev,configuration.oss);
	printf("UP is: %u\n",UP);
	P = bmp180_calpressure(&calibration,UP,configuration.oss);
	printf("Pressure is: %u\n",P);

	// Check Altitude
	RP = 101000;	
	A = get_altitude(P,RP);
	printf("Altitude is: %f\n",A);


	close_device(&dev);

	exit(0);
}
