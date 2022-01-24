#include <unistd.h>
#include "../inc/bmp180.h"

int16_t
bmp180_get_temperature(struct BMP180_CALIBRATION *calib, int16_t UT)
{
	volatile int16_t X1;
	volatile int16_t X2;
	volatile int16_t B5;
	volatile int16_t T;

	X1 = (UT - calib->AC6) * (calib->AC5 / 32768);
	X2 = (calib->MC * 2048) / (X1 + calib->MD);
	calib->B5 = X1 + X2;
	T = (calib->B5 + 8)/16;

	return T;
}
