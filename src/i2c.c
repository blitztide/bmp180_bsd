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
#include "../inc/i2c.h"


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
	uint8_t buffer;
	int err;

	i2c_get_value(dev,BMP180_ID,BMP180_ID_SIZE,&buffer);

	return buffer;
}

uint32_t
i2c_get_temperature(struct device *dev)
{
	volatile int16_t temperature;
	uint8_t buffer1 = BMP180_RDTEMP;
	uint8_t MSB;
	uint8_t LSB;
	int err;

	// Start temperature collection
	err = i2c_set_value(dev,BMP180_CTRL,1,&buffer1);

	// Wait for 4.5ms
	usleep(4500);	

	// Read result
	err = i2c_get_value(dev,0xF6,1,&MSB);
	err = i2c_get_value(dev,0xF7,1,&LSB);

	temperature = (MSB << 8) + LSB;

	return temperature;
}

uint32_t
i2c_get_pressure(struct device *dev, uint8_t oss)
{
	volatile int16_t pressure;
	uint8_t config = 0x34 + (oss << 6);
	volatile uint8_t MSB;
	volatile uint8_t LSB;
	volatile uint8_t XLSB;
	volatile int32_t UP;
	int err;

	// Start pressure collection
	err = i2c_set_value(dev,BMP180_CTRL,1,&config);

	// Wait for required time
	switch(oss)
	{
		case 0:
			usleep(4500);
			break;
		case 1:
			usleep(7500);
			break;
		case 2:
			usleep(13500);
			break;
		case 3:
			usleep(25500);
			break;
		default:
			usleep(25500);
			break;
	}

	//Read result
	err = i2c_get_value(dev,0xF6,1,&MSB);
	err = i2c_get_value(dev,0xF7,1,&LSB);
	err = i2c_get_value(dev,0xF8,1,&XLSB);

	// Calculate return value
	UP = (MSB <<16) | (LSB<<8) | (XLSB >> (8-oss));
	return UP;
}


int
i2c_get_calibration(struct device *dev, struct BMP180_CALIBRATION *cal)
{
	uint8_t MSB;
	uint8_t LSB;
	
	i2c_get_value(dev,BMP180_AC1H,1,&MSB);
	i2c_get_value(dev,BMP180_AC1L,1,&LSB);
	cal->AC1 = (MSB << 8)+LSB;

	i2c_get_value(dev,BMP180_AC2H,1,&MSB);
	i2c_get_value(dev,BMP180_AC2L,1,&LSB);
	cal->AC2 = (MSB << 8)+LSB;

	i2c_get_value(dev,BMP180_AC3H,1,&MSB);
	i2c_get_value(dev,BMP180_AC3L,1,&LSB);
	cal->AC3 = (MSB << 8)+LSB;

	i2c_get_value(dev,BMP180_AC4H,1,&MSB);
	i2c_get_value(dev,BMP180_AC4L,1,&LSB);
	cal->AC4 = (MSB << 8)+LSB;

	i2c_get_value(dev,BMP180_AC5H,1,&MSB);
	i2c_get_value(dev,BMP180_AC5L,1,&LSB);
	cal->AC5 = (MSB << 8)+LSB;

	i2c_get_value(dev,BMP180_AC6H,1,&MSB);
	i2c_get_value(dev,BMP180_AC6L,1,&LSB);
	cal->AC6 = (MSB << 8)+LSB;

	i2c_get_value(dev,BMP180_B1H,1,&MSB);
	i2c_get_value(dev,BMP180_B1L,1,&LSB);
	cal->B1 = (MSB << 8)+LSB;

	i2c_get_value(dev,BMP180_B2H,1,&MSB);
	i2c_get_value(dev,BMP180_B2L,1,&LSB);
	cal->B2 = (MSB << 8)+LSB;

	i2c_get_value(dev,BMP180_MBH,1,&MSB);
	i2c_get_value(dev,BMP180_MBL,1,&LSB);
	cal->MB = (MSB << 8)+LSB;

	i2c_get_value(dev,BMP180_MCH,1,&MSB);
	i2c_get_value(dev,BMP180_MCL,1,&LSB);
	cal->MC = (MSB << 8)+LSB;

	i2c_get_value(dev,BMP180_MDH,1,&MSB);
	i2c_get_value(dev,BMP180_MDL,1,&LSB);
	cal->MD = (MSB << 8)+LSB;
	
	return 0;
}

int
i2c_get_value(struct device *dev, uint8_t address, size_t length, uint8_t *buffer)
{
	struct iic_msg msg[I2C_MAXLEN];
	struct iic_rdwr_data rdwr;
	int err;
	
	// Select address for read

	msg[0].slave = BMP180_ADDR;
	msg[0].flags = !IIC_M_RD;
	msg[0].len = 1;
	msg[0].buf = &address;

	// Read allocated size;
	
	msg[1].slave = BMP180_ADDR;
	msg[1].flags = IIC_M_RD;
	msg[1].len = length;
	msg[1].buf = buffer;

	// Create payload
	
	rdwr.nmsgs = 2;
	rdwr.msgs = msg;

	// Send payload
	
	err = ioctl(dev->fd, I2CRDWR, &rdwr);
	return err;
}

int
i2c_set_value(struct device *dev, uint8_t address, size_t length, uint8_t *buffer)
{
	struct iic_msg msg[I2C_MAXLEN];
	struct iic_rdwr_data rdwr;
	int err;

	// Select address for write

	msg[0].slave = BMP180_ADDR;
	msg[0].flags = !IIC_M_RD;
	msg[0].len = 1;
	msg[0].buf = &address;

	// Read allocated size;
	
	msg[1].slave = BMP180_ADDR;
	msg[1].flags = !IIC_M_RD;
	msg[1].len = length;
	msg[1].buf = buffer;

	// Create payload
	
	rdwr.nmsgs = 2;
	rdwr.msgs = msg;

	// Send payload
	
	err = ioctl(dev->fd, I2CRDWR, &rdwr);
	return err;
}
