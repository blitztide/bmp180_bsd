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

#include "bmp180.h"
#include "i2c.h"

static struct BMP180_CALIBRATION calibration;

/*
 * Need to send write to 0x77 0xD0, then read from 0x77
 */
uint8_t
i2c_get_id(struct device *dev)
{
	struct iic_msg msg[2];
	struct iic_rdwr_data rdwr;
	uint8_t buffer[1];
	int err;

	// Create write to id register

	memset(buffer, BMP180_ID,1);
	msg[0].slave = BMP180_ADDR;
	msg[0].flags = !IIC_M_RD;
	msg[0].len = sizeof(buffer);
	msg[0].buf = buffer;

	// Create read of id register
	
	msg[1].slave = BMP180_ADDR;
	msg[1].flags = IIC_M_RD;
	msg[1].len = sizeof buffer;
	msg[1].buf = buffer;

	// Create payload
	
	rdwr.nmsgs = 2;
	rdwr.msgs = msg;

	// Send payload
	err = ioctl(dev->fd, I2CRDWR, &rdwr);
	
	if (err < 0)
		return 0x00;
	
	return *buffer;
}

int
i2c_get_calibration(struct device *dev)
{
	return 0;
}

int
i2c_get_value(struct device *dev, uint8_t address, size_t length, void *buffer)
{
	return 0;
}



int
main(int argc, char ** argv)
{

	int err;
	uint8_t id;
	struct device dev;
	strcpy(dev.name, "/dev/iic0");

	init_device(&dev);
	
	if (dev.fd == 0)
		exit(-1);

	id = i2c_get_id(&dev);
	if ( id != 0x55 )
		exit(-1);
	
	printf("Device ID is %x\n",id);
	
	close_device(&dev);

	exit(0);
}
