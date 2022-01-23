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

#include "../inc/device.h"
#include "../inc/bmp180.h"


int
init_device(struct device *dev)
{
	int err;

	err = open(dev->name,O_RDWR);
	
	if (err < 0) {
		// Not valid
	}

	dev->fd = err;
	return err;
}

int
close_device(struct device *dev)
{
	int err;

	err = close(dev->fd);

	dev->fd = 0;

	return err;
}

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
i2c_get_value(struct device *dev, uint8_t address, size_t length, char *buffer)
{
	return 0;
}
