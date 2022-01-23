#include <stdio.h>
#include <fcntl.h>
#include "device.h"

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
