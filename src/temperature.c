#include "bmp180.h"

long
bmp180_get_temperature(struct BMP180_CALIBRATION *calib, long UT)
{
	long X1;
	long X2;
	long B5;
	long T;

	X1 = (UT - calib->AC6) * (calib->AC5 / 32768);
	X2 = (calib->MC * 2048) / (X1 + calib->MD);
	B5 = X1 + X2;
	T = (B5 + 8)/16;

	return T;
}
