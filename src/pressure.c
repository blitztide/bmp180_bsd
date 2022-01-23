#include "../inc/bmp180.h"

long
bmp180_calpressure(struct BMP180_CALIBRATION *calib, long UP, short oss)
{
	long B6;
	long X1;
	long X2;
	long X3;
	long B3;
	long B4;
	long B7;
	long P;

	B6 = calib->B5 - 4000;
	X1 = (calib->B2 * ( B6 * (B6/4096))) / 2048;
	X2 = calib->AC2 * (B6 /2048);
	X3 = X1 + X2;
	B3 = (((calib->AC1 * 4 + X3) << oss) + 2)/4;
	X1 = calib->AC3 * (B6 / 8192);
	X2 = (calib->B1 * ( B6 * ( B6 / 4096)))/ 65536;
	X3 = (( X1 + X2) + 2) /4;
	B4 = calib->AC4 * (unsigned long)(X3 + 32768) / 32768;
	B7 = ((unsigned long)UP - B3) * (50000 >> oss);
	if ( B7 < 0x80000000)
	{
		P = (B7 * 2)/B4;
	} else {
		P = (B7/B4)*2;
	}

	X1 = ( P / 256) * (P / 256);
	X1 = ( X1 * 3038) / 65536;
	X2 = (-7357 * P)/65536;

	P = P + (X1 + X2 + 3791)/16;

	return P;
}
