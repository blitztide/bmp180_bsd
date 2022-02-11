#ifndef bmp180_device
#define bmp180_device

#define DEV_NAME_MAX 16

struct device {
	int fd;
	char name[DEV_NAME_MAX];
};
#endif
