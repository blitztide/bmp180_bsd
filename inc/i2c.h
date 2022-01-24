#include <sys/cdefs.h>
#include <unistd.h>
#include <stdlib.h>
#include "device.h"

#define I2C_MAXLEN 4

int init_device(struct device *);
int close_device(struct device *);
uint8_t i2c_get_id(struct device *);
int i2c_get_calibration(struct device *, struct BMP180_CALIBRATION *);
long i2c_get_temperature(struct device *);
int i2c_get_value(struct device *, uint8_t, size_t, uint8_t *);
int i2c_set_value(struct device *, uint8_t, size_t, uint8_t *);
