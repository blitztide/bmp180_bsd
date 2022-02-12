#include <unistd.h>
#include "../inc/bmp180.h"
#include "../inc/macros.h"

int16_t
bmp180_get_temperature(struct BMP180_CALIBRATION *calib, int16_t UT)
{
	volatile int16_t X1;
	volatile int16_t X2;
	volatile int16_t Y1;
	volatile int16_t T;
	
	X1 = ((UT - calib->AC6) * calib->AC5) >> 15;
	Y1 = X1 + calib->MD;

	// Calculate integer rounding to nearest instead of truncated.
	if ((calib->MC *2048) % Y1 >= Y1/2)
	{
		X2 = ((calib->MC * 2048) / Y1) + 1;
	} else
	{
		X2 = (calib->MC *2048) / Y1;
	}
	calib->B5 = X1 + X2;
	T = (calib->B5 + 8) >> 4;

	return T;
}
