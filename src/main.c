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
main()
{

	int err;
	uint8_t id;
	struct device dev;
	int32_t UT;
	volatile int16_t T;
	int32_t UP;
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

	err = i2c_get_id(&dev, &id);
	if ( err != 0 )
	{
		close_device(&dev);
		exit(-1);
	}

	if ( id != 0x55 )
	{
		close_device(&dev);
		exit(-1);
	}
	
	printf("Device ID: %x\n",id);

	// Calibration

	i2c_get_calibration(&dev,&calibration);

	// Temperature

	err = i2c_get_temperature(&dev, &UT);
	
	if (err != 0)
	{
		close_device(&dev);
		return -1;
	}
	
	T = bmp180_get_temperature(&calibration, UT);
	T = T/10;
	printf("Temperature: %u°C\n",T);

	// Start Pressure Check

	err= i2c_get_pressure(&dev,configuration.oss, &UP);

	if ( err != 0 )
	{
		close_device(&dev);
		return -1;
	}

	P = bmp180_calpressure(&calibration,UP,configuration.oss);
	printf("Pressure: %u hPa\n",P/100);

	// Check Altitude
	RP = 101000;	
	A = get_altitude(P,RP);
	printf("Altitude: %f\n",A);


	close_device(&dev);

	exit(0);
}
