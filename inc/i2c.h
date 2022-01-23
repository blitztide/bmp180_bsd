#include <sys/cdefs.h>
#include <unistd.h>
#include <stdlib.h>
#include "device.h"

int init_device(struct device *);
int close_device(struct device *);
uint8_t i2c_get_id(struct device *);
int i2c_get_calibration(struct device *);
int i2c_get_value(struct device *, uint8_t, size_t, char *);
