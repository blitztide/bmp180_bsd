#include <unistd.h>
#include <dev/iicbus/iic.h>
#include <capsicum_helpers.h>

#define COMMAND_SIZE 1
const unsigned long commands = { I2CRDWR };

int
init_capsicum(int dev)
{
	int err;

	// Limit IOCTL to I2CRDWR
	err = caph_ioctls_limit(dev,&commands,COMMAND_SIZE);
	if (err != 0)
	{
		return -1;
	}

	// Limit STDIN/STDOUT/STDERR to read/write/write
	err = caph_limit_stdio();
	if (err != 0)
	{
		return -1;
	}
	
	// Enter capability mode
	err = caph_enter();
	if (err != 0)
	{
		return -1;
	}
	return 0;
}
