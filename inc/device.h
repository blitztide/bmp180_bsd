#ifndef bmp180_device
#define bmp180_device

struct device {
	int fd;
	char name[16];
};
#endif
