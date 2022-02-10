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
	
	dev->fd = err;
	return err;
}

int
close_device(struct device *dev)
{

	while ( close(dev->fd) != 0 )
	{
		// Sleep for 1 second between close calls if unsuccessful
		usleep(1000);
	}

	dev->fd = 0;

	return 0;
}

int
i2c_get_id(struct device *dev, uint8_t *buffer)
{
	int err;

	err = i2c_get_value(dev,BMP180_ID,BMP180_ID_SIZE,buffer);

	return err;
}

uint32_t
i2c_get_temperature(struct device *dev, int32_t *buffer)
{
	uint8_t buffer1 = BMP180_RDTEMP;
	uint8_t MSB;
	uint8_t LSB;
	int err;

	// Start temperature collection
	err = i2c_set_value(dev,BMP180_CTRL,1,&buffer1);

	if (err != 0)
		return -1;

	// Wait for 4.5ms
	err = usleep(4500);
	if (err != 0)
		return -1;

	// Read result
	err = i2c_get_value(dev,0xF6,1,&MSB);
	err += i2c_get_value(dev,0xF7,1,&LSB);
	if (err != 0)
		return -1;

	*buffer = (MSB << 8) + LSB;

	return 0;
}

uint32_t
i2c_get_pressure(struct device *dev, uint8_t oss, int32_t *buffer)
{
	uint8_t config = 0x34 + (oss << 6);
	uint8_t MSB;
	uint8_t LSB;
	uint8_t XLSB;
	int err;

	// Start pressure collection
	err = i2c_set_value(dev,BMP180_CTRL,1,&config);

	if (err != 0)
		return -1;

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

	if (err != 0)
		return -1;
	
	err = i2c_get_value(dev,0xF7,1,&LSB);

	if (err != 0)
		return -1;

	err = i2c_get_value(dev,0xF8,1,&XLSB);
	if (err != 0)
		return -1;


	// Calculate return value
	*buffer = (((MSB <<16) | (LSB<<8) | XLSB) >> (8-oss));

	return 0;
}


int
i2c_get_calibration(struct device *dev, struct BMP180_CALIBRATION *cal)
{
	uint8_t MSB;
	uint8_t LSB;
	uint16_t * field_ptr;
	int err;
	char types[] = "iiiuuuiiiii";
	int j = 0;

	for ( int i = 0; i <= BMP180_CAL_MAX; i = i + 2)
	{
		field_ptr = ((uint16_t *)cal) + j;
		err = i2c_get_value(dev,BMP180_CAL_START_ADDR + i, 1, &MSB);
		if ( err != 0 )
		{
			printf("Error Reading Cal\n");
			return -1;
		}

		err = i2c_get_value(dev,BMP180_CAL_START_ADDR + i + 1, 1, &LSB);
		if ( err != 0 )
		{
			printf("Error Reading Cal\n");
			return -1;
		}
		
		// Check for errors in bytes
		if (MSB == 0x00 && LSB == 0x00)
			return -1;

		if (MSB == 0xFF && MSB == 0xFF)
			return -1;

		switch ( types[j] )
		{
			case 'i':
				*(int16_t *)field_ptr = (MSB << 8) + LSB;
				break;
			case 'u':
				*field_ptr = (MSB << 8) + LSB;
				break;
		}
		j++;
	}
	
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
	if ( err != 0 )
		return -1;

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
